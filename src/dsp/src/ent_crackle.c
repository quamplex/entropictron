/**
 * File name: ent_crackle.c
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

#include "ent_crackle.h"
#include "ent_log.h"
#include "qx_math.h"
#include "qx_fader.h"
#include "qx_randomizer.h"
#include "ent_shelf_filter.h"

#include <stdlib.h>

struct ent_crackle {
        int sample_rate;
        bool enabled;
        float rate;
        float duration;
        float amplitude;
        float randomness;
        float brightness;
        enum ent_crackle_envelope envelope_shape;
        float stereo_spread;
        struct qx_randomizer prob_randomizer;
        struct qx_randomizer randomizer;
        struct qx_fader fader;
        struct ent_shelf_filter sh_filter_l;
        struct ent_shelf_filter sh_filter_r;
        float buffer[2][4096];
        size_t burst_index;
        float burst_amplitude;
};

struct ent_crackle* ent_crackle_create(int sample_rate)
{
        struct ent_crackle* c = calloc(1, sizeof(struct ent_crackle));
        if (!c)
                return NULL;

        c->sample_rate = sample_rate;
        c->enabled = false;
        c->rate = 10.0f;
        c->duration = 1.0f;
        c->amplitude = 1.0f;
        c->randomness = 0.5f;
        c->brightness = 0.5f;
        c->envelope_shape = ENT_CRACKLE_ENV_EXPONENTIAL;
        c->stereo_spread = 0.0f;
        c->burst_index = 0;
        c->burst_amplitude = 0.0f;

        qx_randomizer_init(&c->prob_randomizer, -1.0f, 1.0f, 1.0f / 65536.0f);
        qx_randomizer_init(&c->randomizer, -1.0f, 1.0f, 1.0f / 65536.0f);
        qx_fader_init(&c->fader, 50, c->sample_rate);

        ent_shelf_filter_init(&c->sh_filter_l, c->sample_rate, 8000.0f, 1.0f);
        ent_shelf_filter_init(&c->sh_filter_r, c->sample_rate, 8000.0f, 1.0f);

        return c;
}

void ent_crackle_free(struct ent_crackle **c)
{
        if (c && *c) {
                free(*c);
                *c = NULL;
        }
}

enum ent_error ent_crackle_enable(struct ent_crackle *c, bool b)
{
        c->enabled = b;
        qx_fader_enable(&c->fader, b);
        return ENT_OK;
}

bool ent_crackle_is_enabled(struct ent_crackle *c)
{
        return c->enabled;
}

enum ent_error ent_crackle_set_rate(struct ent_crackle *c, float rate)
{
        ent_log_info("RATE: %f", rate);
        c->rate = rate;
        return ENT_OK;
}

float ent_crackle_get_rate(struct ent_crackle *c)
{
        return c->rate;
}

enum ent_error ent_crackle_set_duration(struct ent_crackle *c, float duration)
{
        c->duration = duration;
        return ENT_OK;
}

float ent_crackle_get_duration(struct ent_crackle *c)
{
        return c->duration;
}

enum ent_error ent_crackle_set_amplitude(struct ent_crackle *c, float amplitude)
{
        c->amplitude = amplitude;
        return ENT_OK;
}

float ent_crackle_get_amplitude(struct ent_crackle *c)
{
        return c->amplitude;
}

enum ent_error ent_crackle_set_randomness(struct ent_crackle *c, float randomness)
{
        c->randomness = randomness;
        return ENT_OK;
}

float ent_crackle_get_randomness(struct ent_crackle *c)
{
        return c->randomness;
}

enum ent_error ent_crackle_set_brightness(struct ent_crackle *c, float brightness)
{
        c->brightness = qx_clamp_float(brightness, 0.0f, 1.0f);

        float min_cutoff = 1000.0f;
        float max_cutoff = 8000.0f;
        float cutoff = min_cutoff + (max_cutoff - min_cutoff) * c->brightness;
        float gain = 1.0f + 23.0f * c->brightness;

        ent_shelf_filter_set_cutoff(&c->sh_filter_l, c->sample_rate, cutoff, gain);
        ent_shelf_filter_set_cutoff(&c->sh_filter_r, c->sample_rate, cutoff, gain);

        return ENT_OK;
}
float ent_crackle_get_brightness(struct ent_crackle *c)
{
        return c->brightness;
}

enum ent_error ent_crackle_set_envelope_shape(struct ent_crackle *c,
                                              enum ent_crackle_envelope shape)
{
        if (shape >= ENT_CRACKLE_ENV_NUM_TYPES)
                return ENT_ERROR_WRONG_ARGUMENTS;
        c->envelope_shape = shape;
        return ENT_OK;
}
enum ent_crackle_envelope ent_crackle_get_envelope_shape(struct ent_crackle *c)
{
        return c->envelope_shape;
}

enum ent_error ent_crackle_set_stereo_spread(struct ent_crackle *c, float spread)
{
        c->stereo_spread = spread;
        return ENT_OK;
}

float ent_crackle_get_stereo_spread(struct ent_crackle *c)
{
        return c->stereo_spread;
}

void ent_crackle_process(struct ent_crackle *c, float **data, size_t size)
{
        float burst_samples = (c->duration / 1000.0f) * c->sample_rate;

        float val = 0.0f;
        for (size_t i = 0; i < size; i++) {
                // Step 1: Micro noise based on density
                //float micro_prob = qx_randomizer_get_float(&c->prob_randomizer);
                //if (micro_prob <= c->density)
                //val = qx_randomizer_get_float(&c->randomizer);

                // Step 2: Burst handling
                if (c->burst_index > 0) {
                        // Apply envelope
                        float t = (float)c->burst_index / burst_samples;
                        switch(c->envelope_shape) {
                        case ENT_CRACKLE_ENV_EXPONENTIAL:
                        {
                                float decay = 10.0f;
                                float denom = 1.0f - expf(-decay);
                                if (fabsf(denom) < 1e-6f)
                                        denom = 1e-6f;

                                float env = (expf(-decay * t) - expf(-decay)) / denom;
                                val = c->burst_amplitude * env;
                                break;
                        }
                        case ENT_CRACKLE_ENV_LINEAR:
                                val *= c->burst_amplitude * (1.0f - t);
                                break;
                        case ENT_CRACKLE_ENV_TRIANGLE:
                                val *= c->burst_amplitude * (1.0f - fabsf(2.0f * t - 1.0f));
                                break;
                        default:
                                break;
                        }

                        c->burst_index++;
                        if (c->burst_index >= burst_samples)
                                c->burst_index = 0; // burst ends
                } else {
                        // Only trigger a new burst if not in a burst
                        float sparse_prob = fabs(qx_randomizer_get_float(&c->prob_randomizer));
                        if (sparse_prob <= 1.0f / (c->rate + 1.0f)) {
                                c->burst_index = 1; // first sample of burst
                                float amp_random = qx_randomizer_get_float(&c->randomizer);
                                c->burst_amplitude = amp_random * c->amplitude;
                        } else {
                                c->burst_index = 0; // first sample of burst
                                c->burst_amplitude = 0.0;
                        }
                        val = c->burst_amplitude; // apply first sample of burst
                }

                // Apply fader if needed
                //val = qx_fader_fade(&c->fader, val);

                // Copy to internal buffer
                c->buffer[0][i] = val;
                c->buffer[1][i] = val;
        }

        // Apply brightness (high-shelf filter)
        //        ent_shelf_filter_process(&c->sh_filter_l, c->buffer[0], size);
        //        ent_shelf_filter_process(&c->sh_filter_r, c->buffer[1], size);

        // Mix to output
        for (size_t i = 0; i < size; i++) {
                data[0][i] += c->buffer[0][i];
                data[1][i] += c->buffer[1][i];
        }
}
