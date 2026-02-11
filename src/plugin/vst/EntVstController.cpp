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
#include "DspVstProxy.h"
#include "DspNoiseProxyVst.h"
#include "DspCrackleProxyVst.h"
#include "DspGlitchProxyVst.h"
#include "DspRgateProxyVst.h"
#include "EntState.h"

#include "public.sdk/source/vst/vsteditcontroller.h"
#include "pluginterfaces/base/ibstream.h"

#include <type_traits>
#include <atomic>

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

        parameters.addParameter(STR16("State Changed"),
                                nullptr,
                                1, 0, Vst::ParameterInfo::kIsReadOnly | Vst::ParameterInfo::kIsHidden,
                                ParameterId::StateChangedId);

        parameters.addParameter(STR16("Play Mode"),
                                nullptr, 3, DspProxyVst::playModeToNormalized(PlayMode::PlaybackMode),
                                ParameterInfo::kCanAutomate,
                                ParameterId::PlayModeId);

        parameters.addParameter(STR16("Entropy Rate"),
                                nullptr, 0, ENT_DEFAULT_ENTROPY_RATE,
                                ParameterInfo::kCanAutomate,
                                ParameterId::EntropyRateId);
        parameters.addParameter(STR16("Entropy Depth"),
                                nullptr, 0, ENT_DEFAULT_ENTROPY_DEPTH,
                                ParameterInfo::kCanAutomate,
                                ParameterId::EntropyDepthId);

        parameters.addParameter(STR16("Entropy Meter"),
                                nullptr, 0, DspProxyVst::entropyToNormalized(0.0),
                                Vst::ParameterInfo::kIsReadOnly | Vst::ParameterInfo::kIsHidden,
                                ParameterId::EntropyMeterId);

        addNoiseParameters();
        addCrackleParameters();
        addGlitchParameters();
        addRgateParameters();

        setParamNormalized(ParameterId::PlayModeId,
                            DspProxyVst::playModeToNormalized(PlayMode::PlaybackMode));
        setParamNormalized(ParameterId::EntropyRateId, ENT_DEFAULT_ENTROPY_RATE);
        setParamNormalized(ParameterId::EntropyDepthId, ENT_DEFAULT_ENTROPY_DEPTH);
        setParamNormalized(ParameterId::EntropyMeterId, DspProxyVst::entropyToNormalized(0.0));

        // Noise 1
        setParamNormalized(ParameterId::Noise1EnabledId, 0.0);
        setParamNormalized(ParameterId::Noise1TypeId,
                            DspNoiseProxyVst::noiseTypeToNormalized(NoiseType::WhiteNoise));
        setParamNormalized(ParameterId::Noise1DensityId, 1.0);
        setParamNormalized(ParameterId::Noise1BrightnessId, 0.0);
        setParamNormalized(ParameterId::Noise1GainId,
                            DspNoiseProxyVst::gainToNormalized(Entropictron::fromDecibel(0)));
        setParamNormalized(ParameterId::Noise1StereoId, 0.0);
        setParamNormalized(ParameterId::Noise1FilterTypeId,
                            DspNoiseProxyVst::filterTypeToNormalized(FilterType::AllPass));
        setParamNormalized(ParameterId::Noise1CutOffId,
                            DspNoiseProxyVst::cutoffToNormalized(800));
        setParamNormalized(ParameterId::Noise1ResonanceId, 1.0);

        // Noise 2
        setParamNormalized(ParameterId::Noise2EnabledId, 0.0);
        setParamNormalized(ParameterId::Noise2TypeId,
                            DspNoiseProxyVst::noiseTypeToNormalized(NoiseType::WhiteNoise));
        setParamNormalized(ParameterId::Noise2DensityId, 2.0);
        setParamNormalized(ParameterId::Noise2BrightnessId, 0.0);
        setParamNormalized(ParameterId::Noise2GainId,
                            DspNoiseProxyVst::gainToNormalized(Entropictron::fromDecibel(0)));
        setParamNormalized(ParameterId::Noise2StereoId, 0.0);
        setParamNormalized(ParameterId::Noise2FilterTypeId,
                            DspNoiseProxyVst::filterTypeToNormalized(FilterType::AllPass));
        setParamNormalized(ParameterId::Noise2CutOffId,
                            DspNoiseProxyVst::cutoffToNormalized(800));
        setParamNormalized(ParameterId::Noise2ResonanceId, 2.0);

        // Crackle 1
        setParamNormalized(ParameterId::Crackle1EnabledId, 0.0);
        setParamNormalized(ParameterId::Crackle1RateId,
                           DspCrackleProxyVst::rateToNormalized(20.0));
        setParamNormalized(ParameterId::Crackle1DurationId,
                           DspCrackleProxyVst::durationToNormalized(1.0));
        setParamNormalized(ParameterId::Crackle1AmplitudeId, 1.0);
        setParamNormalized(ParameterId::Crackle1RandomnessId, 1.0);
        setParamNormalized(ParameterId::Crackle1BrightnessId, 0.0);
        setParamNormalized(ParameterId::Crackle1EnvelopeShapeId,
                           DspCrackleProxyVst::envelopeShapeToNormalized(CrackleEnvelopeShape::Exponential));
        setParamNormalized(ParameterId::Crackle1StereoSpreadId, 0.0);

        // Crackle 2
        setParamNormalized(ParameterId::Crackle2EnabledId, 0.0);
        setParamNormalized(ParameterId::Crackle2RateId,
                           DspCrackleProxyVst::rateToNormalized(20.0));
        setParamNormalized(ParameterId::Crackle2DurationId,
                           DspCrackleProxyVst::durationToNormalized(2.0));
        setParamNormalized(ParameterId::Crackle2AmplitudeId, 2.0);
        setParamNormalized(ParameterId::Crackle2RandomnessId, 2.0);
        setParamNormalized(ParameterId::Crackle2BrightnessId, 0.0);
        setParamNormalized(ParameterId::Crackle2EnvelopeShapeId,
                           DspCrackleProxyVst::envelopeShapeToNormalized(CrackleEnvelopeShape::Exponential));
        setParamNormalized(ParameterId::Crackle2StereoSpreadId, 0.0);

        // Glitch1
        setParamNormalized(ParameterId::Glitch1EnabledId, 0);
        setParamNormalized(ParameterId::Glitch1RepeatsId,
                            DspGlitchProxyVst::repeatsToNormalized(ENT_GLITCH_DEFAULT_REPEATS));
        setParamNormalized(ParameterId::Glitch1ProbabilityId,
                            DspGlitchProxyVst::probabilityToNormalized(ENT_GLITCH_DEFAULT_PROB));
        setParamNormalized(ParameterId::Glitch1LengthId,
                            DspGlitchProxyVst::lengthToNormalized(ENT_GLITCH_DEFAULT_LENGH));
        setParamNormalized(ParameterId::Glitch1MinJumpId,
                            DspGlitchProxyVst::minJumpToNormalized(ENT_GLITCH_DEFAULT_MIN_JUMP));
        setParamNormalized(ParameterId::Glitch1MaxJumpId,
                            DspGlitchProxyVst::maxJumpToNormalized(ENT_GLITCH_DEFAULT_MAX_JUMP));

        // Glitch2
        setParamNormalized(ParameterId::Glitch2EnabledId, 0);
        setParamNormalized(ParameterId::Glitch2RepeatsId,
                            DspGlitchProxyVst::repeatsToNormalized(ENT_GLITCH_DEFAULT_REPEATS));
        setParamNormalized(ParameterId::Glitch2ProbabilityId,
                            DspGlitchProxyVst::probabilityToNormalized(ENT_GLITCH_DEFAULT_PROB));
        setParamNormalized(ParameterId::Glitch2LengthId,
                            DspGlitchProxyVst::lengthToNormalized(ENT_GLITCH_DEFAULT_LENGH));
        setParamNormalized(ParameterId::Glitch2MinJumpId,
                            DspGlitchProxyVst::minJumpToNormalized(ENT_GLITCH_DEFAULT_MIN_JUMP));
        setParamNormalized(ParameterId::Glitch2MaxJumpId,
                            DspGlitchProxyVst::maxJumpToNormalized(ENT_GLITCH_DEFAULT_MAX_JUMP));

        // Rgate
        setParamNormalized(ParameterId::RgateEnabledId,
                           DspRgateProxyVst::enabledToNormalized(false));
        setParamNormalized(ParameterId::RgateMinIntervalId,
                           DspRgateProxyVst::minIntervalToNormalized(ENT_RGATE_DEFAULT_MIN_INTERVAL));
        setParamNormalized(ParameterId::RgateMaxIntervalId,
                           DspRgateProxyVst::maxIntervalToNormalized(ENT_RGATE_DEFAULT_MAX_INTERVAL));
        setParamNormalized(ParameterId::RgateMinDurationId,
                           DspRgateProxyVst::minDurationToNormalized(ENT_RGATE_DEFAULT_MIN_DURATION));
        setParamNormalized(ParameterId::RgateMaxDurationId,
                           DspRgateProxyVst::maxDurationToNormalized(ENT_RGATE_DEFAULT_MAX_DURATION));
        setParamNormalized(ParameterId::RgateMinGainId,
                           DspRgateProxyVst::minGainToNormalized(ENT_RGATE_DEFAULT_MIN_GAIN));
        setParamNormalized(ParameterId::RgateMaxGainId,
                           DspRgateProxyVst::maxGainToNormalized(ENT_RGATE_DEFAULT_MAX_GAIN));
        setParamNormalized(ParameterId::RgateRandomnessId,
                           DspRgateProxyVst::randomnessToNormalized(ENT_RGATE_DEFAULT_RANDOMNESS));
        setParamNormalized(ParameterId::RgateInvertedId,
                           DspRgateProxyVst::invertedToNormalized(ENT_RGATE_DEFAULT_INVERTED));
        setParamNormalized(ParameterId::RgateDrywetId,
                           DspRgateProxyVst::drywetToNormalized(ENT_RGATE_DEFAULT_DRYWET));

        return result;
}

