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
#include "ent_log.h"
#include "ent_state_internal.h"
#include "qx_math.h"

#include <stdlib.h>

struct ent_bitcrasher {
        // Parameters
        bool enabled;
        int bits;
        float rate;
        float chaos;
        float fold;
        float mix;
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
        b->fold = ENT_BITCRASHER_DEFAULT_FOLD;
        b->mix = ENT_BITCRASHER_DEFAULT_MIX;

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
        ent_log_info("bitcrasher enabled: %s", enabled ? "true" : "false");
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
        ent_log_info("bitcrasher bits: %d", b->bits);
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
        ent_log_info("bitcrasher rate: %f", b->rate);
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
        ent_log_info("bitcrasher chaos: %f", b->chaos);
        return ENT_OK;
}

float ent_bitcrasher_get_chaos(const struct ent_bitcrasher *b)
{
        return b->chaos;
}

enum ent_error ent_bitcrasher_set_fold(struct ent_bitcrasher *b, float fold)
{
        b->fold = qx_clamp_float(fold,
                                 ENT_BITCRASHER_MIN_FOLD,
                                 ENT_BITCRASHER_MAX_FOLD);
        ent_log_info("bitcrasher fold: %f", b->fold);
        return ENT_OK;
}

float ent_bitcrasher_get_fold(const struct ent_bitcrasher *b)
{
        return b->fold;
}

enum ent_error ent_bitcrasher_set_mix(struct ent_bitcrasher *b, float mix)
{
        b->mix = qx_clamp_float(mix,
                                ENT_BITCRASHER_MIN_MIX,
                                ENT_BITCRASHER_MAX_MIX);
        ent_log_info("bitcrasher mix: %f", b->mix);
        return ENT_OK;
}

float ent_bitcrasher_get_mix(const struct ent_bitcrasher *b)
{
        return b->mix;
}

void ent_bitcrasher_process(struct ent_bitcrasher *b, float **data, size_t size)
{
        (void)b;
        (void)data;
        (void)size;
}

void ent_bitcrasher_set_state(struct ent_bitcrasher *b,
                              const struct ent_state_bitcrasher *state)
{
        ENT_SET_STATE(b, state, enabled, ent_bitcrasher_enable);
        ENT_SET_STATE(b, state, bits, ent_bitcrasher_set_bits);
        ENT_SET_STATE(b, state, rate, ent_bitcrasher_set_rate);
        ENT_SET_STATE(b, state, chaos, ent_bitcrasher_set_chaos);
        ENT_SET_STATE(b, state, fold, ent_bitcrasher_set_fold);
        ENT_SET_STATE(b, state, mix, ent_bitcrasher_set_mix);
}

void ent_bitcrasher_get_state(const struct ent_bitcrasher *b,
                              struct ent_state_bitcrasher *state)
{
        ENT_GET_STATE(b, state, enabled, ent_bitcrasher_is_enabled);
        ENT_GET_STATE(b, state, bits, ent_bitcrasher_get_bits);
        ENT_GET_STATE(b, state, rate, ent_bitcrasher_get_rate);
        ENT_GET_STATE(b, state, chaos, ent_bitcrasher_get_chaos);
        ENT_GET_STATE(b, state, fold, ent_bitcrasher_get_fold);
        ENT_GET_STATE(b, state, mix, ent_bitcrasher_get_mix);
}
