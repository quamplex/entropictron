/**
 * File name: ent_glitch.c
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

#include "ent_glitch.h"
#include "ent_log.h"
#include "qx_math.h"
#include "qx_randomizer.h"
#include "ent_state_internal.h"

#include <stdlib.h>

struct ent_glitch {
        // Parameters
        bool enabled;
        float probability;
        float min_jump;
        float max_jump;
        float length;
        int repeats;
        float dry;
        float wet;

        int sample_rate;
        int glitch_length_samples;
        int jump_min_samples;
        int jump_max_samples;
        float *buffer[2];
        size_t buffer_size;
        size_t write_pos;
        int glitch_pos;
        int glitch_count;
        int glitch_play_pos;
        struct qx_randomizer prob_randomizer;
        struct qx_randomizer randomizer;
        int crossfade_len;
};

struct ent_glitch* ent_glitch_create(int sample_rate)
{
        struct ent_glitch* g = calloc(1, sizeof(struct ent_glitch));
        if (!g)
                return NULL;

        g->sample_rate = sample_rate;
        g->enabled = false;
        g->probability = ENT_GLITCH_DEFAULT_PROB;
        g->jump_min_samples = sample_rate * ENT_GLITCH_DEFAULT_MIN_JUMP / 1000;
        g->jump_max_samples = sample_rate * ENT_GLITCH_DEFAULT_MAX_JUMP / 1000;
        g->glitch_length_samples = sample_rate *  (ENT_GLITCH_DEFAULT_LENGH / 1000.0f);
        g->repeats = ENT_GLITCH_DEFAULT_REPEATS;
        g->dry = ENT_GLITCH_DEFAULT_DRY;
        g->dry = ENT_GLITCH_DEFAULT_WET;

        g->buffer_size = sample_rate * (ENT_GLITCH_MAX_MAX_JUMP + ENT_GLITCH_MAX_LENGH)
                                     * ENT_GLITCH_MAX_REPEATS / 1000.0f;
        g->buffer[0] = calloc(g->buffer_size, sizeof(float));
        g->buffer[1] = calloc(g->buffer_size, sizeof(float));
        g->write_pos = 0;
        g->glitch_pos = -1;
        g->glitch_count = 0;
        g->glitch_play_pos = 0;
        g->crossfade_len = 128;

        qx_randomizer_init(&g->prob_randomizer, 0.0f, 1.0f, 1.0f / 65536.0f);
        qx_randomizer_init(&g->randomizer, 0.0f, 1.0f, 1.0f / 65536.0f);

        return g;
}

void ent_glitch_free(struct ent_glitch **g)
{
        if (g && *g) {
                free((*g)->buffer[0]);
                free((*g)->buffer[1]);
                free(*g);
                *g = NULL;
        }
}

enum ent_error ent_glitch_enable(struct ent_glitch *g, bool b)
{
        g->enabled = b;
        return ENT_OK;
}

bool ent_glitch_is_enabled(const struct ent_glitch *g)
{
        return g->enabled;
}

enum ent_error ent_glitch_set_probability(struct ent_glitch *g, float probability)
{
        g->probability = qx_clamp_float(probability,
                                        ENT_GLITCH_MIN_PROB,
                                        ENT_GLITCH_MAX_PROB);
        return ENT_OK;
}

float ent_glitch_get_probability(const struct ent_glitch *g)
{
        return g->probability;
}

enum ent_error ent_glitch_set_jump_min(struct ent_glitch *g, float jump_min_ms)
{
        g->min_jump = qx_clamp_float(jump_min_ms,
                                     ENT_GLITCH_MIN_MIN_JUMP,
                                     ENT_GLITCH_MAX_MIN_JUMP);
        g->jump_min_samples = (int)(g->min_jump * g->sample_rate / 1000.0f);
        return ENT_OK;
}

float ent_glitch_get_jump_min(const struct ent_glitch *g)
{
        return g->min_jump;
}

enum ent_error ent_glitch_set_jump_max(struct ent_glitch *g, float jump_max_ms)
{
        g->max_jump = qx_clamp_float(jump_max_ms,
                                     ENT_GLITCH_MIN_MAX_JUMP,
                                     ENT_GLITCH_MAX_MAX_JUMP);
        g->jump_max_samples = (int)(g->max_jump * g->sample_rate / 1000.0f);
        return ENT_OK;
}

float ent_glitch_get_jump_max(const struct ent_glitch *g)
{
        return g->max_jump;
}

enum ent_error ent_glitch_set_length(struct ent_glitch *g, float length_ms)
{
        g->length = qx_clamp_float(length_ms,
                                   ENT_GLITCH_MIN_LENGH,
                                   ENT_GLITCH_MAX_LENGH);
        g->glitch_length_samples = (int)(g->length * g->sample_rate / 1000.0f);
        return ENT_OK;
}

float ent_glitch_get_length(const struct ent_glitch *g)
{
        return g->length;
}

enum ent_error ent_glitch_set_repeat_count(struct ent_glitch *g, int repeats)
{
        g->repeats = QX_CLAMP(repeats,
                              ENT_GLITCH_MIN_REPEATS,
                              ENT_GLITCH_MAX_REPEATS);
        return ENT_OK;
}

int ent_glitch_get_repeat_count(const struct ent_glitch *g)
{
        return g->repeats;
}

enum ent_error ent_glitch_set_dry(struct ent_glitch *g, float dry)
{
        g->dry = qx_clamp_float(dry,
                                ENT_GLITCH_MIN_DRY,
                                ENT_GLITCH_MAX_DRY);
        return ENT_OK;
}

float ent_glitch_get_dry(const struct ent_glitch *g)
{
        return g->dry;
}

enum ent_error ent_glitch_set_wet(struct ent_glitch *g, float wet)
{
        g->wet = qx_clamp_float(wet,
                                ENT_GLITCH_MIN_WET,
                                ENT_GLITCH_MAX_WET);
        return ENT_OK;
}

float ent_glitch_get_wet(const struct ent_glitch *g)
{
        return g->wet;
}

void ent_glitch_process(struct ent_glitch *g,
                          float **in,
                          float **out,
                          size_t size)
{
        size_t write_pos   = g->write_pos;
        int g_pos          = g->glitch_pos;
        int g_count        = g->glitch_count;
        int play_pos       = g->glitch_play_pos;
        struct qx_randomizer *prob_randomizer = &g->prob_randomizer;
        struct qx_randomizer *randomizer = &g->randomizer;

        const size_t buf_size = g->buffer_size;
        const int g_len       = g->glitch_length_samples;
        const float prob      = g->probability;
        const int j_min       = g->jump_min_samples;
        const int j_range     = fabsf(g->jump_max_samples - j_min);
        const int repeats     = g->repeats;
        const float dry       = g->dry;
        const float wet       = g->wet;

        const int crossfade_len = g->crossfade_len;
        const float k = 1.0f / (float)crossfade_len;
        const float a = k * (float)g_len;

        float *buf_l = g->buffer[0];
        float *buf_r = g->buffer[1];
        float *in_l  = in[0];
        float *in_r  = in[1];
        float *out_l = out[0];
        float *out_r = out[1];

        for (size_t i = 0; i < size; i++) {
                float s_l = in_l[i];
                float s_r = in_r[i];

                buf_l[write_pos] = s_l;
                buf_r[write_pos] = s_r;

                float gs_l = 0;
                float gs_r = 0;

                if (g_count > 0 && g_pos >= 0) {
                        float crossfade_gain = 1.0f;
                        size_t play_offset = play_pos % g_len;
                        size_t pb_idx = (g_pos + play_offset) % buf_size;

                        if (play_offset < crossfade_len) {
                                // Fade in
                                crossfade_gain = k * (float)play_offset;
                        } else if (play_offset >= g_len - crossfade_len) {
                                // Fade out
                                crossfade_gain = -k * (float)play_offset + a;
                        }

                        gs_l = buf_l[pb_idx] * crossfade_gain;
                        gs_r = buf_r[pb_idx] * crossfade_gain;

                        play_pos++;
                        g_count--;
                } else if (qx_randomizer_get_float(prob_randomizer) < prob) {
                        float jump_prob = qx_randomizer_get_float(randomizer);
                        int jump = j_min + (int)(jump_prob * j_range);

                        g_pos = (write_pos + buf_size - (jump % buf_size)) % buf_size;
                        g_count = g_len * repeats;
                        play_pos = 0;
                }

                out_l[i] += dry * s_l + wet * gs_l;
                out_r[i] += dry * s_r + wet * gs_r;

                write_pos = (write_pos + 1) % buf_size;
        }

        g->write_pos       = write_pos;
        g->glitch_pos      = g_pos;
        g->glitch_count    = g_count;
        g->glitch_play_pos = play_pos;
}

void ent_glitch_set_state(struct ent_glitch *g, const struct ent_state_glitch *state)
{
        ENT_SET_STATE(g, state, enabled,     ent_glitch_enable);
        ENT_SET_STATE(g, state, probability, ent_glitch_set_probability);
        ENT_SET_STATE(g, state, min_jump,    ent_glitch_set_jump_min);
        ENT_SET_STATE(g, state, max_jump,    ent_glitch_set_jump_max);
        ENT_SET_STATE(g, state, length,      ent_glitch_set_length);
        ENT_SET_STATE(g, state, repeats,     ent_glitch_set_repeat_count);
        ENT_SET_STATE(g, state, dry,         ent_glitch_set_dry);
        ENT_SET_STATE(g, state, wet,         ent_glitch_set_wet);
}

void ent_glitch_get_state(const struct ent_glitch *g, struct ent_state_glitch *state)
{
        ENT_GET_STATE(g, state, enabled,     ent_glitch_is_enabled);
        ENT_GET_STATE(g, state, probability, ent_glitch_get_probability);
        ENT_GET_STATE(g, state, min_jump,    ent_glitch_get_jump_min);
        ENT_GET_STATE(g, state, max_jump,    ent_glitch_get_jump_max);
        ENT_GET_STATE(g, state, length,      ent_glitch_get_length);
        ENT_GET_STATE(g, state, repeats,     ent_glitch_get_repeat_count);
        ENT_SET_STATE(g, state, dry,         ent_glitch_get_dry);
        ENT_SET_STATE(g, state, wet,         ent_glitch_get_wet);
}
