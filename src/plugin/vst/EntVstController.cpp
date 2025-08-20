/**
 * File name: EntVstController.cpp
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

#include "EntVstController.h"
#include "EntVstPluginView.h"
#include "VstIds.h"
#include "EntVstParameters.h"

#include "public.sdk/source/vst/vsteditcontroller.h"

using namespace Steinberg;

FUnknown* EntVstController::createInstance(void*)
{
        return static_cast<Vst::IEditController*>(new EntVstController());
}

tresult PLUGIN_API
EntVstController::initialize(FUnknown* context)
{
    tresult result = EditControllerEx1::initialize(context);
    if (result != kResultOk)
        return result;

    // Noise 1
    parameters.addParameter(STR16("Noise 1 Enabled"),
                            nullptr, 1, 0.0,
                            0,
                            ParametersId::Noise1EnabledId);
    parameters.addParameter(STR16("Noise 1 Type"),
                            nullptr, 2, 1.0,
                            ParameterInfo::kCanAutomate,
                            ParametersId::Noise1TypeId);
    parameters.addParameter(STR16("Noise 1 Gain"),
                            nullptr, 0, 1.0,
                            ParameterInfo::kCanAutomate,
                            ParametersId::Noise1GainId);
    parameters.addParameter(STR16("Noise 1 Brightness"),
                            nullptr, 0, 1.0,
                            ParameterInfo::kCanAutomate,
                            ParametersId::Noise1BrightnessId);

    // Noise 2
    parameters.addParameter(STR16("Noise 2 Enabled"),
                            nullptr, 1, 0.0,
                            0,
                            ParametersId::Noise2EnabledId);
    parameters.addParameter(STR16("Noise 2 Type"),
                            nullptr, 2, 1.0,
                            ParameterInfo::kCanAutomate,
                            ParametersId::Noise2TypeId);
    parameters.addParameter(STR16("Noise 2 Gain"),
                            nullptr, 0, 1.0,
                            ParameterInfo::kCanAutomate,
                            ParametersId::Noise2GainId);
    parameters.addParameter(STR16("Noise 2 Brightness"),
                            nullptr, 0, 1.0,
                            ParameterInfo::kCanAutomate,
                            ParametersId::Noise2BrightnessId);

    return result;
}

IPlugView* PLUGIN_API
EntVstController::createView(Steinberg::FIDString name)
{
        if (std::string_view{name} == std::string_view{Vst::ViewType::kEditor})
                return new EntVstPluginView(this);
    return nullptr;
}

