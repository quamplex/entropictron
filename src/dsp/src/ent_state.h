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

#ifndef ENT_STATE_H
#define ENT_STATE_H

#include "ent_defs.h"
#include "ent_noise.h"
#include "ent_crackle.h"
#include "ent_glitch.h"

struct ent_state_noise {
        bool enabled;
        enum ent_noise_type type;
        float density;
        float birghtness;
        float gain;
        float stereo;
        bool filter_enabled;
        enum ent_filter_type filter_type;
        float cutoff;
        float resonance;
};

struct ent_state_crackle {
        bool enabled;
        float rate;
        float duration;
        float amplitude;
        float randomness;
        float brightness;
        enum ent_crackle_envelope envelope_shape;
        float stereo_spread;
};

struct ent_state_glitch {
        bool enabled;
        float probability;
        int jump_min_samples;
        int jump_max_samples;
        int glitch_length_samples;
        int glitch_repeat_count;
};

struct ent_state {
        enum ent_play_mode play_mode;
        struct ent_state_noise noises[2];
        struct ent_state_crackle crackles[2];
        struct ent_state_glitch glitches[2];
};

#endif // ENT_STATE_H
