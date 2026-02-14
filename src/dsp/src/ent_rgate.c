/**
 * File name: ent_rgate.c
 * Project: Entropictron (A texture synthesizer)
 *
 * Copyright (C) 2026 Iurie Nistor
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

#include "ent_rgate.h"
#include "ent_log.h"
#include "qx_math.h"
#include "qx_smoother.h"
#include "qx_randomizer.h"
#include "ent_state_internal.h"

struct ent_rgate {
        // Parameters
        bool enabled;
        float min_interval;
        float max_interval;
        float min_duration;
        float max_duration;
        float min_gain;
        float max_gain;
        float randomness;
        bool inverted;

        int sample_rate;

        // Internal state
        struct qx_randomizer randomizer;
        struct qx_randomizer randomizer_interval;
        struct qx_randomizer randomizer_duration;
        struct qx_randomizer randomizer_gain;
        struct qx_smoother current_gain;
        float timer_ms;
        float duration;
        float ms_per_sample;
        float interval_range;
        float duration_range;
        float gain_range;
};

struct ent_rgate* ent_rgate_create(int sample_rate)
{
        struct ent_rgate* g = calloc(1, sizeof(struct ent_rgate));
        if (!g)
                return NULL;

        g->sample_rate = sample_rate;
        g->ms_per_sample = 1000.0f / g->sample_rate;

        // Default parameters
        g->enabled = false;
        g->min_interval = ENT_RGATE_DEFAULT_MIN_INTERVAL;
        g->max_interval = ENT_RGATE_DEFAULT_MAX_INTERVAL;
        g->min_duration = ENT_RGATE_DEFAULT_MIN_DURATION;
        g->max_duration = ENT_RGATE_DEFAULT_MAX_DURATION;
        g->min_gain = ENT_RGATE_DEFAULT_MIN_GAIN;
        g->max_gain = ENT_RGATE_DEFAULT_MAX_GAIN;
        g->randomness = ENT_RGATE_DEFAULT_RANDOMNESS;
        g->inverted = ENT_RGATE_DEFAULT_INVERTED;

        // Ranges
        g->interval_range = fabs(g->max_interval - g->min_interval);
        g->duration_range = fabs(g->max_duration - g->min_duration);
        g->gain_range = fabs(g->max_gain - g->min_gain);

        qx_randomizer_init(&g->randomizer, 0.0f, 1.0f, 1.0f / 65536.0f);
        qx_randomizer_init(&g->randomizer_interval, 0.0f, 1.0f, 1.0f / 65536.0f);
        qx_randomizer_init(&g->randomizer_duration, 0.0f, 1.0f, 1.0f / 65536.0f);
        qx_randomizer_init(&g->randomizer_gain, 0.0f, 1.0f, 1.0f / 65536.0f);
        qx_smoother_init(&g->current_gain, ENT_RGATE_DEFAULT_MIN_GAIN, 400.0f);

        g->timer_ms = g->min_interval;
        g->duration = g->min_duration;

        return g;
}

void ent_rgate_free(struct ent_rgate **g)
{
        if (g && *g) {
                free(*g);
                *g = NULL;
        }
}

enum ent_error ent_rgate_enable(struct ent_rgate *g, bool enable)
{
        g->enabled = enable;
        //        qx_fader_enable(&g->fader, enable);
        return ENT_OK;
}

bool ent_rgate_is_enabled(const struct ent_rgate *g)
{
        return g->enabled;
}

enum ent_error ent_rgate_set_min_interval(struct ent_rgate *g, float val)
{
        g->min_duration = qx_clamp_float(val,
                                         ENT_RGATE_MIN_MIN_DURATION,
                                         ENT_RGATE_MAX_MIN_DURATION);
        g->interval_range = fabs(g->max_interval - g->min_interval);
        return ENT_OK;
}

float ent_rgate_get_min_interval(const struct ent_rgate *g)
{
        return g->min_interval;
}

enum ent_error ent_rgate_set_max_interval(struct ent_rgate *g, float val)
{
        g->max_interval = qx_clamp_float(val,
                                         ENT_RGATE_MIN_MAX_INTERVAL,
                                         ENT_RGATE_MAX_MAX_INTERVAL);
        g->interval_range = fabs(g->max_interval - g->min_interval);
        return ENT_OK;
}

float ent_rgate_get_max_interval(const struct ent_rgate *g)
{
        return g->max_interval;
}

enum ent_error ent_rgate_set_min_duration(struct ent_rgate *g, float val)
{
        g->min_duration = qx_clamp_float(val,
                                         ENT_RGATE_MIN_MIN_DURATION,
                                         ENT_RGATE_MAX_MIN_DURATION);
        g->duration_range = fabs(g->max_duration - g->min_duration);
        return ENT_OK;
}

float ent_rgate_get_min_duration(const struct ent_rgate *g)
{
        return g->min_duration;
}

enum ent_error ent_rgate_set_max_duration(struct ent_rgate *g, float val)
{
        g->max_duration = qx_clamp_float(val,
                                         ENT_RGATE_MIN_MAX_DURATION,
                                         ENT_RGATE_MAX_MAX_DURATION);
        g->duration_range = fabs(g->max_duration - g->min_duration);
        return ENT_OK;
}

float ent_rgate_get_max_duration(const struct ent_rgate *g)
{
        return g->max_duration;
}

enum ent_error ent_rgate_set_min_gain(struct ent_rgate *g, float val)
{
        g->min_gain = qx_clamp_float(val,
                                     ENT_RGATE_MIN_MIN_GAIN,
                                     ENT_RGATE_MAX_MIN_GAIN);
        g->gain_range = fabs(g->max_gain - g->min_gain);
        ent_log_info("min_gain: [v]%f, %f, r: %f", val, g->max_gain, g->gain_range);
        return ENT_OK;
}

float ent_rgate_get_min_gain(const struct ent_rgate *g)
{
        return g->min_gain;
}

enum ent_error ent_rgate_set_max_gain(struct ent_rgate *g, float val)
{
        g->max_gain = qx_clamp_float(val,
                                     ENT_RGATE_MIN_MAX_GAIN,
                                     ENT_RGATE_MAX_MAX_GAIN);
        g->gain_range = fabs(g->max_gain - g->min_gain);
        ent_log_info("max_gain: [v]%f, %f, r: %f", val, g->max_gain, g->gain_range);
        return ENT_OK;
}

float ent_rgate_get_max_gain(const struct ent_rgate *g)
{
        return g->max_gain;
}

enum ent_error ent_rgate_set_randomness(struct ent_rgate *g, float val)
{
        g->randomness = qx_clamp_float(val,
                                       ENT_RGATE_MIN_RANDOMNESS,
                                       ENT_RGATE_MAX_RANDOMNESS);
        return ENT_OK;
}

float ent_rgate_get_randomness(const struct ent_rgate *g)
{
        return g->randomness;
}

enum ent_error ent_rgate_set_inverted(struct ent_rgate *g, bool val)
{
        g->inverted = val;
        return ENT_OK;
}

bool ent_rgate_get_inverted(const struct ent_rgate *g)
{
        return g->inverted;
}

void ent_rgate_process(struct ent_rgate *g,
                       float **in,
                       float **out,
                       size_t size)
{
        float duration      = g->duration;
        float timer_ms      = g->timer_ms;
        float ms_per_sample = g->ms_per_sample;

        float randomness    = g->randomness;
        float min_interval  = g->min_interval;
        float interval_rng  = g->interval_range;
        float min_duration  = g->min_duration;
        float duration_rng  = g->duration_range;
        float min_gain      = g->min_gain;
        float gain_rng      = g->gain_range;
        bool  inverted      = g->inverted;

        struct qx_smoother   *current_gain  = &g->current_gain;
        struct qx_randomizer *rand_interval = &g->randomizer_interval;
        struct qx_randomizer *rand_duration = &g->randomizer_duration;
        struct qx_randomizer *rand_gain     = &g->randomizer_gain;
        struct qx_randomizer *rand_main     = &g->randomizer;

        float *inL  = in[0];
        float *inR  = in[1];
        float *outL = out[0];
        float *outR = out[1];

        for (size_t i = 0; i < size; i++) {
                float in_l = inL[i];
                float in_r = inR[i];

                if (duration > 0.0f) {
                        duration -= ms_per_sample;
                } else {
                        qx_smoother_set_target(current_gain, 1.0f);
                        timer_ms -= ms_per_sample;

                        if (timer_ms <= 0.0f) {
                                float rv = qx_randomizer_get_float(rand_interval);
                                timer_ms = min_interval + interval_rng * rv;

                                rv = qx_randomizer_get_float(rand_main);
                                if (rv <= randomness) {
                                        rv = qx_randomizer_get_float(rand_duration);
                                        duration = min_duration + duration_rng * rv;

                                        rv = qx_randomizer_get_float(rand_gain);
                                        float new_gain = min_gain + gain_rng * rv;

                                        qx_smoother_set_target(current_gain, new_gain);
                                }
                        }
                }

                float gain = qx_smoother_next(current_gain);
                gain = inverted ? 1.0f - gain : gain;

                outL[i] += in_l * gain;
                outR[i] += in_r * gain;
        }

        g->duration = duration;
        g->timer_ms = timer_ms;
}

void ent_rgate_set_state(struct ent_rgate *g, const struct ent_state_rgate *state)
{
        ENT_SET_STATE(g, state, enabled, ent_rgate_enable);
        ENT_SET_STATE(g, state, min_interval, ent_rgate_set_min_interval);
        ENT_SET_STATE(g, state, max_interval, ent_rgate_set_max_interval);
        ENT_SET_STATE(g, state, min_duration, ent_rgate_set_min_duration);
        ENT_SET_STATE(g, state, max_duration, ent_rgate_set_max_duration);
        ENT_SET_STATE(g, state, min_gain, ent_rgate_set_min_gain);
        ENT_SET_STATE(g, state, max_gain, ent_rgate_set_max_gain);
        ENT_SET_STATE(g, state, randomness, ent_rgate_set_randomness);
        ENT_SET_STATE(g, state, inverted, ent_rgate_set_inverted);
 }

void ent_rgate_get_state(const struct ent_rgate *g, struct ent_state_rgate *state)
{
        ENT_GET_STATE(g, state, enabled, ent_rgate_is_enabled);
        ENT_GET_STATE(g, state, min_interval, ent_rgate_get_min_interval);
        ENT_GET_STATE(g, state, max_interval, ent_rgate_get_max_interval);
        ENT_GET_STATE(g, state, min_duration, ent_rgate_get_min_duration);
        ENT_GET_STATE(g, state, max_duration, ent_rgate_get_max_duration);
        ENT_GET_STATE(g, state, min_gain, ent_rgate_get_min_gain);
        ENT_GET_STATE(g, state, max_gain, ent_rgate_get_max_gain);
        ENT_GET_STATE(g, state, randomness, ent_rgate_get_randomness);
        ENT_GET_STATE(g, state, inverted, ent_rgate_get_inverted);
}
