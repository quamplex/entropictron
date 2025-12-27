/**
 * File name: ent_state.c
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

#include "ent_state_internal.h"

struct ent_state* ent_state_create()
{
        return calloc(1, sizeof(struct ent_state));
}

void ent_state_free(struct ent_state* state)
{
        free(state);
}

struct ent_state_noise*
ent_state_get_noise(struct ent_state *state, size_t index)
{
        return &state->noises[index];
}

struct ent_state_crackle*
ent_state_get_crackle(struct ent_state *state, size_t index)
{
        return &state->crackles[index];
}

struct ent_state_glitch*
ent_state_get_glitch(struct ent_state *state, size_t index)
{
        return &state->glitches[index];
}

const struct ent_state_noise*
ent_state_get_noise_const(const struct ent_state *state, size_t index)
{
        return &state->noises[index];
}

const struct ent_state_crackle*
ent_state_get_crackle_const(const struct ent_state *state, size_t index)
{
        return &state->crackles[index];
}

const struct ent_state_glitch*
ent_state_get_glitch_const(const struct ent_state *state, size_t index)
{
        return &state->glitches[index];
}

/* PLAY MODE */
void ent_state_set_play_mode(struct ent_state *state, int play_mode)
{
    atomic_store_explicit(&state->play_mode,
                          (enum ent_play_mode)play_mode,
                          memory_order_release);
}

int ent_state_get_play_mode(const struct ent_state *state)
{
    return (int)atomic_load_explicit(&state->play_mode, memory_order_acquire);
}

/* NOISE */
void ent_state_noise_set_enabled(struct ent_state_noise *n, bool enabled)
{
    atomic_store_explicit(&n->enabled, enabled, memory_order_relaxed);
}

bool ent_state_noise_get_enabled(const struct ent_state_noise *n)
{
    return atomic_load_explicit(&n->enabled, memory_order_relaxed);
}

void ent_state_noise_set_type(struct ent_state_noise *n, int type)
{
    atomic_store_explicit(&n->type, (enum ent_noise_type)type, memory_order_relaxed);
}

int ent_state_noise_get_type(const struct ent_state_noise *n)
{
    return (int)atomic_load_explicit(&n->type, memory_order_relaxed);
}

void ent_state_noise_set_density(struct ent_state_noise *n, float density)
{
    atomic_store_explicit(&n->density, density, memory_order_relaxed);
}

float ent_state_noise_get_density(const struct ent_state_noise *n)
{
    return atomic_load_explicit(&n->density, memory_order_relaxed);
}

void ent_state_noise_set_brightness(struct ent_state_noise *n, float brightness)
{
    atomic_store_explicit(&n->brightness, brightness, memory_order_relaxed);
}

float ent_state_noise_get_brightness(const struct ent_state_noise *n)
{
    return atomic_load_explicit(&n->brightness, memory_order_relaxed);
}

void ent_state_noise_set_gain(struct ent_state_noise *n, float gain)
{
    atomic_store_explicit(&n->gain, gain, memory_order_relaxed);
}

float ent_state_noise_get_gain(const struct ent_state_noise *n)
{
    return atomic_load_explicit(&n->gain, memory_order_relaxed);
}

void ent_state_noise_set_stereo(struct ent_state_noise *n, float stereo)
{
    atomic_store_explicit(&n->stereo, stereo, memory_order_relaxed);
}

float ent_state_noise_get_stereo(const struct ent_state_noise *n)
{
    return atomic_load_explicit(&n->stereo, memory_order_relaxed);
}

void ent_state_noise_set_filter_type(struct ent_state_noise *n, int filter_type)
{
    atomic_store_explicit(&n->filter_type, (enum ent_filter_type)filter_type, memory_order_relaxed);
}

int ent_state_noise_get_filter_type(const struct ent_state_noise *n)
{
    return (int)atomic_load_explicit(&n->filter_type, memory_order_relaxed);
}

void ent_state_noise_set_cutoff(struct ent_state_noise *n, float cutoff)
{
    atomic_store_explicit(&n->cutoff, cutoff, memory_order_relaxed);
}

float ent_state_noise_get_cutoff(const struct ent_state_noise *n)
{
    return atomic_load_explicit(&n->cutoff, memory_order_relaxed);
}

void ent_state_noise_set_resonance(struct ent_state_noise *n, float resonance)
{
    atomic_store_explicit(&n->resonance, resonance, memory_order_relaxed);
}

float ent_state_noise_get_resonance(const struct ent_state_noise *n)
{
    return atomic_load_explicit(&n->resonance, memory_order_relaxed);
}

/* CRACKLE */
void ent_state_crackle_set_enabled(struct ent_state_crackle *c, bool enabled)
{
    atomic_store_explicit(&c->enabled, enabled, memory_order_relaxed);
}

