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
#include "DspNoiseProxyVst.h"

#include "public.sdk/source/vst/vsteditcontroller.h"

using namespace Steinberg;
using namespace EntVst;

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

        addNoiseParameters();
        addCrackleParameters();
        addGlitchParameters();

        return result;
}

void EntVstController::addNoiseParameters()
{
        parameters.addParameter(STR16("Play Mode"),
                                nullptr, 3, 0.0,
                                ParameterInfo::kIsHidden,
                                ParameterId::PlayModeId);

        // Noise 1
        parameters.addParameter(STR16("Noise 1 Enabled"),
                                nullptr, 2, 0.0,
                                ParameterInfo::kCanAutomate,
                                ParameterId::Noise1EnabledId);
        parameters.addParameter(STR16("Noise 1 Type"),
                                nullptr, 3, 0.0,
                                ParameterInfo::kCanAutomate,
                                ParameterId::Noise1TypeId);
        parameters.addParameter(STR16("Noise 1 Density"),
                                STR16("%"), 0, 1.0,
                                ParameterInfo::kCanAutomate,
                                ParameterId::Noise1DensityId);
        parameters.addParameter(STR16("Noise 1 Brightness"),
                                STR16("%"), 0, 0.0,
                                ParameterInfo::kCanAutomate,
                                ParameterId::Noise1BrightnessId);
        parameters.addParameter(STR16("Noise 1 Gain"),
                                STR16("dB"), 0,
                                // -50db...+6db, 0db
                                DspNoiseProxyVst::gainToNormalized(Entropictron::fromDecibel(0)),
                                ParameterInfo::kCanAutomate,
                                ParameterId::Noise1GainId);
        parameters.addParameter(STR16("Noise 1 Stereo"),
                                STR16("%"), 0, 1.0,
                                ParameterInfo::kCanAutomate,
                                ParameterId::Noise1StereoId);
        parameters.addParameter(STR16("Noise 1 Enable Filter"),
                                nullptr, 2, 0.0,
                                ParameterInfo::kCanAutomate,
                                ParameterId::Noise1FilterEnableId);
        parameters.addParameter(STR16("Noise 1 Filter Type"),
                                nullptr, 3, 0.0,
                                ParameterInfo::kCanAutomate,
                                ParameterId::Noise1FilterTypeId);
        parameters.addParameter(STR16("Noise 1 CutOff"),
                                STR16("Hz"),
                                0,
                                // 20 - 18000 Hz, default 800Hz
                                (800.0 - 20.0) / (18000.0 - 20.0),
                                ParameterInfo::kCanAutomate,
                                ParameterId::Noise1CutOffId);
        parameters.addParameter(STR16("Noise 1 Resonance"),
                                STR16("Q"), 0, 1.0,
                                ParameterInfo::kCanAutomate,
                                ParameterId::Noise1ResonanceId);

        // Noise 2
        parameters.addParameter(STR16("Noise 2 Enabled"),
                                nullptr, 2, 0.0,
                                ParameterInfo::kCanAutomate,
                                ParameterId::Noise2EnabledId);
        parameters.addParameter(STR16("Noise 2 Type"),
                                nullptr, 3, 0.0,
                                ParameterInfo::kCanAutomate,
                                ParameterId::Noise2TypeId);
        parameters.addParameter(STR16("Noise 2 Density"),
                                STR16("%"), 0, 1.0,
                                ParameterInfo::kCanAutomate,
                                ParameterId::Noise2DensityId);
        parameters.addParameter(STR16("Noise 2 Brightness"),
                                STR16("%"), 0, 0.0,
                                ParameterInfo::kCanAutomate,
                                ParameterId::Noise2BrightnessId);
        parameters.addParameter(STR16("Noise 2 Gain"),
                                STR16("dB"), 0,
                                // -50db...+6db, 0db
                                DspNoiseProxyVst::gainToNormalized(Entropictron::fromDecibel(0)),
                                ParameterInfo::kCanAutomate,
                                ParameterId::Noise2GainId);
        parameters.addParameter(STR16("Noise 2 Stereo"),
                                STR16("%"), 0, 1.0,
                                ParameterInfo::kCanAutomate,
                                ParameterId::Noise2StereoId);
        parameters.addParameter(STR16("Noise 2 Enable Filter"),
                                nullptr, 2, 0.0,
                                ParameterInfo::kCanAutomate,
                                ParameterId::Noise2FilterEnableId);
        parameters.addParameter(STR16("Noise 2 Filter Type"),
                                nullptr, 3, 0.0,
                                ParameterInfo::kCanAutomate,
                                ParameterId::Noise2FilterTypeId);
        parameters.addParameter(STR16("Noise 2 CutOff"),
                                STR16("Hz"), 0, 1.0,
                                ParameterInfo::kCanAutomate,
                                ParameterId::Noise2CutOffId);
        parameters.addParameter(STR16("Noise 2 Resonance"),
                                STR16("Q"), 0, 1.0,
                                ParameterInfo::kCanAutomate,
                                ParameterId::Noise2ResonanceId);
}

