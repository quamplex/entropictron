/**
 * File name: ent_crackle.h
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

#ifndef ENT_CRACKLE_H
#define ENT_CRACKLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ent_defs.h"

enum ent_crackle_envelope {
    ENT_CRACKLE_ENV_EXPONENTIAL = 0,
    ENT_CRACKLE_ENV_LINEAR,
    ENT_CRACKLE_ENV_TRIANGLE,
    ENT_CRACKLE_ENV_NUM_TYPES
};

struct ent_crackle;

struct ent_crackle* ent_crackle_create(int sample_rate);

void ent_crackle_free(struct ent_crackle **c);

enum ent_error ent_crackle_enable(struct ent_crackle *c, bool b);

bool ent_crackle_is_enabled(struct ent_crackle *c);

enum ent_error ent_crackle_set_rate(struct ent_crackle *c, float rate);

float ent_crackle_get_rate(struct ent_crackle *c);

enum ent_error ent_crackle_set_duration(struct ent_crackle *c, float duration);

float ent_crackle_get_duration(struct ent_crackle *c);

enum ent_error ent_crackle_set_amplitude(struct ent_crackle *c, float amplitude);

float ent_crackle_get_amplitude(struct ent_crackle *c);

enum ent_error ent_crackle_set_randomness(struct ent_crackle *c, float randomness);

float ent_crackle_get_randomness(struct ent_crackle *c);

enum ent_error ent_crackle_set_brightness(struct ent_crackle *c, float brightness);

float ent_crackle_get_brightness(struct ent_crackle *c);

enum ent_error ent_crackle_set_envelope_shape(struct ent_crackle *c,
                                              enum ent_crackle_envelope shape);

enum ent_crackle_envelope ent_crackle_get_envelope_shape(struct ent_crackle *c);

enum ent_error ent_crackle_set_stereo_spread(struct ent_crackle *c, float spread);

float ent_crackle_get_stereo_spread(struct ent_crackle *c);

void ent_crackle_process(struct ent_crackle *c, float **data, size_t size);

#ifdef __cplusplus
}
#endif

#endif // ENT_CRACKLE_H