tresult PLUGIN_API EntVstController::setComponentState(IBStream* state)
{
        if (state == nullptr)
                return kInvalidArgument;

        Vst::EditControllerEx1::setComponentState(state);

        if (state->seek(0, IBStream::kIBSeekEnd, 0) == kResultFalse) {
                ENT_LOG_ERROR("can't seek in stream");
                return kResultFalse;
        }

        int64 endStream = 0;
        if (state->tell(&endStream) == kResultFalse) {
                ENT_LOG_ERROR("can't get current position in stream");
                return kResultFalse;
        } else if (endStream < 1) {
                ENT_LOG_ERROR("stream is empty");
                return kResultFalse;
        }

        if (state->seek(0, IBStream::kIBSeekSet, 0) == kResultFalse) {
                ENT_LOG_ERROR("can't seek in stream");
                return kResultFalse;
        }

        std::string data(endStream, '\0');
        int32 nBytes = 0;
        if (state->read(data.data(), data.size(), &nBytes) == kResultFalse) {
                ENT_LOG_ERROR("error on reading the state");
                return kResultFalse;
        }

        if (static_cast<decltype(nBytes)>(data.size()) != nBytes) {
                ENT_LOG_ERROR("error on reading the state");
                return kResultFalse;
        }

        EntState entState{data};

        auto playMode = static_cast<PlayMode>(entState.getPlayMode());
        setParamNormalized (ParameterId::PlayModeId,
                            DspProxyVst::playModeToNormalized(playMode));
        setParamNormalized (ParameterId::EntropyRateId,
                            entState.getEntropyRate());
        setParamNormalized (ParameterId::EntropyDepthId,
                            entState.getEntropyDepth());
        setParamNormalized (ParameterId::EntropyMeterId, DspProxyVst::entropyToNormalized(0.0));

        setNoiseState(entState);
        setCrackleState(entState);
        setGlitchState(entState);

        if (stateCallback)
                stateCallback();

        return kResultOk;
}