void EntVstController::addCrackleParameters()
{
        // Crackle1 Enabled (On/Off)
        parameters.addParameter(STR16("Crackle1 Enabled"),
                                nullptr, 2, 0.0, // 0 = off, 1 = on
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle1EnabledId);

        // Rate (bursts per second)
        auto rateDefalutNormalized = (20 - 0.5) / (150 - 0.5);
        parameters.addParameter(STR16("Crackle1 Rate"),
                                nullptr,
                                0, rateDefalutNormalized, // 0–150 Hz, default 20Hz
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle1RateId);

        // Duration (ms)
        parameters.addParameter(STR16("Crackle1 Duration"),
                                nullptr, 0,
                                (1.0 - 0.1) / (50.0 - 0.1), // 0.1–50 ms, default 1.0ms
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle1DurationId);

        // Amplitude
        parameters.addParameter(STR16("Crackle1 Amplitude"),
                                nullptr, 0, 0.5, // 0–1 linear, default 0.5
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle1AmplitudeId);

        // Randomness
        parameters.addParameter(STR16("Crackle1 Randomness"),
                                nullptr, 0, 0.5, // 1–100%, default 50%
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle1RandomnessId);

        // Brightness
        parameters.addParameter(STR16("Crackle1 Brightness"),
                                nullptr, 0, 0.5, // 0–1, default 0.5
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle1BrightnessId);

        // Envelope Shape
        parameters.addParameter(STR16("Crackle1 Envelope Shape"),
                                nullptr, 3, 0.0, // 0–1, default 0.0
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle1EnvelopeShapeId);

        // Stereo Spread
        parameters.addParameter(STR16("Crackle1 Stereo Spread"),
                                nullptr, 0, 0.0, // 0.0–1.0, default 0.0
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle1StereoSpreadId);

        // Crackle2 Enabled (On/Off)
        parameters.addParameter(STR16("Crackle2 Enabled"),
                                nullptr, 2, 0.0, // 0 = off, 1 = on
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle2EnabledId);

        // Rate (bursts per second)
        parameters.addParameter(STR16("Crackle2 Rate"),
                                nullptr, 0, rateDefalutNormalized, // 0–100 Hz, default 20
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle2RateId);

        // Duration (ms)
        parameters.addParameter(STR16("Crackle2 Duration"),
                                nullptr, 0,
                                (1.0 - 0.1) / (50.0 - 0.1), // 1–50 ms, default 20
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle2DurationId);

        // Amplitude
        parameters.addParameter(STR16("Crackle2 Amplitude"),
                                nullptr, 0, 0.5, // 0–1 linear, default 0.5
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle2AmplitudeId);

        // Randomness
        parameters.addParameter(STR16("Crackle2 Randomness"),
                                nullptr, 0, 0.5, // 1–100%, default 50%
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle2RandomnessId);

        // Brightness
        parameters.addParameter(STR16("Crackle2 Brightness"),
                                nullptr, 0, 0.5, // 0–1, default 0.5
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle2BrightnessId);

        // Envelope Shape
        parameters.addParameter(STR16("Crackle2 Envelope Shape"),
                                nullptr, 3, 0.0, // 0–1, default 0.0
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle2EnvelopeShapeId);

        // Stereo Spread
        parameters.addParameter(STR16("Crackle2 Stereo Spread"),
                                nullptr, 0, 0.0, // 0.0–1.0, default 0.0
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle2StereoSpreadId);
}

