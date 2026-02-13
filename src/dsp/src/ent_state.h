/**
 * File name: ent_state.h
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

#ifndef ENT_DSP_STATE_H
#define ENT_DSP_STATE_H

#include "ent_defs.h"
#include "ent_noise.h"
#include "ent_crackle.h"
#include "ent_glitch.h"
#include "ent_rgate.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ent_state_noise;
struct ent_state_crackle;
struct ent_state_glitch;
struct ent_state;

struct ent_state* ent_state_create();

void ent_state_free(struct ent_state* state);

struct ent_state_noise*
ent_state_get_noise(struct ent_state *state, size_t index);

struct ent_state_crackle*
ent_state_get_crackle(struct ent_state *state, size_t index);

struct ent_state_glitch*
ent_state_get_glitch(struct ent_state *state, size_t index);

struct ent_state_rgate*
ent_state_get_rgate(struct ent_state *state);

const struct ent_state_noise*
ent_state_get_noise_const(const struct ent_state *state, size_t index);

const struct ent_state_crackle*
ent_state_get_crackle_const(const struct ent_state *state, size_t index);

const struct ent_state_glitch*
ent_state_get_glitch_const(const struct ent_state *state, size_t index);

const struct ent_state_rgate*
ent_state_get_rgate_const(const struct ent_state *state);

void ent_state_set_play_mode(struct ent_state *state, int play_mode);
int ent_state_get_play_mode(const struct ent_state *state);

void ent_state_set_entropy_rate(struct ent_state *state, float rate);
float ent_state_get_entropy_rate(const struct ent_state *state);

void ent_state_set_entropy_depth(struct ent_state *state, float depth);
float ent_state_get_entropy_depth(const struct ent_state *state);

/* NOISE */
void ent_state_noise_set_enabled(struct ent_state_noise *n, bool enabled);
bool ent_state_noise_get_enabled(const struct ent_state_noise *n);

void ent_state_noise_set_type(struct ent_state_noise *n, int type);
int  ent_state_noise_get_type(const struct ent_state_noise *n);

void ent_state_noise_set_density(struct ent_state_noise *n, float density);
float ent_state_noise_get_density(const struct ent_state_noise *n);

void ent_state_noise_set_brightness(struct ent_state_noise *n, float brightness);
float ent_state_noise_get_brightness(const struct ent_state_noise *n);

void ent_state_noise_set_gain(struct ent_state_noise *n, float gain);
float ent_state_noise_get_gain(const struct ent_state_noise *n);

void ent_state_noise_set_stereo(struct ent_state_noise *n, float stereo);
float ent_state_noise_get_stereo(const struct ent_state_noise *n);

void ent_state_noise_set_filter_type(struct ent_state_noise *n, int filter_type);
int  ent_state_noise_get_filter_type(const struct ent_state_noise *n);

void ent_state_noise_set_cutoff(struct ent_state_noise *n, float cutoff);
float ent_state_noise_get_cutoff(const struct ent_state_noise *n);

void ent_state_noise_set_resonance(struct ent_state_noise *n, float resonance);
float ent_state_noise_get_resonance(const struct ent_state_noise *n);

/* CRACKLE */
void ent_state_crackle_set_enabled(struct ent_state_crackle *c, bool enabled);
bool ent_state_crackle_get_enabled(const struct ent_state_crackle *c);

void ent_state_crackle_set_rate(struct ent_state_crackle *c, float rate);
float ent_state_crackle_get_rate(const struct ent_state_crackle *c);

void ent_state_crackle_set_duration(struct ent_state_crackle *c, float duration);
float ent_state_crackle_get_duration(const struct ent_state_crackle *c);

void ent_state_crackle_set_amplitude(struct ent_state_crackle *c, float amplitude);
float ent_state_crackle_get_amplitude(const struct ent_state_crackle *c);

void ent_state_crackle_set_randomness(struct ent_state_crackle *c, float randomness);
float ent_state_crackle_get_randomness(const struct ent_state_crackle *c);

void ent_state_crackle_set_brightness(struct ent_state_crackle *c, float brightness);
float ent_state_crackle_get_brightness(const struct ent_state_crackle *c);

void ent_state_crackle_set_envelope_shape(struct ent_state_crackle *c, int shape);
int  ent_state_crackle_get_envelope_shape(const struct ent_state_crackle *c);

void ent_state_crackle_set_stereo_spread(struct ent_state_crackle *c, float spread);
float ent_state_crackle_get_stereo_spread(const struct ent_state_crackle *c);

/* GLITCH */
void ent_state_glitch_set_enabled(struct ent_state_glitch *g, bool enabled);
bool ent_state_glitch_get_enabled(const struct ent_state_glitch *g);

void ent_state_glitch_set_probability(struct ent_state_glitch *g, float probability);
float ent_state_glitch_get_probability(const struct ent_state_glitch *g);

void ent_state_glitch_set_min_jump(struct ent_state_glitch *g, float min_jump);
float ent_state_glitch_get_min_jump(const struct ent_state_glitch *g);

void ent_state_glitch_set_max_jump(struct ent_state_glitch *g, float max_jump);
float ent_state_glitch_get_max_jump(const struct ent_state_glitch *g);

void ent_state_glitch_set_length(struct ent_state_glitch *g, float length);
float ent_state_glitch_get_length(const struct ent_state_glitch *g);

void ent_state_glitch_set_repeats(struct ent_state_glitch *g, int repeats);
int  ent_state_glitch_get_repeats(const struct ent_state_glitch *g);

/* RGATE */
void ent_state_rgate_set_enabled(struct ent_state_rgate *g, bool val);
bool ent_state_rgate_get_enabled(const struct ent_state_rgate *g);

void ent_state_rgate_set_min_interval(struct ent_state_rgate *g, float val);
float ent_state_rgate_get_min_interval(const struct ent_state_rgate *g);
void ent_state_rgate_set_max_interval(struct ent_state_rgate *g, float val);
float ent_state_rgate_get_max_interval(const struct ent_state_rgate *g);

void ent_state_rgate_set_min_duration(struct ent_state_rgate *g, float val);
float ent_state_rgate_get_min_duration(const struct ent_state_rgate *g);
void ent_state_rgate_set_max_duration(struct ent_state_rgate *g, float val);
float ent_state_rgate_get_max_duration(const struct ent_state_rgate *g);

void ent_state_rgate_set_min_gain(struct ent_state_rgate *g, float val);
float ent_state_rgate_get_min_gain(const struct ent_state_rgate *g);
void ent_state_rgate_set_max_gain(struct ent_state_rgate *g, float val);
float ent_state_rgate_get_max_gain(const struct ent_state_rgate *g);

void ent_state_rgate_set_randomness(struct ent_state_rgate *g, float val);
float ent_state_rgate_get_randomness(const struct ent_state_rgate *g);

void ent_state_rgate_set_inverted(struct ent_state_rgate *g, bool val);
bool ent_state_rgate_get_inverted(const struct ent_state_rgate *g);

#ifdef __cplusplus
}
#endif
#endif // ENT_DSP_STATE_H