void EntVstController::setNoiseState(const EntState& state)
{
        {
                const auto& noise = state.noise[static_cast<size_t>(NoiseId::Noise1)];
                setParamNormalized (ParameterId::Noise1EnabledId, noise.enabled);
                setParamNormalized (ParameterId::Noise1TypeId,
                                    DspNoiseProxyVst::noiseTypeToNormalized(static_cast<NoiseType>(noise.type)));
                setParamNormalized (ParameterId::Noise1DensityId, noise.density);
                setParamNormalized (ParameterId::Noise1BrightnessId, noise.brightness);
                setParamNormalized (ParameterId::Noise1GainId,
                                    DspNoiseProxyVst::gainToNormalized(Entropictron::fromDecibel(noise.gain)));
                setParamNormalized (ParameterId::Noise1StereoId, noise.stereo);
                setParamNormalized (ParameterId::Noise1FilterTypeId,
                                    DspNoiseProxyVst::filterTypeToNormalized(static_cast<FilterType>(noise.filter_type)));
                setParamNormalized (ParameterId::Noise1CutOffId,
                                    DspNoiseProxyVst::cutoffToNormalized(noise.cutoff));
                setParamNormalized (ParameterId::Noise1ResonanceId, noise.resonance);
        }

        {
                const auto& noise = state.noise[static_cast<size_t>(NoiseId::Noise2)];
                setParamNormalized (ParameterId::Noise2EnabledId, noise.enabled);
                setParamNormalized (ParameterId::Noise2TypeId,
                                    DspNoiseProxyVst::noiseTypeToNormalized(static_cast<NoiseType>(noise.type)));
                setParamNormalized (ParameterId::Noise2DensityId, noise.density);
                setParamNormalized (ParameterId::Noise2BrightnessId, noise.brightness);
                setParamNormalized (ParameterId::Noise2GainId,
                                    DspNoiseProxyVst::gainToNormalized(Entropictron::fromDecibel(noise.gain)));
                setParamNormalized (ParameterId::Noise2StereoId, noise.stereo);
                setParamNormalized (ParameterId::Noise2FilterTypeId,
                                    DspNoiseProxyVst::filterTypeToNormalized(static_cast<FilterType>(noise.filter_type)));
                setParamNormalized (ParameterId::Noise2CutOffId,
                                    DspNoiseProxyVst::cutoffToNormalized(noise.cutoff));
                setParamNormalized (ParameterId::Noise2ResonanceId, noise.resonance);
        }
}

