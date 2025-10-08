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
    // Noise 1
    Noise1EnabledId     = 1000,
    Noise1TypeId        = 1001,
    Noise1DensityId     = 1002,
    Noise1BrightnessId  = 1003,
    Noise1GainId        = 1004,

    // Noise 2
    Noise2EnabledId     = 2000,
    Noise2TypeId        = 2001,
    Noise2DensityId     = 2002,
    Noise2BrightnessId  = 2003,
    Noise2GainId        = 2004,

    // Crackle
    CrackleEnabledId       = 20101,
    CrackleRateId          = 20102,
    CrackleDurationId      = 20103,
    CrackleAmplitudeId     = 20104,
    CrackleRandomnessId    = 20105,
    CrackleBrightnessId    = 20106,
    CrackleEnvelopeShapeId = 20107,
    CrackleStereoSpreadId  = 20108
};

} // namesapce EntVst

#endif // NT_VST_PARMETERS_H
