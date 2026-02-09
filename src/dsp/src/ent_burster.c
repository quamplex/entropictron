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
#include "qx_fader.h"
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
        float gain_randomness;
        float probability;
        bool inverted;

        int sample_rate;

        // Internal state
        struct qx_randomizer randomizer;
        struct qx_fader fader;
        float current_gain;
        float timer_ms;
        float burst_timer_ms;
};

struct ent_rgate* ent_rgate_create(int sample_rate)
{
        struct ent_rgate* b = calloc(1, sizeof(struct ent_rgate));
        if (!b)
                return NULL;

        b->sample_rate = sample_rate;

        // Default parameters
        b->enabled = false;
        b->min_interval = 100.0f;
        b->max_interval = 500.0f;
        b->min_duration = 10.0f;
        b->max_duration = 100.0f;
        b->min_gain = 0.0f;
        b->max_gain = 1.0f;
        b->gain_randomness = 0.2f;
        b->probability = 50.0f;
        b->slew_ms = 5.0f;
        b->inverted = false;

        qx_randomizer_init(&b->randomizer, 0.0f, 1.0f, 1.0f / 65536.0f);
        qx_fader_init(&b->fader, (int)b->slew_ms, sample_rate);

        b->current_gain = 0.0f;
        b->timer_ms = 0.0f;
        b->burst_timer_ms = 0.0f;

        return b;
}

void ent_rgate_free(struct ent_rgate **b)
{
        if (b && *b) {
                free(*b);
                *b = NULL;
        }
}

enum ent_error ent_rgate_enable(struct ent_rgate *b, bool enable)
{
        b->enabled = enable;
        qx_fader_enable(&b->fader, enable);
        return ENT_OK;
}

bool ent_rgate_is_enabled(const struct ent_rgate *b)
{
        return b->enabled;
}

enum ent_error ent_rgate_set_min_interval(struct ent_rgate *b, float val)
{
        b->min_interval = val;
        return ENT_OK;
}

float ent_rgate_get_min_interval(const struct ent_rgate *b)
{
        return b->min_interval;
}

enum ent_error ent_rgate_set_max_interval(struct ent_rgate *b, float val)
{
        b->max_interval = val;
        return ENT_OK;
}

float ent_rgate_get_max_interval(const struct ent_rgate *b)
{
        return b->max_interval;
}

enum ent_error ent_rgate_set_min_duration(struct ent_rgate *b, float val)
{
        b->min_duration = val;
        return ENT_OK;
}

float ent_rgate_get_min_duration(const struct ent_rgate *b)
{
        return b->min_duration;
}

enum ent_error ent_rgate_set_max_duration(struct ent_rgate *b, float val)
{
        b->max_duration = val; return ENT_OK;
}

float ent_rgate_get_max_duration(const struct ent_rgate *b)
{
        return b->max_duration;
}

enum ent_error ent_rgate_set_min_gain(struct ent_rgate *b, float val)
{
        b->min_gain = val;
        return ENT_OK;
}

float ent_rgate_get_min_gain(const struct ent_rgate *b)
{
        return b->min_gain;
}

enum ent_error ent_rgate_set_max_gain(struct ent_rgate *b, float val)
{
        b->max_gain = val;
        return ENT_OK;
}

float ent_rgate_get_max_gain(const struct ent_rgate *b)
{
        return b->max_gain;
}

enum ent_error ent_rgate_set_gain_randomness(struct ent_rgate *b, float val)
{
        b->gain_randomness = val;
        return ENT_OK;
}

float ent_rgate_get_gain_randomness(const struct ent_rgate *b)
{
        return b->gain_randomness;
}

enum ent_error ent_rgate_set_probability(struct ent_rgate *b, float val)
{
        b->probability = val;
        return ENT_OK;
}

float ent_rgate_get_probability(const struct ent_rgate *b)
{
        return b->probability;
}

