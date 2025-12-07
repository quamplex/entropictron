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
        int sample_channel;
        struct qx_randomizer prob_randomizer;
        struct qx_randomizer randomizer;
        struct qx_randomizer burst_width_randomizer;
        struct qx_randomizer stereo_randomizer;
        struct qx_fader fader;
        struct ent_shelf_filter sh_filter_l;
        struct ent_shelf_filter sh_filter_r;
        float buffer[2][4096];
        size_t burst_index;
        size_t burst_samples;
        float burst_amplitude;
};

struct ent_crackle* ent_crackle_create(int sample_rate)
{
        struct ent_crackle* c = calloc(1, sizeof(struct ent_crackle));
        if (!c)
                return NULL;

        qx_randomizer_init(&c->prob_randomizer, -1.0f, 1.0f, 1.0f / 65536.0f);
        qx_randomizer_init(&c->randomizer, -1.0f, 1.0f, 1.0f / 65536.0f);
        qx_randomizer_init(&c->stereo_randomizer, 0.0f, 1.0f, 1.0f / 65536.0f);
        qx_randomizer_init(&c->burst_width_randomizer, 0.0f, 1.0f, 1.0f / 65536.0f);

        c->sample_rate = sample_rate;
        c->enabled = false;
        c->rate = 20.0f; // 0.5 - 150Hz, default 20Hz
        c->duration = 1.0f; // 0.1-50ms
        c->burst_samples = (c->duration / 1000.0f) * c->sample_rate;
        c->amplitude = 1.0f;
        c->randomness = 1.0f;
        c->brightness = 0.5f;
        c->envelope_shape = ENT_CRACKLE_ENV_EXPONENTIAL;
        c->stereo_spread = 0.0f;
        c->sample_channel = 0;
        c->burst_index = 0;
        c->burst_amplitude = 0.0f;

        qx_fader_init(&c->fader, 50, c->sample_rate);

        ent_shelf_filter_init(&c->sh_filter_l, c->sample_rate, 4000.0f, 1.0f);
        ent_shelf_filter_init(&c->sh_filter_r, c->sample_rate, 4000.0f, 1.0f);

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
        c->rate = qx_clamp_float(rate, 0.5f, 150.0f);
        return ENT_OK;
}

float ent_crackle_get_rate(struct ent_crackle *c)
{
        return c->rate;
}

enum ent_error ent_crackle_set_duration(struct ent_crackle *c, float duration)
{
        ent_log_info("DURATION: %f", duration);
        c->duration = qx_clamp_float(duration, 0.1f, 50.0f);
        return ENT_OK;
}

float ent_crackle_get_duration(struct ent_crackle *c)
{
        return c->duration;
}

enum ent_error ent_crackle_set_amplitude(struct ent_crackle *c, float amplitude)
{
        ent_log_info("AMPLITUDE: %f", amplitude);
        c->amplitude = qx_clamp_float(amplitude, 0.0f, 1.0f);
        return ENT_OK;
}

float ent_crackle_get_amplitude(struct ent_crackle *c)
{
        return c->amplitude;
}

enum ent_error ent_crackle_set_randomness(struct ent_crackle *c, float randomness)
{
        ent_log_info("RNADOMNESS: %f", randomness);
        c->randomness = qx_clamp_float(randomness, 0.01f, 1.0f);
        return ENT_OK;
}

float ent_crackle_get_randomness(struct ent_crackle *c)
{
        return c->randomness;
}

enum ent_error ent_crackle_set_brightness(struct ent_crackle *c, float brightness)
{
        ent_log_info("BRIGHTNESS: %f", brightness);
        c->brightness = 1.0 - qx_clamp_float(brightness, 0.0f, 1.0f);

        float min_cutoff = 4000.0f;
        float max_cutoff = 8000.0f;
        float cutoff = min_cutoff + (max_cutoff - min_cutoff) * c->brightness;