void EntVstController::setCrackleState(const EntState& state)
{
        {
                const auto& crackle = state.crackle[static_cast<size_t>(CrackleId::Crackle1)];
                setParamNormalized(ParameterId::Crackle1EnabledId,
                                   crackle.enabled);
                setParamNormalized(ParameterId::Crackle1RateId,
                                   DspCrackleProxyVst::rateToNormalized(crackle.rate));
                setParamNormalized(ParameterId::Crackle1DurationId,
                                   DspCrackleProxyVst::durationToNormalized(crackle.duration));
                setParamNormalized(ParameterId::Crackle1AmplitudeId, crackle.amplitude);
                setParamNormalized(ParameterId::Crackle1RandomnessId, crackle.randomness);
                setParamNormalized(ParameterId::Crackle1BrightnessId, crackle.brightness);
                auto envShape = DspCrackleProxyVst::envelopeShapeToNormalized(static_cast<CrackleEnvelopeShape>(crackle.envelope_shape));
                setParamNormalized(ParameterId::Crackle1EnvelopeShapeId, envShape);
                setParamNormalized(ParameterId::Crackle1StereoSpreadId, crackle.stereo_spread);
        }

        {
                const auto& crackle = state.crackle[static_cast<size_t>(CrackleId::Crackle2)];
                setParamNormalized(ParameterId::Crackle2EnabledId,
                                   crackle.enabled);
                setParamNormalized(ParameterId::Crackle2RateId,
                                   DspCrackleProxyVst::rateToNormalized(crackle.rate));
                setParamNormalized(ParameterId::Crackle2DurationId,
                                   DspCrackleProxyVst::durationToNormalized(crackle.duration));
                setParamNormalized(ParameterId::Crackle2AmplitudeId, crackle.amplitude);
                setParamNormalized(ParameterId::Crackle2RandomnessId, crackle.randomness);
                setParamNormalized(ParameterId::Crackle2BrightnessId, crackle.brightness);
                auto envShape = DspCrackleProxyVst::envelopeShapeToNormalized(static_cast<CrackleEnvelopeShape>(crackle.envelope_shape));
                setParamNormalized(ParameterId::Crackle2EnvelopeShapeId, envShape);
                setParamNormalized(ParameterId::Crackle2StereoSpreadId, crackle.stereo_spread);
        }
}

