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

namesapce EntVst {

enum ParameterId : Steinberg::Vst::ParamID
{
    // Noise 1
    Noise1EnabledId     = 1000,
    Noise1TypeId        = 1001,
    Noise1GainId        = 1002,
    Noise1BrightnessId  = 1003,

    // Noise 2
    Noise2EnabledId     = 1100,
    Noise2TypeId        = 1101,
    Noise2GainId        = 1102,
    Noise2BrightnessId  = 1103
};

using ParameterValue = std::variant<bool, int, float, double>;

} // namesapce EntVst

#endif // NT_VST_PARMETERS_H
