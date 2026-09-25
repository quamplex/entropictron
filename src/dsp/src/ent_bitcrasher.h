/**
 * File name: ent_bitcrasher.h
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

#ifndef ENT_BITCRASHER_H
#define ENT_BITCRASHER_H

#include "ent_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ENT_BITCRASHER_MIN_BITS 1
#define ENT_BITCRASHER_MAX_BITS 10
#define ENT_BITCRASHER_DEFAULT_BITS 6

#define ENT_BITCRASHER_MIN_RATE 0.02f
#define ENT_BITCRASHER_MAX_RATE 1.0f
#define ENT_BITCRASHER_DEFAULT_RATE 1.0f

#define ENT_BITCRASHER_MIN_CHAOS 0.0f
#define ENT_BITCRASHER_MAX_CHAOS 0.5f
#define ENT_BITCRASHER_DEFAULT_CHAOS 0.0f

#define ENT_BITCRASHER_MIN_GAIN (-50.0f) // dB
#define ENT_BITCRASHER_MAX_GAIN (6.0f)   // dB
#define ENT_BITCRASHER_DEFAULT_GAIN 0.0f // dB

#define ENT_BITCRASHER_MIN_MIX 0.0f
#define ENT_BITCRASHER_MAX_MIX 1.0f
#define ENT_BITCRASHER_DEFAULT_MIX 1.0f

struct ent_bitcrasher;
struct ent_state_bitcrasher;

struct ent_bitcrasher* ent_bitcrasher_create(int sample_rate);

void ent_bitcrasher_free(struct ent_bitcrasher **b);

enum ent_error ent_bitcrasher_enable(struct ent_bitcrasher *b, bool enabled);

bool ent_bitcrasher_is_enabled(const struct ent_bitcrasher *b);

enum ent_error ent_bitcrasher_set_bits(struct ent_bitcrasher *b, int bits);

int ent_bitcrasher_get_bits(const struct ent_bitcrasher *b);

enum ent_error ent_bitcrasher_set_rate(struct ent_bitcrasher *b, float rate);

float ent_bitcrasher_get_rate(const struct ent_bitcrasher *b);

enum ent_error ent_bitcrasher_set_chaos(struct ent_bitcrasher *b, float chaos);

float ent_bitcrasher_get_chaos(const struct ent_bitcrasher *b);

enum ent_error ent_bitcrasher_set_gain(struct ent_bitcrasher *b, float gain);

float ent_bitcrasher_get_gain(const struct ent_bitcrasher *b);

enum ent_error ent_bitcrasher_set_mix(struct ent_bitcrasher *b, float mix);

float ent_bitcrasher_get_mix(const struct ent_bitcrasher *b);

void ent_bitcrasher_process(struct ent_bitcrasher *b,
                            float **in,
                            float **out,
                            size_t size,
                            float entropy);

void ent_bitcrasher_set_state(struct ent_bitcrasher *b,
                              const struct ent_state_bitcrasher *state);

 void ent_bitcrasher_get_state(const struct ent_bitcrasher *b,
                               struct ent_state_bitcrasher *state);

#ifdef __cplusplus
}
#endif

#endif // ENT_BITCRASHER_H
