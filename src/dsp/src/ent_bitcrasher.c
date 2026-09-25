/**
 * File name: ent_bitcrasher.c
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

#include "ent_bitcrasher.h"
#include "ent_state_internal.h"
#include "qx_math.h"
#include "qx_randomizer.h"

#include <math.h>
#include <stdlib.h>

struct ent_bitcrasher {
        // Parameters
        bool enabled;
        int bits;
        float rate;
        float chaos;
        float gain;
        float mix;

        float phase;
        float held_sample[2];
        struct qx_randomizer prob_randomizer;
        struct qx_randomizer randomizer;
};

struct ent_bitcrasher* ent_bitcrasher_create(int sample_rate)
{
        (void)sample_rate;

        struct ent_bitcrasher *b = calloc(1, sizeof(*b));
        if (!b)
                return NULL;

        b->bits = ENT_BITCRASHER_DEFAULT_BITS;
        b->rate = ENT_BITCRASHER_DEFAULT_RATE;
        b->chaos = ENT_BITCRASHER_DEFAULT_CHAOS;
        b->gain = qx_db_to_val(ENT_BITCRASHER_DEFAULT_GAIN);
        b->mix = ENT_BITCRASHER_DEFAULT_MIX;
        b->phase = 1.0f;
        qx_randomizer_init(&b->prob_randomizer,
                           0.0f,
                           1.0f,
                           1.0f / 65536.0f);
        qx_randomizer_init(&b->randomizer,
                           0.0f,
                           1.0f,
                           1.0f / 65536.0f);

        return b;
}

void ent_bitcrasher_free(struct ent_bitcrasher **b)
{
        if (b && *b) {
                free(*b);
                *b = NULL;
        }
}

enum ent_error ent_bitcrasher_enable(struct ent_bitcrasher *b, bool enabled)
{
        b->enabled = enabled;
        return ENT_OK;
}

bool ent_bitcrasher_is_enabled(const struct ent_bitcrasher *b)
{
        return b->enabled;
}

enum ent_error ent_bitcrasher_set_bits(struct ent_bitcrasher *b, int bits)
{
        b->bits = QX_CLAMP(bits,
                           ENT_BITCRASHER_MIN_BITS,
                           ENT_BITCRASHER_MAX_BITS);
        return ENT_OK;
}

int ent_bitcrasher_get_bits(const struct ent_bitcrasher *b)
{
        return b->bits;
}

enum ent_error ent_bitcrasher_set_rate(struct ent_bitcrasher *b, float rate)
{
        b->rate = qx_clamp_float(rate,
                                 ENT_BITCRASHER_MIN_RATE,
                                 ENT_BITCRASHER_MAX_RATE);
        return ENT_OK;
}

float ent_bitcrasher_get_rate(const struct ent_bitcrasher *b)
{
        return b->rate;
}

enum ent_error ent_bitcrasher_set_chaos(struct ent_bitcrasher *b, float chaos)
{
        b->chaos = qx_clamp_float(chaos,
                                  ENT_BITCRASHER_MIN_CHAOS,
                                  ENT_BITCRASHER_MAX_CHAOS);
        return ENT_OK;
}

float ent_bitcrasher_get_chaos(const struct ent_bitcrasher *b)
{
        return b->chaos;
}

enum ent_error ent_bitcrasher_set_gain(struct ent_bitcrasher *b, float gain)
{
        b->gain = qx_clamp_float(gain,
                                 qx_db_to_val(ENT_BITCRASHER_MIN_GAIN),
                                 qx_db_to_val(ENT_BITCRASHER_MAX_GAIN));
        return ENT_OK;
}

float ent_bitcrasher_get_gain(const struct ent_bitcrasher *b)
{
        return b->gain;
}

enum ent_error ent_bitcrasher_set_mix(struct ent_bitcrasher *b, float mix)
{
        b->mix = qx_clamp_float(mix,
                                ENT_BITCRASHER_MIN_MIX,
                                ENT_BITCRASHER_MAX_MIX);
        return ENT_OK;
}

float ent_bitcrasher_get_mix(const struct ent_bitcrasher *b)
{
        return b->mix;
}

/*
 * Process one stereo audio block.
 *
 * The input is reduced in time by periodically capturing one sample for
 * each channel, quantizing it to the current bit depth,
 * and holding it until the next capture. Rate controls the capture interval.
 * Chaos can change the bit depth and rate while processing.
 */
