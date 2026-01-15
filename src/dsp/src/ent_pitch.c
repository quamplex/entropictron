/**
 * File name: ent_pitch.c
 * Project: Entropictron (A texture synthesizer)
 *
 * Copyright (C) 2025 Iurie Nistor
 *
 * This file is part of Entropictron.
 *
 * Entropictron is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "ent_pitch.h"
#include "ent_log.h"
#include "ent_state_internal.h"

#include "qx_math.h"
#include "qx_randomizer.h"
#include "qx_fader.h"
#include "qx_smoother.h"

#define ENT_PITCH_RING_BUFFER_SIZE 10 * 48000

struct ent_pitch {
        // Parameters
        bool enabled;
        float pitch;
        float fine;
        float depth;
        float drift;

        int sample_rate;
        struct qx_smoother entropy;

        size_t buffer_size;
        size_t write_pos;
        float read_pos_f;
        float min_delay;
        float base_ratio;
        float buffer[2][ENT_PITCH_RING_BUFFER_SIZE];
};

struct ent_pitch* ent_pitch_create(int sample_rate)
{
        struct ent_pitch *m = calloc(1, sizeof(struct ent_pitch));
        if (!m)
                return NULL;

        m->enabled = true;
        m->pitch = ENT_PITCH_DEFAULT_PITCH;
        m->fine  = ENT_PITCH_DEFAULT_FINE;
        m->depth = ENT_PITCH_DEFAULT_DEPTH;
        m->drift = ENT_PITCH_DEFAULT_DRIFT;
        m->min_delay = ENT_PITCH_RING_BUFFER_SIZE / 2;
        m->write_pos = 0;
        m->read_pos_f = m->buffer_size - m->min_delay;

        m->sample_rate = sample_rate;
        m->buffer_size = ENT_PITCH_RING_BUFFER_SIZE;

        qx_smoother_init(&m->entropy, 0.0f, 20.0f);

        m->base_ratio = powf(2.0f, m->pitch / 12.0f) *
                        powf(2.0f, m->fine / 1200.0f);

        return m;
}

void ent_pitch_free(struct ent_pitch **m)
{
        if (m && *m) {
                free(*m);
                *m = NULL;
        }
}

void ent_pitch_enable(struct ent_pitch *m, bool b)
{
        m->enabled = b;
}

bool ent_pitch_is_enabled(const struct ent_pitch *m)
{
        return m->enabled;
}

void ent_pitch_set_pitch(struct ent_pitch *m, float semitones)
{
        m->pitch = qx_clamp_float(semitones,
                                  ENT_PITCH_PITCH_MIN,
                                  ENT_PITCH_PITCH_MAX);
        m->base_ratio = powf(2.0f, m->pitch / 12.0f) *
                        powf(2.0f, m->fine / 1200.0f);
}

float ent_pitch_get_pitch(const struct ent_pitch *m)
{
        return m->pitch;
}

void ent_pitch_set_fine(struct ent_pitch *m, float cents)
{
        m->fine = qx_clamp_float(cents,
                                 ENT_PITCH_FINE_MIN,
                                 ENT_PITCH_FINE_MIN);
        m->base_ratio = powf(2.0f, m->pitch / 12.0f) *
                        powf(2.0f, m->fine / 1200.0f);
}

float ent_pitch_get_fine(const struct ent_pitch *m)
{
        return m->fine;
}

void ent_pitch_set_depth(struct ent_pitch *m, float depth)
{
        m->depth = qx_clamp_float(depth,
                                  ENT_PITCH_DEPTH_MIN,
                                  ENT_PITCH_DEPTH_MAX);
}

float ent_pitch_get_depth(const struct ent_pitch *m)
{
        return m->depth;
}

void ent_pitch_set_drift(struct ent_pitch *m, float drift)
{
        m->drift = qx_clamp_float(drift,
                                  ENT_PITCH_DRIFT_MIN,
                                  ENT_PITCH_DRIFT_MAX);
}

float ent_pitch_get_drift(const struct ent_pitch *m)
{
        return m->drift;
}

void ent_pitch_set_entropy(struct ent_pitch *m, float entropy)
{
        qx_smoother_set_target(&m->entropy, entropy);
}

float ent_pitch_get_entropy(const struct ent_pitch *m)
{
        return qx_smoother_get(&m->entropy);
}

void ent_pitch_process(struct ent_pitch *m,
                       float **in,
                       float **out,
                       size_t size)
{
        float entropy = qx_smoother_next(&m->entropy);
        //        ent_log_info("entropy: %f", entropy);

        float drift = entropy * 2 * m->depth;
        float ratio = m->base_ratio * (1.0f + drift);
        ratio = qx_clamp_float(ratio, 0.5f, 2.0f);
        //if (entropy < 0.0)
        //        ratio *= -1;
        //        ent_log_info("ratio: %f", ratio);

        for (size_t i = 0; i < size; i++) {
                m->buffer[0][m->write_pos] = in[0][i];
                m->buffer[1][m->write_pos] = in[1][i];

                float out_l = qx_ring_interp_linear(m->buffer[0],
                                                    m->read_pos_f,
                                                    m->buffer_size);
                float out_r = qx_ring_interp_linear(m->buffer[1],
                                                    m->read_pos_f,
                                                    m->buffer_size);

                out[0][i] += out_l;
                out[1][i] += out_r;

                /* advance write head */
                m->write_pos++;
                if (m->write_pos >= m->buffer_size)
                        m->write_pos = 0;

                /* advance read head */
                m->read_pos_f += ratio;
                if (m->read_pos_f >= m->buffer_size)
                        m->read_pos_f -= m->buffer_size;

                /* ---- SAFETY: keep minimum delay ---- */
                float distance = (float)m->write_pos - m->read_pos_f;
                if (distance < 0.0f)
                        distance += m->buffer_size;

                if (distance < m->min_delay) {
                        m->read_pos_f =
                                (float)m->write_pos - m->min_delay;
                        if (m->read_pos_f < 0.0f)
                                m->read_pos_f += m->buffer_size;
                }
        }
}
