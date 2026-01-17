/**
 * File name: entropictron.h
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

#ifndef ENT_H
#define ENT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __FAST_MATH__
#error -ffast-math disables nan detection needed by entropictron
#endif

#ifdef __STDC_NO_ATOMICS__
#error atomic operations are not supported
#endif

#include "ent_defs.h"
#include "ent_version.h"
#include "ent_log.h"

struct ent_noise;
struct ent_crackle;
struct ent_glitch;
struct entropictron;
struct ent_state;

#define ENT_DEFAULT_ENTROPY_RATE 0.0f
#define ENT_ENTROPY_RATE_MIN 0.0f
#define ENT_ENTROPY_RATE_MAX 1.0f

#define ENT_DEFAULT_ENTROPY_DEPTH 0.5f
#define ENT_ENTROPY_DEPTH_MIN 0.0f
#define ENT_ENTROPY_DEPTH_MAX 1.0f


enum ent_error ent_create(struct entropictron **ent, unsigned int sample_rate);

void ent_free(struct entropictron **ent);

enum ent_error ent_set_sample_rate(struct entropictron *ent, unsigned int rate);

enum ent_error ent_get_sample_rate(const struct entropictron *ent, unsigned int *sample_rate);

enum ent_error ent_set_play_mode(struct entropictron *ent, enum ent_play_mode mode);

enum ent_play_mode ent_get_play_mode(const struct entropictron *ent);

void ent_set_entropy_rate(struct entropictron *ent, float rate);

float ent_get_entropy_rate(const struct entropictron *ent);

void ent_set_entropy_depth(struct entropictron *ent, float depth);

float ent_get_entropy_depth(const struct entropictron *ent);

float ent_get_entropy(struct entropictron *ent);

void ent_update_entropy(struct entropictron *ent);

enum ent_error ent_process(struct entropictron *ent, float** data, size_t size);

void ent_press_key(struct entropictron *ent, bool on, int pitch, int velocity);

struct ent_noise* ent_get_noise(struct entropictron *ent, int id);

struct ent_crackle* ent_get_crackle(struct entropictron *ent, int id);

struct ent_glitch* ent_get_glitch(struct entropictron *ent, int id);

void ent_set_state(struct entropictron *ent, const struct ent_state *state);

void ent_get_state(const struct entropictron *ent, struct ent_state *state);

#ifdef __cplusplus
}
#endif
#endif // ENTROPICTRON_H
