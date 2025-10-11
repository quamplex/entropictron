 /**
 * File name: DspTypes.h
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

#ifndef ENT_DSP_TYPES_H
#define ENT_DSP_TYPES_H

#include "ent_noise.h"
#include "ent_crackle.h"

enum class NoiseId: int {
        Noise1,
        Noise2
};

enum class CrackleId: int {
        Crackle1,
        Crackle2
};

enum class GlitchId: int {
        Glitch1,
        Glitch2
};

enum class NoiseType: int {
        WhiteNoise = ENT_NOISE_TYPE_WHITE,
        PinkNoise  = ENT_NOISE_TYPE_PINK,
        BrownNoise = ENT_NOISE_TYPE_BROWN
};

enum class CrackleEnvelopeShape : int {
        Exponential = ENT_CRACKLE_ENV_EXPONENTIAL,
        Linear      = ENT_CRACKLE_ENV_LINEAR,
        Triangle    = ENT_CRACKLE_ENV_TRIANGLE
};

#endif // ENT_DSP_TYPES_H
