/**
 * File name: ent_glitch.h
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

#ifndef ENT_GLITCH_H
#define ENT_GLITCH_H

#include "ent_defs.h"
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ENT_GLITCH_MIN_REPEATS  1
#define ENT_GLITCH_MAX_REPEATS  6

#define ENT_GLITCH_MIN_PROB     0.05f
#define ENT_GLITCH_MAX_PROB     1.0f

#define ENT_GLITCH_MIN_LENGH    5.0f   // ms
#define ENT_GLITCH_MAX_LENGH    250.0f  // ms

#define ENT_GLITCH_MIN_MIN_JUMP 0.0f    // ms
#define ENT_GLITCH_MAX_MIN_JUMP 100.0f   // ms

#define ENT_GLITCH_MIN_MAX_JUMP ENT_GLITCH_MAX_MIN_JUMP
#define ENT_GLITCH_MAX_MAX_JUMP 1000.0f  // ms

#define ENT_GLITCH_DEFAULT_REPEATS  3
#define ENT_GLITCH_DEFAULT_PROB     0.25f
#define ENT_GLITCH_DEFAULT_LENGH    50.0f   // ms
#define ENT_GLITCH_DEFAULT_MIN_JUMP 0.0f    // ms
#define ENT_GLITCH_DEFAULT_MAX_JUMP 50.0f  // ms

struct ent_glitch;
struct ent_state_glitch;

struct ent_glitch* ent_glitch_create(int sample_rate);

void ent_glitch_free(struct ent_glitch **g);

enum ent_error ent_glitch_enable(struct ent_glitch *g, bool b);

bool ent_glitch_is_enabled(struct ent_glitch *g);

enum ent_error ent_glitch_set_probability(struct ent_glitch *g, float probability);

float ent_glitch_get_probability(struct ent_glitch *g);

enum ent_error ent_glitch_set_jump_min(struct ent_glitch *g, float jump_min_ms);

float ent_glitch_get_jump_min(struct ent_glitch *g);

enum ent_error ent_glitch_set_jump_max(struct ent_glitch *g, float jump_max_ms);

float ent_glitch_get_jump_max(struct ent_glitch *g);

enum ent_error ent_glitch_set_length(struct ent_glitch *g, float length_ms);

float ent_glitch_get_length(struct ent_glitch *g);

enum ent_error ent_glitch_set_repeat_count(struct ent_glitch *g, int repeats);

int ent_glitch_get_repeat_count(struct ent_glitch *g);

void ent_glitch_process(struct ent_glitch *g,
                        float **in,
                        float **out,
                        size_t size);

void ent_glitch_set_state(struct ent_glitch *g, const struct ent_state_glitch *state);

void ent_glitch_get_state(const struct ent_glitch *g, struct ent_state_glitch *state);

#ifdef __cplusplus
}
#endif

#endif // ENT_GLITCH_H
