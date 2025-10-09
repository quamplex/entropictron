/**
 * File name: ent_glitcher.h
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

#ifndef ENT_GLITCHER_H
#define ENT_GLITCHER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ent_defs.h"
#include <stdbool.h>
#include <stddef.h>

struct ent_glitcher;

struct ent_glitcher* ent_glitcher_create(int sample_rate);
void ent_glitcher_free(struct ent_glitcher **g);
enum ent_error ent_glitcher_enable(struct ent_glitcher *g, bool b);
bool ent_glitcher_is_enabled(struct ent_glitcher *g);
enum ent_error ent_glitcher_set_probability(struct ent_glitcher *g, float probability);
float ent_glitcher_get_probability(struct ent_glitcher *g);
enum ent_error ent_glitcher_set_jump_min(struct ent_glitcher *g, float jump_min_ms);
float ent_glitcher_get_jump_min(struct ent_glitcher *g);
enum ent_error ent_glitcher_set_jump_max(struct ent_glitcher *g, float jump_max_ms);
float ent_glitcher_get_jump_max(struct ent_glitcher *g);
enum ent_error ent_glitcher_set_length(struct ent_glitcher *g, float length_ms);
float ent_glitcher_get_length(struct ent_glitcher *g);
enum ent_error ent_glitcher_set_repeat_count(struct ent_glitcher *g, int repeats);
int ent_glitcher_get_repeat_count(struct ent_glitcher *g);
void ent_glitcher_process(struct ent_glitcher *g,
                          float **in,
                          float **out,
                          size_t size);

#ifdef __cplusplus
}
#endif

#endif // ENT_GLITCHER_H