void EntVstController::setGlitchState(const EntState &state)
{
        {
                const auto& glitch = state.glitch[static_cast<size_t>(GlitchId::Glitch1)];
                setParamNormalized (ParameterId::Glitch1EnabledId, glitch.enabled);
                setParamNormalized (ParameterId::Glitch1RepeatsId,
                                    DspGlitchProxyVst::repeatsToNormalized(glitch.repeats));
                setParamNormalized (ParameterId::Glitch1ProbabilityId,
                                    DspGlitchProxyVst::probabilityToNormalized(glitch.probability));
                setParamNormalized (ParameterId::Glitch1LengthId,
                                    DspGlitchProxyVst::lengthToNormalized(glitch.length));
                setParamNormalized (ParameterId::Glitch1MinJumpId,
                                    DspGlitchProxyVst::minJumpToNormalized(glitch.min_jump));
                setParamNormalized (ParameterId::Glitch1MaxJumpId,
                                    DspGlitchProxyVst::maxJumpToNormalized(glitch.max_jump));
        }

        {
                const auto& glitch = state.glitch[static_cast<size_t>(GlitchId::Glitch2)];
                setParamNormalized (ParameterId::Glitch2EnabledId, glitch.enabled);
                setParamNormalized (ParameterId::Glitch2RepeatsId,
                                    DspGlitchProxyVst::repeatsToNormalized(glitch.repeats));
                setParamNormalized (ParameterId::Glitch2ProbabilityId,
                                    DspGlitchProxyVst::probabilityToNormalized(glitch.probability));
                setParamNormalized (ParameterId::Glitch2LengthId,
                                    DspGlitchProxyVst::lengthToNormalized(glitch.length));
                setParamNormalized (ParameterId::Glitch2MinJumpId,
                                    DspGlitchProxyVst::minJumpToNormalized(glitch.min_jump));
                setParamNormalized (ParameterId::Glitch2MaxJumpId,
                                    DspGlitchProxyVst::maxJumpToNormalized(glitch.max_jump));
        }
}

void EntVstController::setRgateState(const EntState &state)
{

        const auto& rgate = state.rgate;
        setParamNormalized(ParameterId::RgateEnabledId, rgate.enabled);
        setParamNormalized(ParameterId::RgateMinIntervalId,
                           DspRgateProxyVst::minIntervalToNormalized(rgate.min_interval));
        setParamNormalized(ParameterId::RgateMaxIntervalId,
                           DspRgateProxyVst::maxIntervalToNormalized(rgate.max_interval));
        setParamNormalized(ParameterId::RgateMinDurationId,
                           DspRgateProxyVst::minDurationToNormalized(rgate.min_duration));
        setParamNormalized(ParameterId::RgateMaxDurationId,
                           DspRgateProxyVst::maxDurationToNormalized(rgate.max_duration));
        setParamNormalized(ParameterId::RgateMinGainId,
                           DspRgateProxyVst::minGainToNormalized(rgate.min_gain));
        setParamNormalized(ParameterId::RgateMaxGainId,
                           DspRgateProxyVst::maxGainToNormalized(rgate.max_gain));
        setParamNormalized(ParameterId::RgateRandomnessId,
                           DspRgateProxyVst::randomnessToNormalized(rgate.randomness));
        setParamNormalized(ParameterId::RgateInvertedId,
                           DspRgateProxyVst::invertedToNormalized(rgate.inverted));
        setParamNormalized(ParameterId::RgateDrywetId,
                           DspRgateProxyVst::drywetToNormalized(rgate.drywet));
}

