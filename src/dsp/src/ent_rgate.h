/**
 * File name: ent_rgate.h
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

#ifndef ENT_RGATE_H
#define ENT_RGATE_H

#include "ent_defs.h"
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ENT_RGATE_MIN_MIN_INTERVAL       10.0f
#define ENT_RGATE_MAX_MIN_INTERVAL       100.0f
#define ENT_RGATE_MIN_MAX_INTERVAL       100.0f
#define ENT_RGATE_MAX_MAX_INTERVAL       2000.0f
#define ENT_RGATE_DEFAULT_MIN_INTERVAL   20.0f
#define ENT_RGATE_DEFAULT_MAX_INTERVAL   200.0f

#define ENT_RGATE_MIN_MIN_DURATION       10.0f
#define ENT_RGATE_MAX_MIN_DURATION       100.0f
#define ENT_RGATE_MIN_MAX_DURATION       100.0f
#define ENT_RGATE_MAX_MAX_DURATION       1000.0f
#define ENT_RGATE_DEFAULT_MIN_DURATION   5.0f
#define ENT_RGATE_DEFAULT_MAX_DURATION   50.0f

#define ENT_RGATE_MIN_MIN_GAIN            0.0f
#define ENT_RGATE_MAX_MIN_GAIN            1.0f
#define ENT_RGATE_MIN_MAX_GAIN            0.0f
#define ENT_RGATE_MAX_MAX_GAIN            1.0f
#define ENT_RGATE_DEFAULT_MIN_GAIN        0.0f
#define ENT_RGATE_DEFAULT_MAX_GAIN        1.0f

#define ENT_RGATE_MIN_RANDOMNESS          0.0f
#define ENT_RGATE_MAX_RANDOMNESS          1.0f
#define ENT_RGATE_DEFAULT_RANDOMNESS      0.5f

#define ENT_RGATE_DEFAULT_INVERTED        false

#define ENT_RGATE_MIN_DRYWET              0.0f
#define ENT_RGATE_MAX_DRYWET              1.0f
#define ENT_RGATE_DEFAULT_DRYWET          0.5f

struct ent_rgate;
struct ent_state_rgate;

struct ent_rgate*
ent_rgate_create(int sample_rate);

void ent_rgate_free(struct ent_rgate **g);

enum ent_error
ent_rgate_enable(struct ent_rgate *g, bool enable);

bool ent_rgate_is_enabled(const struct ent_rgate *g);

enum ent_error
ent_rgate_set_min_interval(struct ent_rgate *g, float val);

float ent_rgate_get_min_interval(const struct ent_rgate *g);

enum ent_error
ent_rgate_set_max_interval(struct ent_rgate *g, float val);

float ent_rgate_get_max_interval(const struct ent_rgate *g);

enum ent_error
ent_rgate_set_min_duration(struct ent_rgate *g, float val);

float ent_rgate_get_min_duration(const struct ent_rgate *g);

enum ent_error
ent_rgate_set_max_duration(struct ent_rgate *g, float val);

float ent_rgate_get_max_duration(const struct ent_rgate *g);

enum ent_error
ent_rgate_set_min_gain(struct ent_rgate *g, float val);

float ent_rgate_get_min_gain(const struct ent_rgate *g);

enum ent_error
ent_rgate_set_max_gain(struct ent_rgate *g, float val);

float ent_rgate_get_max_gain(const struct ent_rgate *g);

enum ent_error
ent_rgate_set_randomness(struct ent_rgate *g, float val);

float ent_rgate_get_randomness(const struct ent_rgate *g);

enum ent_error
ent_rgate_set_inverted(struct ent_rgate *g, bool b);

bool ent_rgate_get_inverted(const struct ent_rgate *g);

enum ent_error ent_rgate_set_drywet(struct ent_rgate *g, float val);

float ent_rgate_get_drywet(const struct ent_rgate *g);

void ent_rgate_process(struct ent_rgate *g,
                        float **in,
                        float **out,
                        size_t size);

void ent_rgate_set_state(struct ent_rgate *g,
                         const struct ent_state_rgate *state);

void ent_rgate_get_state(const struct ent_rgate *g,
                         struct ent_state_rgate *state);

#ifdef __cplusplus
}
#endif

#endif // ENT_RGATE_H
