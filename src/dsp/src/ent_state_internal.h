/**
 * File name: ent_state_internal.h
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

#ifndef ENT_DSP_STATE_INTERNAL_H
#define ENT_DSP_STATE_INTERNAL_H

#include "ent_state.h"

#include <stdatomic.h>

struct ent_state_noise {
        _Atomic(bool) enabled;
        _Atomic(enum ent_noise_type) type;
        _Atomic(float) density;
        _Atomic(float) brightness;
        _Atomic(float) gain;
        _Atomic(float) stereo;
        _Atomic(enum ent_filter_type) filter_type;
        _Atomic(float) cutoff;
        _Atomic(float) resonance;
};

struct ent_state_crackle {
        _Atomic(bool) enabled;
        _Atomic(float) rate;
        _Atomic(float) duration;
        _Atomic(float) amplitude;
        _Atomic(float) randomness;
        _Atomic(float) brightness;
        _Atomic(enum ent_crackle_envelope) envelope_shape;
        _Atomic(float) stereo_spread;
};

struct ent_state_glitch {
        _Atomic(bool) enabled;
        _Atomic(float) probability;
        _Atomic(int) min_jump;
        _Atomic(int) max_jump;
        _Atomic(int) length;
        _Atomic(int) repeats;
};

struct ent_state_pitch {
        _Atomic(bool) enabled;
        _Atomic(float) pitch;
        _Atomic(float) fine;
        _Atomic(float) depth;
        _Atomic(float) drift;
};

struct ent_state {
        _Atomic(enum ent_play_mode) play_mode;
        _Atomic(float) entropy_rate;
        _Atomic(float) entropy_depth;
        struct ent_state_noise noises[2];
        struct ent_state_crackle crackles[2];
        struct ent_state_glitch glitches[2];
        struct ent_state_glitch pitch;
};

#define ENT_SET_STATE(obj, state, field, setter)                        \
        setter(obj, atomic_load_explicit(&state->field, memory_order_relaxed))

#define ENT_GET_STATE(obj, state, field, getter)                        \
        atomic_store_explicit(&state->field, getter(obj), memory_order_relaxed)

#endif // ENT_DSP_STATE_INTERNAL_H