void ent_bitcrasher_process(struct ent_bitcrasher *b,
                            float **in,
                            float **out,
                            size_t size,
                            float entropy)
{
        const float chaos = qx_clamp_float(0.5 * b->chaos * (1.0f - entropy),
                                           ENT_BITCRASHER_MIN_CHAOS,
                                           ENT_BITCRASHER_MAX_CHAOS);
        int bits = b->bits;
        float rate = b->rate;
        const float gain = b->gain;
        float mix = b->mix;
        float phase = b->phase;
        float *held_sample = b->held_sample;
        float levels = (float)((1u << bits) - 1u);

        for (size_t i = 0; i < size; i++) {
                if (qx_randomizer_get_float(&b->prob_randomizer) < chaos) {
                        bits = ENT_BITCRASHER_MIN_BITS
                                + (int)(qx_randomizer_get_float(&b->randomizer)
                                        * (b->bits - ENT_BITCRASHER_MIN_BITS + 1));
                        levels = (float)((1u << bits) - 1u);
                        rate = ENT_BITCRASHER_MIN_RATE
                                + qx_randomizer_get_float(&b->randomizer)
                                * (b->rate - ENT_BITCRASHER_MIN_RATE);
                }

                /*
                 * Phase is progress toward the next sample capture.
                 * A constant rate advances it steadily; when it
                 * reaches one, capture and hold a new quantized sample.
                 */
                phase += rate;
                if (phase >= 1.0f) {
                        phase -= 1.0f;
                        for (size_t channel = 0; channel < 2; channel++) {
                                float sample = qx_clamp_float(in[channel][i], -1.0f, 1.0f);
                                held_sample[channel] = roundf((sample + 1.0f) * 0.5f * levels)
                                        / levels * 2.0f - 1.0f;
                        }
                }

                for (size_t channel = 0; channel < 2; channel++) {
                        const float dry = in[channel][i];
                        if (dry == 0.0f)
                                held_sample[channel] = 0.0f;
                        out[channel][i] += (dry * (1.0f - mix)
                                            + held_sample[channel] * mix) * gain;
                }
        }

        b->phase = phase;
}

void ent_bitcrasher_set_state(struct ent_bitcrasher *b,
                              const struct ent_state_bitcrasher *state)
{
        ENT_SET_STATE(b, state, enabled, ent_bitcrasher_enable);
        ENT_SET_STATE(b, state, bits, ent_bitcrasher_set_bits);
        ENT_SET_STATE(b, state, rate, ent_bitcrasher_set_rate);
        ENT_SET_STATE(b, state, chaos, ent_bitcrasher_set_chaos);
        ENT_SET_STATE(b, state, gain, ent_bitcrasher_set_gain);
        ENT_SET_STATE(b, state, mix, ent_bitcrasher_set_mix);
}

void ent_bitcrasher_get_state(const struct ent_bitcrasher *b,
                              struct ent_state_bitcrasher *state)
{
        ENT_GET_STATE(b, state, enabled, ent_bitcrasher_is_enabled);
        ENT_GET_STATE(b, state, bits, ent_bitcrasher_get_bits);
        ENT_GET_STATE(b, state, rate, ent_bitcrasher_get_rate);
        ENT_GET_STATE(b, state, chaos, ent_bitcrasher_get_chaos);
        ENT_GET_STATE(b, state, gain, ent_bitcrasher_get_gain);
        ENT_GET_STATE(b, state, mix, ent_bitcrasher_get_mix);
}
