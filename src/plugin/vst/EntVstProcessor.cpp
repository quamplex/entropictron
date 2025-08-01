/**
 * File name: EntVstProcessor.cpp
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

#include "EntVstProcessor.h"
#include "EntVstPluginView.h"
#include "VstIds.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstevents.h"

#ifndef ENTROPICTRON_OS_WINDOWS
bool ModuleEntry (void*)
{
        return true;
}

bool ModuleExit (void)
{
        return true;
}
#endif // ENTROPICTRON_OS_WINDOWS

EntVstProcessor::EntVstProcessor()
{
}

EntVstProcessor::~EntVstProcessor()
{
}

FUnknown* EntVstProcessor::createInstance(void*)
{
        return static_cast<IAudioProcessor*>(new EntVstProcessor());
}

tresult PLUGIN_API
EntVstProcessor::initialize(FUnknown* context)
{
        auto res = AudioEffect::initialize(context);
        if (res != kResultTrue)
                return res;

        setControllerClass(EntVstControllerUID);

        addAudioOutput(reinterpret_cast<const TChar*>(u"Stereo Out"),
                       SpeakerArr::kStereo);
        /*parameters.addParameter(STR16("Noise Type"),
                                nullptr,
                                2,
                                1.0,
                                ParameterInfo::kCanAutomate,
                                1000);
        parameters.addParameter(STR16("Gain"),
                                nullptr,
                                0,
                                1.0,
                                ParameterInfo::kCanAutomate,
                                1001);
        parameters.addParameter(STR16("Brightness"),
                                nullptr,
                                0,
                                1.0,
                                ParameterInfo::kCanAutomate,
                                1002);
        parameters.addParameter(STR16("Gain"),
                                nullptr,
                                0,
                                1.0,
                                ParameterInfo::kCanAutomate,
                                1002);*/

        return kResultTrue;
}

tresult PLUGIN_API
EntVstProcessor::setBusArrangements(SpeakerArrangement* inputs,
                                    int32 numIns,
                                    SpeakerArrangement* outputs,
                                    int32 numOuts)
{
        return AudioEffect::setBusArrangements(inputs,
                                                              numIns,
                                                              outputs,
                                                              numOuts);
}

tresult PLUGIN_API
EntVstProcessor::setupProcessing(ProcessSetup& setup)
{
        return AudioEffect::setupProcessing(setup);
}

tresult PLUGIN_API
EntVstProcessor::setActive(TBool state)
{
        return AudioEffect::setActive(state);
}

tresult PLUGIN_API
EntVstProcessor::process(ProcessData& data)
{
        return kResultOk;
}

tresult PLUGIN_API
EntVstProcessor::setState(IBStream* state)
{
        return kResultOk;
}

tresult PLUGIN_API
EntVstProcessor::getState(IBStream* state)
{
        return kResultOk;
}
