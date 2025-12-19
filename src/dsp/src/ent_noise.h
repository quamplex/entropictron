/**
 * File name: ent_noise.
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

#ifndef ENT_NOISE_H
#define ENT_NOISE_H

#include "ent_defs.h"
#include "ent_filter.h"

#ifdef __cplusplus
extern "C" {
#endif

enum ent_noise_type {
        ENT_NOISE_TYPE_WHITE,
        ENT_NOISE_TYPE_PINK,
        ENT_NOISE_TYPE_BROWN,
};

struct ent_noise;
struct ent_state_noise;

struct ent_noise* ent_noise_create(int sample_rate);

void ent_noise_free(struct ent_noise **noise);

enum ent_error ent_noise_enable(struct ent_noise *noise, bool b);

bool ent_noise_is_enabled(struct ent_noise *noise);

enum ent_error ent_noise_set_type(struct ent_noise *noise,
                                  enum ent_noise_type type);

enum ent_noise_type ent_noise_get_type(struct ent_noise *noise);

enum ent_error ent_noise_set_density(struct ent_noise *noise, float density);

float ent_noise_get_density(struct ent_noise *noise);

enum ent_error ent_noise_set_brightness(struct ent_noise *noise, float brightness);

float ent_noise_get_brightness(struct ent_noise *noise);

enum ent_error ent_noise_set_gain(struct ent_noise *noise, float gain);

float ent_noise_get_gain(struct ent_noise *noise);

enum ent_error ent_noise_set_stereo(struct ent_noise *noise, float stereo);

float ent_noise_get_stereo(struct ent_noise *noise);

enum ent_error
ent_noise_filter_enable(struct ent_noise *noise, bool enable);

bool ent_noise_filter_is_enabled(struct ent_noise *noise);

enum ent_error
ent_noise_set_filter_type(struct ent_noise *noise,
                          enum ent_filter_type type);

enum ent_filter_type
ent_noise_get_filter_type(struct ent_noise *noise);

enum ent_error ent_noise_set_cutoff(struct ent_noise *noise, float cutoff);

float ent_noise_get_cutoff(struct ent_noise *noise);

enum ent_error ent_noise_set_resonance(struct ent_noise *noise, float resonance);

float ent_noise_get_resonance(struct ent_noise *noise);

void ent_noise_process(struct ent_noise *noise,
                       float **data,
                       size_t size);

void ent_noise_set_state(struct ent_noise *noise, const struct ent_state_noise *state);

void ent_noise_get_state(const struct ent_noise *noise, struct ent_state_noise *state);

#ifdef __cplusplus
}
#endif
#endif // ENT_NOISE_H
