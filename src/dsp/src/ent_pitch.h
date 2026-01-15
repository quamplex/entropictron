/**
 * File name: ent_pitch.h
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

#ifndef ENT_PITCH_H
#define ENT_PITCH_H

#include "ent_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ENT_PITCH_DEFAULT_PITCH 0.0f
#define ENT_PITCH_DEFAULT_FINE 0.0f
#define ENT_PITCH_DEFAULT_DEPTH 0.1f
#define ENT_PITCH_DEFAULT_DRIFT 0.001f

#define ENT_PITCH_PITCH_MIN -3.0f
#define ENT_PITCH_PITCH_MAX 3.0f
#define ENT_PITCH_FINE_MIN -30.0f
#define ENT_PITCH_FINE_MAX 30.0f
#define ENT_PITCH_DEPTH_MIN 0.0f
#define ENT_PITCH_DEPTH_MAX 15.0f
#define ENT_PITCH_DRIFT_MIN 0.001f
#define ENT_PITCH_DRIFT_MAX 0.1f

struct ent_pitch;
struct ent_state_pitch;

struct ent_pitch* ent_pitch_create(int sample_rate);
void ent_pitch_free(struct ent_pitch **m);

void ent_pitch_enable(struct ent_pitch *m, bool b);
bool ent_pitch_is_enabled(const struct ent_pitch *m);

void ent_pitch_set_pitch(struct ent_pitch *m, float semitones);
float ent_pitch_get_pitch(const struct ent_pitch *m);

void ent_pitch_set_fine(struct ent_pitch *m, float cents);
float ent_pitch_get_fine(const struct ent_pitch *m);

void ent_pitch_set_depth(struct ent_pitch *m, float depth);
float ent_pitch_get_depth(const struct ent_pitch *m);

void ent_pitch_set_drift(struct ent_pitch *m, float drift);
float ent_pitch_get_drift(const struct ent_pitch *m);

void ent_pitch_set_entropy(struct ent_pitch *m, float entropy);
float ent_pitch_get_entropy(const struct ent_pitch *m);

void ent_pitch_process(struct ent_pitch *m,
                       float **in,
                       float **out,
                       size_t size);
#ifdef __cplusplus
}
#endif
#endif // ENT_PITCH_H
