/**
 * File name: ent_chaos.h
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

#ifndef ENT_CHAOS_H
#define ENT_CHAOS_H

#include "ent_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ent_chaos {
        float phase;
        float frequency;
        float sample_rate;
        float x;
        float y;
        int type; // selects generator
};

struct ent_noise;
struct ent_state_noise;

struct ent_noise* ent_noise_create(int sample_rate);

void ent_noise_free(struct ent_noise **noise);

enum ent_error
ent_noise_enable(struct ent_noise *noise, bool b);

bool ent_noise_is_enabled(const struct ent_noise *noise);

void ent_noise_process(struct ent_noise *noise,
                       float **data,
                       size_t size);

#ifdef __cplusplus
}
#endif
#endif // ENT_CHAOS_H