void EntVstController::addNoiseParameters()
{
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
        parameters.addParameter(STR16("Noise 1 Filter Type"),
                                nullptr, 4, 0.0,
                                ParameterInfo::kCanAutomate,
                                ParameterId::Noise1FilterTypeId);
        parameters.addParameter(STR16("Noise 1 CutOff"),
                                STR16("Hz"),
                                0, DspNoiseProxyVst::cutoffToNormalized(800),
                                ParameterInfo::kCanAutomate,
                                ParameterId::Noise1CutOffId);
        parameters.addParameter(STR16("Noise 1 Resonance"),
                                STR16("Q"), 0, 0.5,
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
        parameters.addParameter(STR16("Noise 2 Filter Type"),
                                nullptr, 4, 0.0,
                                ParameterInfo::kCanAutomate,
                                ParameterId::Noise2FilterTypeId);
        parameters.addParameter(STR16("Noise 2 CutOff"),
                                STR16("Hz"), 0, DspNoiseProxyVst::cutoffToNormalized(800),
                                ParameterInfo::kCanAutomate,
                                ParameterId::Noise2CutOffId);
        parameters.addParameter(STR16("Noise 2 Resonance"),
                                STR16("Q"), 0, 0.5,
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
        parameters.addParameter(STR16("Crackle1 Rate"),
                                nullptr,
                                0, DspCrackleProxyVst::rateToNormalized(20.0), // default 20Hz
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle1RateId);

        // Duration (ms)
        parameters.addParameter(STR16("Crackle1 Duration"),
                                nullptr, 0,
                                DspCrackleProxyVst::durationToNormalized(1.0),
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle1DurationId);

        // Amplitude
        parameters.addParameter(STR16("Crackle1 Amplitude"),
                                nullptr, 0, 0.5, // 0–1 linear, default 0.5
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle1AmplitudeId);

        // Randomness
        parameters.addParameter(STR16("Crackle1 Randomness"),
                                nullptr, 0, 1.0, // 1–100%, default 50%
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle1RandomnessId);

        // Brightness
        parameters.addParameter(STR16("Crackle1 Brightness"),
                                nullptr, 0, 0.5, // 0–1, default 0.5
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle1BrightnessId);

        // Envelope Shape
        parameters.addParameter(STR16("Crackle1 Envelope Shape"),
                                nullptr, 3,
                                DspCrackleProxyVst::envelopeShapeToNormalized(CrackleEnvelopeShape::Exponential),
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

        parameters.addParameter(STR16("Crackle2 Rate"),
                                nullptr, 0, DspCrackleProxyVst::rateToNormalized(20),
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle2RateId);

        // Duration (ms)
        parameters.addParameter(STR16("Crackle2 Duration"),
                                nullptr, 0,
                                DspCrackleProxyVst::durationToNormalized(1.0),
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle2DurationId);

        // Amplitude
        parameters.addParameter(STR16("Crackle2 Amplitude"),
                                nullptr, 0, 0.5, // 0–1 linear, default 0.5
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle2AmplitudeId);

        // Randomness
        parameters.addParameter(STR16("Crackle2 Randomness"),
                                nullptr, 0, 1.0, // 1–100%, default 50%
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle2RandomnessId);

        // Brightness
        parameters.addParameter(STR16("Crackle2 Brightness"),
                                nullptr, 0, 0.0, // 0–1, default 0.0
                                ParameterInfo::kCanAutomate,
                                ParameterId::Crackle2BrightnessId);

        // Envelope Shape
        parameters.addParameter(STR16("Crackle2 Envelope Shape"),
                                nullptr, 3,
                                DspCrackleProxyVst::envelopeShapeToNormalized(CrackleEnvelopeShape::Exponential),
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
                                nullptr, 2, 0.0,
                                ParameterInfo::kCanAutomate,
                                ParameterId::Glitch1EnabledId);

        // Glitch1 Repeats
        parameters.addParameter(STR16("Glitch1 Repeats"),
                                nullptr, 10,
                                DspGlitchProxyVst::repeatsToNormalized(ENT_GLITCH_DEFAULT_REPEATS),
                                ParameterInfo::kCanAutomate,
                                ParameterId::Glitch1RepeatsId);

        // Glitch1 Probability
        parameters.addParameter(STR16("Glitch1 Probability"),
                                STR16("%"), 0,
                                DspGlitchProxyVst::probabilityToNormalized(ENT_GLITCH_DEFAULT_PROB),
                                ParameterInfo::kCanAutomate,
                                ParameterId::Glitch1ProbabilityId);

        // Glitch1 Length (ms)
        parameters.addParameter(STR16("Glitch1 Length"),
                                STR16("ms"), 0,
                                DspGlitchProxyVst::lengthToNormalized(ENT_GLITCH_DEFAULT_LENGH),
                                ParameterInfo::kCanAutomate,
                                ParameterId::Glitch1LengthId);

        // Glitch1 Min Jump Time (ms)
        parameters.addParameter(STR16("Min Jump Time"),
                                STR16("ms"), 0,
                                DspGlitchProxyVst::minJumpToNormalized(ENT_GLITCH_DEFAULT_MIN_JUMP),
                                ParameterInfo::kCanAutomate,
                                ParameterId::Glitch1MinJumpId);

        // Glitch1 Max Jump Time (ms)
        parameters.addParameter(STR16("Max Jum Time"),
                                STR16("ms"), 0,
                                DspGlitchProxyVst::maxJumpToNormalized(ENT_GLITCH_DEFAULT_MAX_JUMP),
                                ParameterInfo::kCanAutomate,
                                ParameterId::Glitch1MaxJumpId);

        // Glitch2 Enabled (On/Off)
        parameters.addParameter(STR16("Glitch2 Enabled"),
                                nullptr, 2, 0.0,
                                ParameterInfo::kCanAutomate,
                                ParameterId::Glitch2EnabledId);

        // Glitch2 Repeats
        parameters.addParameter(STR16("Glitch2 Repeats"),
                                nullptr, 10,
                                DspGlitchProxyVst::repeatsToNormalized(ENT_GLITCH_DEFAULT_REPEATS),
                                ParameterInfo::kCanAutomate,
                                ParameterId::Glitch2RepeatsId);

        // Glitch2 Probability
        parameters.addParameter(STR16("Glitch2 Probability"),
                                STR16("%"), 0,
                                DspGlitchProxyVst::probabilityToNormalized(ENT_GLITCH_DEFAULT_PROB),
                                ParameterInfo::kCanAutomate,
                                ParameterId::Glitch2ProbabilityId);

        // Glitch2 Length (ms)
        parameters.addParameter(STR16("Glitch2 Length"),
                                STR16("ms"), 0,
                                DspGlitchProxyVst::lengthToNormalized(ENT_GLITCH_DEFAULT_LENGH),
                                ParameterInfo::kCanAutomate,
                                ParameterId::Glitch2LengthId);

        // Glitch2 Min Jump Time (ms)
        parameters.addParameter(STR16("Min Jump Time"),
                                STR16("ms"), 0,
                                DspGlitchProxyVst::minJumpToNormalized(ENT_GLITCH_DEFAULT_MIN_JUMP),
                                ParameterInfo::kCanAutomate,
                                ParameterId::Glitch2MinJumpId);

        // Glitch2 Max Jump Time (ms)
        parameters.addParameter(STR16("Max Jum Time"),
                                STR16("ms"), 0,
                                DspGlitchProxyVst::maxJumpToNormalized(ENT_GLITCH_DEFAULT_MAX_JUMP),
                                ParameterInfo::kCanAutomate,
                                ParameterId::Glitch2MaxJumpId);
}

void EntVstController::addRgateParameters()
{
        parameters.addParameter(STR16("Rgate Enabled"),
                                nullptr, 2, DspRgateProxyVst::enabledToNormalized(false),
                                ParameterInfo::kCanAutomate,
                                ParameterId::RgateEnabledId);

        parameters.addParameter(STR16("Rgate Min Interval"),
                                nullptr, 0,
                                DspRgateProxyVst::minIntervalToNormalized(ENT_RGATE_DEFAULT_MIN_INTERVAL),
                                ParameterInfo::kCanAutomate,
                                ParameterId::RgateMinIntervalId);

        parameters.addParameter(STR16("Rgate Max Interval"),
                                nullptr, 0,
                                DspRgateProxyVst::maxIntervalToNormalized(ENT_RGATE_DEFAULT_MAX_INTERVAL),
                                ParameterInfo::kCanAutomate,
                                ParameterId::RgateMaxIntervalId);

        parameters.addParameter(STR16("Rgate Min Duration"),
                                nullptr, 0,
                                DspRgateProxyVst::minDurationToNormalized(ENT_RGATE_DEFAULT_MIN_DURATION),
                                ParameterInfo::kCanAutomate,
                                ParameterId::RgateMinDurationId);

        parameters.addParameter(STR16("Rgate Max Duration"),
                                nullptr, 0,
                                DspRgateProxyVst::maxDurationToNormalized(ENT_RGATE_DEFAULT_MAX_DURATION),
                                ParameterInfo::kCanAutomate,
                                ParameterId::RgateMaxDurationId);

        parameters.addParameter(STR16("Rgate Min Gain"),
                                nullptr, 0,
                                DspRgateProxyVst::minGainToNormalized(ENT_RGATE_DEFAULT_MIN_GAIN),
                                ParameterInfo::kCanAutomate,
                                ParameterId::RgateMinGainId);

        parameters.addParameter(STR16("Rgate Max Gain"),
                                nullptr, 0,
                                DspRgateProxyVst::maxGainToNormalized(ENT_RGATE_DEFAULT_MAX_GAIN),
                                ParameterInfo::kCanAutomate,
                                ParameterId::RgateMaxGainId);

        parameters.addParameter(STR16("Rgate Randomness"),
                                nullptr, 0,
                                DspRgateProxyVst::randomnessToNormalized(ENT_RGATE_DEFAULT_RANDOMNESS),
                                ParameterInfo::kCanAutomate,
                                ParameterId::RgateRandomnessId);

        parameters.addParameter(STR16("Rgate Inverted"),
                                nullptr, 2,
                                DspRgateProxyVst::invertedToNormalized(ENT_RGATE_DEFAULT_INVERTED),
                                ParameterInfo::kCanAutomate,
                                ParameterId::RgateInvertedId);

        parameters.addParameter(STR16("Rgate Drywet"),
                                nullptr, 0,
                                DspRgateProxyVst::drywetToNormalized(ENT_RGATE_DEFAULT_DRYWET),
                                ParameterInfo::kCanAutomate,
                                ParameterId::RgateDrywetId);

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

void EntVstController::setStateCallback(StateCallback callback)
{
        stateCallback = callback;
}

void EntVstController::clearStateCallback()
{
        stateCallback = {};
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

void EntVstController::restartComponent()
{
        if (componentHandler)
                componentHandler->restartComponent (kParamValuesChanged);
}
