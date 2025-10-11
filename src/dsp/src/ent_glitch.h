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

#ifdef __cplusplus
extern "C" {
#endif

#include "ent_defs.h"
#include <stdbool.h>
#include <stddef.h>

struct ent_glitch;

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

#ifdef __cplusplus
}
#endif

#endif // ENT_GLITCH_H
