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

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

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
struct ent_glitcher;
struct entropictron;

enum ent_error ent_create(struct entropictron **ent, unsigned int sample_rate);

void ent_free(struct entropictron **ent);

enum ent_error ent_set_sample_rate(struct entropictron *ent, unsigned int rate);

enum ent_error ent_get_sample_rate(struct entropictron *ent, unsigned int *sample_rate);

enum ent_error ent_process(struct entropictron *ent, float** data, size_t size);

struct ent_noise* ent_get_noise(struct entropictron *ent, int id);

struct ent_crackle* ent_get_crackle(struct entropictron *ent);

struct ent_glitcher* ent_get_glitcher(struct entropictron *ent);

#ifdef __cplusplus
}
#endif
#endif // ENTROPICTRON_H
