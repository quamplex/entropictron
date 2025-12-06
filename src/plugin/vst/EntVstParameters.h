/**
 * File name: Parameters.h
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

#ifndef ENT_VST_PARMETERS_H
#define ENT_VST_PARMETERS_H

#include "pluginterfaces/vst/vsttypes.h"

namespace EntVst {

enum ParameterId : Steinberg::Vst::ParamID
{
    // Play mode
    PlayModeId = 1000,

    // Noise 1
    Noise1EnabledId      = 10101,
    Noise1TypeId         = 10102,
    Noise1DensityId      = 10103,
    Noise1BrightnessId   = 10104,
    Noise1GainId         = 10105,
    Noise1StereoId       = 10106,
    Noise1FilterEnableId = 10107,
    Noise1FilterTypeId   = 10108,
    Noise1CutOffId       = 10109,
    Noise1ResonanceId    = 10110,

    // Noise 2
    Noise2EnabledId      = 10201,
    Noise2TypeId         = 10202,
    Noise2DensityId      = 10203,
    Noise2BrightnessId   = 10204,
    Noise2GainId         = 10205,
    Noise2StereoId       = 10206,
    Noise2FilterEnableId = 10207,
    Noise2FilterTypeId   = 10208,
    Noise2CutOffId       = 10209,
    Noise2ResonanceId    = 10210,

    // Crackle 1
    Crackle1EnabledId       = 20101,
    Crackle1RateId          = 20102,
    Crackle1DurationId      = 20103,
    Crackle1AmplitudeId     = 20104,
    Crackle1RandomnessId    = 20105,
    Crackle1BrightnessId    = 20106,
    Crackle1EnvelopeShapeId = 20107,
    Crackle1StereoSpreadId  = 20108,

    // Crackle 2
    Crackle2EnabledId       = 20201,
    Crackle2RateId          = 20202,
    Crackle2DurationId      = 20203,
    Crackle2AmplitudeId     = 20204,
    Crackle2RandomnessId    = 20205,
    Crackle2BrightnessId    = 20206,
    Crackle2EnvelopeShapeId = 20207,
    Crackle2StereoSpreadId  = 20208,


    // Glitcher 1
    Glitch1EnabledId        = 30101,
    Glitch1ProbabilityId    = 30102,
    Glitch1MinJumpId        = 30103,
    Glitch1MaxJumpId        = 30104,
    Glitch1LengthId         = 30105,
    Glitch1RepeatsId        = 30106,

    // Glitcher 2
    Glitch2EnabledId        = 30201,
    Glitch2ProbabilityId    = 30202,
    Glitch2MinJumpId        = 30203,
    Glitch2MaxJumpId        = 30204,
    Glitch2LengthId         = 30205,
    Glitch2RepeatsId        = 30206,
};

} // namesapce EntVst

#endif // NT_VST_PARMETERS_H