        float min_gain = 1.0f;
        float max_gain = 24.0f;
        float gain = min_gain + (max_gain - min_gain) *  brightness;

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
        ent_log_info("SHAPE: %d", (int)shape);
        if (shape >= ENT_CRACKLE_ENV_NUM_TYPES)
                shape = ENT_CRACKLE_ENV_EXPONENTIAL;
        c->envelope_shape = shape;
        return ENT_OK;
}
enum ent_crackle_envelope ent_crackle_get_envelope_shape(struct ent_crackle *c)
{
        return c->envelope_shape;
}

enum ent_error ent_crackle_set_stereo_spread(struct ent_crackle *c, float spread)
{
        ent_log_info("STEREO: %f", spread);
        c->stereo_spread = qx_clamp_float(spread, 0.0f, 1.0f);
        return ENT_OK;
}

float ent_crackle_get_stereo_spread(struct ent_crackle *c)
{
        return c->stereo_spread;
}

void ent_crackle_process(struct ent_crackle *c, float **data, size_t size)
{
        float val = 0.0f;
        for (size_t i = 0; i < size; i++) {
                 if (c->burst_index > 0) {
                        // Apply envelope
                        float t = (float)c->burst_index / c->burst_samples;
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
                                val = c->burst_amplitude * (1.0f - t);
                                break;
                        case ENT_CRACKLE_ENV_TRIANGLE:
                                val = c->burst_amplitude * (1.0f - fabsf(2.0f * t - 1.0f));
                                break;
                        default:
                                val = 0.0f;
                                break;
                        }

                        c->burst_index++;
                        if (c->burst_index >= c->burst_samples)
                                c->burst_index = 0;
                } else {
                        float sparse_prob = fabs(qx_randomizer_get_float(&c->prob_randomizer));
                        if (sparse_prob <= c->rate / c->sample_rate) {
                                c->burst_index = 1; // first sample of burst

                                // Calculate random burst amplitude.
                                float rand_val = qx_randomizer_get_float(&c->randomizer);
                                float ampl_sign = rand_val >= 0 ? 1.0f : -1.0f;
                                float amp_random = 1.0f - fabs(rand_val) * c->randomness;
                                c->burst_amplitude = ampl_sign * amp_random * c->amplitude;

                                // Calculate random burst width.
                                rand_val = qx_randomizer_get_float(&c->burst_width_randomizer);
                                float rad_duration = 0.1f + (c->duration - 0.1f) * (1.0 - rand_val * c->randomness);
                                c->burst_samples = (rad_duration / 1000.0f) * c->sample_rate;

                                // Calculate random stereo channel
                                rand_val = fabs(qx_randomizer_get_float(&c->stereo_randomizer));
                                float width = c->stereo_spread / 2.0f;
                                if (rand_val < width)
                                        c->sample_channel = 1; // left channel
                                else if (rand_val > 1.0f - width)
                                        c->sample_channel = 2; // right channel
                                else
                                        c->sample_channel = 0; // both channel
                        } else {
                                c->burst_index = 0; // first sample of burst
                                c->burst_amplitude = 0.0;
                        }

                        // apply first sample of burst
                        val *= c->burst_amplitude;
                }

                val = qx_fader_fade(&c->fader, val);

                if (c->sample_channel == 1) {
                        c->buffer[0][i] = val;
                        c->buffer[1][i] = 0.0f;
                } else if (c->sample_channel == 2) {
                        c->buffer[0][i] = 0.0f;
                        c->buffer[1][i] = val;
                } else {
                        c->buffer[0][i] = val;
                        c->buffer[1][i] = val;
                }
        }

        ent_shelf_filter_process(&c->sh_filter_l, c->buffer[0], size);
        ent_shelf_filter_process(&c->sh_filter_r, c->buffer[1], size);

        for (size_t i = 0; i < size; i++) {
                data[0][i] += c->buffer[0][i];
                data[1][i] += c->buffer[1][i];
        }
}