void EntVstController::addGlitchParameters()
{
    // Glitch1 Enabled (On/Off)
    parameters.addParameter(STR16("Glitch1 Enabled"),
                            nullptr, 2, 0.0, // 0 = off, 1 = on
                            ParameterInfo::kCanAutomate,
                            ParameterId::Glitch1EnabledId);

    // Glitch1 Probability
    parameters.addParameter(STR16("Glitch1 Probability"),
                            STR16("%"), 0, 0.2, // 0–100%, default 20%
                            ParameterInfo::kCanAutomate,
                            ParameterId::Glitch1ProbabilityId);

    // Min Jump Time (ms)
    parameters.addParameter(STR16("Min Jump Time"),
                            STR16("ms"), 0, 0.05, // 0–2000 ms, default 100 ms
                            ParameterInfo::kCanAutomate,
                            ParameterId::Glitch1MinJumpId);

    // Max Jump Time (ms)
    parameters.addParameter(STR16("Max Jum Time"),
                            STR16("ms"), 0, 0.2, // 0–5000 ms, default 1000 ms
                            ParameterInfo::kCanAutomate,
                            ParameterId::Glitch1MaxJumpId);

    // Glitch1 Length (ms)
    parameters.addParameter(STR16("Glitch1 Length"),
                            STR16("ms"), 0, 0.1, // 0–2000 ms, default 200 ms
                            ParameterInfo::kCanAutomate,
                            ParameterId::Glitch1LengthId);

    // Glitch1 Repeats
    parameters.addParameter(STR16("Glitch1 Repeats"),
                            nullptr, 10, 0.2, // 1–10 repeats, default 2
                            ParameterInfo::kCanAutomate,
                            ParameterId::Glitch1RepeatsId);

    // Glitch2 Enabled (On/Off)
    parameters.addParameter(STR16("Glitch2 Enabled"),
                            nullptr, 2, 0.0, // 0 = off, 1 = on
                            ParameterInfo::kCanAutomate,
                            ParameterId::Glitch2EnabledId);

    // Glitch2 Probability
    parameters.addParameter(STR16("Glitch2 Probability"),
                            STR16("%"), 0, 0.2, // 0–100%, default 20%
                            ParameterInfo::kCanAutomate,
                            ParameterId::Glitch2ProbabilityId);

    // Min Jum Time (ms)
    parameters.addParameter(STR16("Min Jump Time"),
                            STR16("ms"), 0, 0.05, // 0–2000 ms, default 100 ms
                            ParameterInfo::kCanAutomate,
                            ParameterId::Glitch2MinJumpId);

    // Max Jump Time (ms)
    parameters.addParameter(STR16("Max Jump Time"),
                            STR16("ms"), 0, 0.2, // 0–5000 ms, default 1000 ms
                            ParameterInfo::kCanAutomate,
                            ParameterId::Glitch2MaxJumpId);

    // Glitch2 Length (ms)
    parameters.addParameter(STR16("Glitch2 Length"),
                            STR16("ms"), 0, 0.1, // 0–2000 ms, default 200 ms
                            ParameterInfo::kCanAutomate,
                            ParameterId::Glitch2LengthId);

    // Glitch2 Repeats
    parameters.addParameter(STR16("Glitch2 Repeats"),
                            nullptr, 10, 0.2, // 1–10 repeats, default 2
                            ParameterInfo::kCanAutomate,
                            ParameterId::Glitch2RepeatsId);
}

IPlugView* PLUGIN_API
EntVstController::createView(Steinberg::FIDString name)
{
        if (std::string_view{name} == std::string_view{Vst::ViewType::kEditor})
                return new EntVstPluginView(this);
        return nullptr;
}

void EntVstController::setParamterCallback(ParameterId paramId,
                                           const ParameterCallback &callback)
{
        parametersCallbacks.insert({paramId, callback});
}

void EntVstController::removeParamterCallback(ParameterId id)
{
        parametersCallbacks.erase(id);
}

tresult EntVstController::setParamNormalized (ParamID tag, ParamValue value)
{
        auto result = EditControllerEx1::setParamNormalized(tag, value);
        if (result != kResultOk)
                return result;

        auto parameterId = static_cast<ParameterId>(tag);
        auto res = parametersCallbacks.find(parameterId);
        if (res != parametersCallbacks.end())
                res->second(parameterId, value);
        return result;
}