enum ent_error ent_rgate_set_slew(struct ent_rgate *b, float val)
{
        b->slew_ms = val;
        return ENT_OK;
}

float ent_rgate_get_slew(const struct ent_rgate *b)
{
        return b->slew_ms;
}

enum ent_error ent_rgate_set_inverted(struct ent_rgate *b, bool val)
{
        b->inverted = val;
        return ENT_OK;
}

bool ent_rgate_is_inverteded(const struct ent_rgate *b)
{
        return b->inverted;
}

void ent_rgate_process(struct ent_rgate *b, float **data, size_t size)
{
        for (size_t i = 0; i < size; i++) {
                float in_l = in[0][i];
                float in_r = in[1][i];
                float gain = 0.0f;

                if (b->burst_timer_ms > 0.0f) {
                        b->burst_timer_ms -= ms_per_sample;
                        gain = qx_fader_fade(&b->fader, b->current_gain);
                        if (b->inverted) gain = 1.0f - gain;
                } else {
                        b->timer_ms -= ms_per_sample;
                        if (b->timer_ms <= 0.0f) {
                                float rand_val = qx_randomizer_get_float(&b->randomizer);
                                b->timer_ms = b->min_interval +
                                        (b->max_interval - b->min_interval) * rand_val;
                                rand_val = qx_randomizer_get_float(&b->randomizer) * 100.0f;
                                if (rand_val <= b->probability) {
                                        b->burst_timer_ms = b->min_duration +
                                                (b->max_duration - b->min_duration) *
                                                qx_randomizer_get_float(&b->randomizer);
                                        b->current_gain = b->min_gain +
                                                (b->max_gain - b->min_gain) *
                                                qx_randomizer_get_float(&b->randomizer);
                                }
                        }
                        gain = b->inverted ? 1.0f : 0.0f;
                }

                out[0][i] = in_l * gain;
                out[1][i] = in_r * gain;
        }
}

void ent_rgate_set_state(struct ent_rgate *b, const struct ent_state_rgate *state)
{
        ENT_SET_STATE(b, state, enabled, ent_rgate_enable);
        ENT_SET_STATE(b, state, min_interval, ent_rgate_set_min_interval);
        ENT_SET_STATE(b, state, max_interval, ent_rgate_set_max_interval);
        ENT_SET_STATE(b, state, min_duration, ent_rgate_set_min_duration);
        ENT_SET_STATE(b, state, max_duration, ent_rgate_set_max_duration);
        ENT_SET_STATE(b, state, min_gain, ent_rgate_set_min_gain);
        ENT_SET_STATE(b, state, max_gain, ent_rgate_set_max_gain);
        ENT_SET_STATE(b, state, gain_randomness, ent_rgate_set_gain_randomness);
        ENT_SET_STATE(b, state, probability, ent_rgate_set_probability);
        ENT_SET_STATE(b, state, inverted, ent_rgate_set_inverted);
}

void ent_rgate_get_state(const struct ent_rgate *b, struct ent_state_rgate *state)
{
        ENT_GET_STATE(b, state, enabled, ent_rgate_is_enabled);
        ENT_GET_STATE(b, state, min_interval, ent_rgate_get_min_interval);
        ENT_GET_STATE(b, state, max_interval, ent_rgate_get_max_interval);
        ENT_GET_STATE(b, state, min_duration, ent_rgate_get_min_duration);
        ENT_GET_STATE(b, state, max_duration, ent_rgate_get_max_duration);
        ENT_GET_STATE(b, state, min_gain, ent_rgate_get_min_gain);
        ENT_GET_STATE(b, state, max_gain, ent_rgate_get_max_gain);
        ENT_GET_STATE(b, state, gain_randomness, ent_rgate_get_gain_randomness);
        ENT_GET_STATE(b, state, probability, ent_rgate_get_probability);
        ENT_GET_STATE(b, state, inverted, ent_rgate_is_inverted);
}