bool ent_state_crackle_get_enabled(const struct ent_state_crackle *c)
{
    return atomic_load_explicit(&c->enabled, memory_order_relaxed);
}

void ent_state_crackle_set_rate(struct ent_state_crackle *c, float rate)
{
    atomic_store_explicit(&c->rate, rate, memory_order_relaxed);
}

float ent_state_crackle_get_rate(const struct ent_state_crackle *c)
{
    return atomic_load_explicit(&c->rate, memory_order_relaxed);
}

void ent_state_crackle_set_duration(struct ent_state_crackle *c, float duration)
{
    atomic_store_explicit(&c->duration, duration, memory_order_relaxed);
}

float ent_state_crackle_get_duration(const struct ent_state_crackle *c)
{
    return atomic_load_explicit(&c->duration, memory_order_relaxed);
}

void ent_state_crackle_set_amplitude(struct ent_state_crackle *c, float amplitude)
{
    atomic_store_explicit(&c->amplitude, amplitude, memory_order_relaxed);
}

float ent_state_crackle_get_amplitude(const struct ent_state_crackle *c)
{
    return atomic_load_explicit(&c->amplitude, memory_order_relaxed);
}

void ent_state_crackle_set_randomness(struct ent_state_crackle *c, float randomness)
{
    atomic_store_explicit(&c->randomness, randomness, memory_order_relaxed);
}

float ent_state_crackle_get_randomness(const struct ent_state_crackle *c)
{
    return atomic_load_explicit(&c->randomness, memory_order_relaxed);
}

void ent_state_crackle_set_brightness(struct ent_state_crackle *c, float brightness)
{
    atomic_store_explicit(&c->brightness, brightness, memory_order_relaxed);
}

float ent_state_crackle_get_brightness(const struct ent_state_crackle *c)
{
    return atomic_load_explicit(&c->brightness, memory_order_relaxed);
}

void ent_state_crackle_set_envelope_shape(struct ent_state_crackle *c, int shape)
{
    atomic_store_explicit(&c->envelope_shape, (enum ent_crackle_envelope)shape, memory_order_relaxed);
}

int ent_state_crackle_get_envelope_shape(const struct ent_state_crackle *c)
{
    return (int)atomic_load_explicit(&c->envelope_shape, memory_order_relaxed);
}

void ent_state_crackle_set_stereo_spread(struct ent_state_crackle *c, float spread)
{
    atomic_store_explicit(&c->stereo_spread, spread, memory_order_relaxed);
}

float ent_state_crackle_get_stereo_spread(const struct ent_state_crackle *c)
{
    return atomic_load_explicit(&c->stereo_spread, memory_order_relaxed);
}

/* GLITCH */
void ent_state_glitch_set_enabled(struct ent_state_glitch *g, bool enabled)
{
    atomic_store_explicit(&g->enabled, enabled, memory_order_relaxed);
}

bool ent_state_glitch_get_enabled(const struct ent_state_glitch *g)
{
    return atomic_load_explicit(&g->enabled, memory_order_relaxed);
}

void ent_state_glitch_set_probability(struct ent_state_glitch *g, float probability)
{
    atomic_store_explicit(&g->probability, probability, memory_order_relaxed);
}

float ent_state_glitch_get_probability(const struct ent_state_glitch *g)
{
    return atomic_load_explicit(&g->probability, memory_order_relaxed);
}

void ent_state_glitch_set_min_jump(struct ent_state_glitch *g, float min_jump)
{
    atomic_store_explicit(&g->min_jump, min_jump, memory_order_relaxed);
}

float ent_state_glitch_get_min_jump(const struct ent_state_glitch *g)
{
    return atomic_load_explicit(&g->min_jump, memory_order_relaxed);
}

void ent_state_glitch_set_max_jump(struct ent_state_glitch *g, float max_jump)
{
    atomic_store_explicit(&g->max_jump, max_jump, memory_order_relaxed);
}

float ent_state_glitch_get_max_jump(const struct ent_state_glitch *g)
{
    return atomic_load_explicit(&g->max_jump, memory_order_relaxed);
}

void ent_state_glitch_set_length(struct ent_state_glitch *g, float length)
{
    atomic_store_explicit(&g->length, length, memory_order_relaxed);
}

float ent_state_glitch_get_length(const struct ent_state_glitch *g)
{
    return atomic_load_explicit(&g->length, memory_order_relaxed);
}

void ent_state_glitch_set_repeats(struct ent_state_glitch *g, int repeats)
{
    atomic_store_explicit(&g->repeats, repeats, memory_order_relaxed);
}

int ent_state_glitch_get_repeats(const struct ent_state_glitch *g)
{
    return (int)atomic_load_explicit(&g->repeats, memory_order_relaxed);
}
