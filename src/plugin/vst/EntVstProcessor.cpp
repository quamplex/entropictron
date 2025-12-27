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
#include "DspWrapper.h"
#include "DspWrapperNoise.h"
#include "DspWrapperCrackle.h"
#include "DspWrapperGlitch.h"
#include "EntVstParameters.h"
#include "DspVstProxy.h"
#include "DspNoiseProxyVst.h"
#include "DspCrackleProxyVst.h"
#include "DspGlitchProxyVst.h"
#include "EntState.h"
#include "ent_state.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstevents.h"

using namespace EntVst;
using namespace Steinberg::Vst;

static constexpr int MAX_EVENTS = 512;

struct QueuedEvent {
    int32 sampleOffset;
    enum class Type { NoteOn, NoteOff, Automation } type;
    union {
        struct { int32 pitch; float velocity; bool on; } note;
        struct { Steinberg::Vst::ParamID pid; Steinberg::Vst::ParamValue value; } automation;
    };
};

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
        :  entropictronDsp {std::make_unique<DspWrapper>()}
        , dspStateUpdated{false}
        , isPendingState{false}
        , dspState{ent_state_create()}
{
        entropictronDsp->getState(dspState);
        initParamMappings();
}

EntVstProcessor::~EntVstProcessor()
{
        ent_state_free(dspState);
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
        addAudioInput(reinterpret_cast<const TChar*>(u"Stereo In"),
                      SpeakerArr::kStereo);
        addAudioOutput(reinterpret_cast<const TChar*>(u"Stereo Out"),
                       SpeakerArr::kStereo);
        addEventInput(reinterpret_cast<const TChar*>(u"MIDI Input"), 1);

        entropictronDsp->getState(dspState);
        isPendingState.store(false, std::memory_order_release);

        processContextRequirements.flags = ProcessContext::kPlaying;

        return kResultTrue;
}

tresult PLUGIN_API EntVstProcessor::terminate()
{
        entropictronDsp.reset();
        return AudioEffect::terminate();
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
        if (entropictronDsp)
                entropictronDsp->setSampleRate(setup.sampleRate);
        return AudioEffect::setupProcessing(setup);
}

tresult PLUGIN_API
EntVstProcessor::setActive(TBool state)
{
        return AudioEffect::setActive(state);
}

tresult PLUGIN_API EntVstProcessor::setProcessing (TBool state)
{
        return AudioEffect::setProcessing (state);
}

tresult PLUGIN_API
EntVstProcessor::process(ProcessData& data)
 {
         if (!entropictronDsp || data.numSamples < 1)
                 return kResultOk;

         memset(data.outputs[0].channelBuffers32[0], 0,
                data.numSamples * sizeof(float));
         memset(data.outputs[0].channelBuffers32[1], 0,
                data.numSamples * sizeof(float));

         const auto* ctx = data.processContext;
         if (entropictronDsp->playMode() == PlayMode::PlaybackMode && ctx)
                 entropictronDsp->pressKey(ctx->state & ProcessContext::kPlaying);


         bool expected = true;
         auto ok = isPendingState.compare_exchange_strong(expected,
                                                          false,
                                                          std::memory_order_acquire,
                                                          std::memory_order_relaxed);
         if (ok)
                 entropictronDsp->setState(dspState);

         // Collect MIDI events
         auto midiEvents = data.inputEvents;
         int32 nMidiEvents = midiEvents ? midiEvents->getEventCount() : 0;

         // Collect automation events
         auto inputParams = data.inputParameterChanges;
         std::array<QueuedEvent, MAX_EVENTS> eventQueue;
         int eventCount = 0;

         // Insert MIDI events into queue
         for (int32 i = 0; i < nMidiEvents && eventCount < MAX_EVENTS; ++i) {
                 Event event{};
                 if (midiEvents->getEvent(i, event) == kResultOk) {
                         QueuedEvent qe;
                         qe.sampleOffset = event.sampleOffset;

                         switch (event.type) {
                         case Event::kNoteOnEvent:
                                 qe.type = QueuedEvent::Type::NoteOn;
                                 qe.note.pitch = event.noteOn.pitch;
                                 qe.note.velocity = event.noteOn.velocity;
                                 qe.note.on = true;
                                 eventQueue[eventCount++] = qe;
                                 break;

                         case Event::kNoteOffEvent:
                                 qe.type = QueuedEvent::Type::NoteOff;
                                 qe.note.pitch = event.noteOff.pitch;
                                 qe.note.velocity = event.noteOff.velocity;
                                 qe.note.on = false;
                                 eventQueue[eventCount++] = qe;
                                 break;

                         default:
                                 break;
                         }
                 }
         }

         // Insert automation parameter changes into queue
         if (inputParams) {
                 int32 paramCount = inputParams->getParameterCount();
                 for (int32 i = 0; i < paramCount && eventCount < MAX_EVENTS; ++i) {
                         auto queue = inputParams->getParameterData(i);
                         if (!queue)
                                 continue;

                         ParamID pid = queue->getParameterId();
                         int32 pointCount = queue->getPointCount();

                         // Push all points as automation events
                         for (int32 p = 0; p < pointCount && eventCount < MAX_EVENTS; ++p) {
                                 int32 sampleOffset = 0;
                                 ParamValue value = 0;
                                 if (queue->getPoint(p, sampleOffset, value) == kResultOk) {
                                         QueuedEvent qe;
                                         qe.sampleOffset = sampleOffset;
                                         qe.type = QueuedEvent::Type::Automation;
                                         qe.automation.pid = pid;
                                         qe.automation.value = value;
                                         eventQueue[eventCount++] = qe;
                                 }
                         }
                 }
         }

         // Sort all events by sampleOffset ascending
         std::sort(eventQueue.begin(), eventQueue.begin() + eventCount,
                   [](const QueuedEvent& a, const QueuedEvent& b) {
                           return a.sampleOffset < b.sampleOffset;
                   });

         float* buffer[4] = {
                 data.inputs[0].channelBuffers32[0],
                 data.inputs[0].channelBuffers32[1],
                 data.outputs[0].channelBuffers32[0],
                 data.outputs[0].channelBuffers32[1]
         };

         size_t currentFrame = 0;
         for (int i = 0; i < eventCount; ++i) {
                 size_t eventFrame = static_cast<size_t>(eventQueue[i].sampleOffset);
                 if (eventFrame > static_cast<size_t>(data.numSamples))
                         eventFrame = data.numSamples;

                 size_t chunkSize = eventFrame - currentFrame;
                 if (chunkSize > 0) {
                         entropictronDsp->process(buffer, chunkSize);
                         buffer[0] += chunkSize;
                         buffer[1] += chunkSize;
                         buffer[2] += chunkSize;
                         buffer[3] += chunkSize;
                         currentFrame += chunkSize;
                 }

                 // Apply event at exact sample
                 switch (eventQueue[i].type) {
                 case QueuedEvent::Type::NoteOn:
                         if (entropictronDsp->playMode() == PlayMode::HoldMode)
                                 entropictronDsp->pressKey(true);
                         break;
                 case QueuedEvent::Type::NoteOff:
                         if (entropictronDsp->playMode() == PlayMode::HoldMode)
                                 entropictronDsp->pressKey(false);
                         break;
                 case QueuedEvent::Type::Automation:
                         updateParameters(static_cast<ParameterId>(eventQueue[i].automation.pid),
                                          eventQueue[i].automation.value);
                         dspStateUpdated = true;
                         break;
                 }
         }

         // Process remaining buffer after last event
         if (currentFrame < static_cast<size_t>(data.numSamples)) {
                 size_t remaining = data.numSamples - currentFrame;
                 entropictronDsp->process(buffer, remaining);
         }

         if (dspStateUpdated) {
                 entropictronDsp->getState(dspState);
                 dspStateUpdated = false;
         }

         return kResultOk;
 }

void EntVstProcessor::updateParameters(ParameterId id, ParamValue value)
 {
        if (auto it = paramMap.find(id); it != paramMap.end())
                it->second(value);
}

void EntVstProcessor::initParamMappings()
{
        paramMap[ParameterId::PlayModeId] = [this](ParamValue v) {
                entropictronDsp->setPlayMode(DspProxyVst::playModeFromNormalized(v));
        };

        initNoiseParamMappings();
        initCrackleParamMappings();
        initGlitchParamMappings();
}

void EntVstProcessor::initNoiseParamMappings()
{
        // Noise 1
        auto noise = entropictronDsp->getNoise(NoiseId::Noise1);
        paramMap[ParameterId::Noise1EnabledId] = [noise](ParamValue v) {
                noise->enable(v >= 0.5);
        };
        paramMap[ParameterId::Noise1TypeId] = [noise](ParamValue v) {
                noise->setType(DspNoiseProxyVst::noiseTypeFromNormalized(v));
        };
        paramMap[ParameterId::Noise1DensityId] = [noise](ParamValue v) {
                noise->setDensity(static_cast<float>(v));
        };
        paramMap[ParameterId::Noise1BrightnessId] = [noise](ParamValue v) {
                noise->setBrightness(static_cast<float>(v));
        };
        paramMap[ParameterId::Noise1GainId] = [noise](ParamValue v) {
                noise->setGain(DspNoiseProxyVst::gainFromNormalized(v));
        };
        paramMap[ParameterId::Noise1StereoId] = [noise](ParamValue v) {
                noise->setStereo(v);
        };
        paramMap[ParameterId::Noise1FilterTypeId] = [noise](ParamValue v) {
                noise->setFilterType(DspNoiseProxyVst::filterTypeFromNormalized(v));
        };
        paramMap[ParameterId::Noise1CutOffId] = [noise](ParamValue v) {
                noise->setCutOff(DspNoiseProxyVst::cutoffFromNormalized(v));
        };
        paramMap[ParameterId::Noise1ResonanceId] = [noise](ParamValue v) {
                noise->setResonance(static_cast<float>(v));
        };

        // Noise 2
        noise = entropictronDsp->getNoise(NoiseId::Noise2);
        paramMap[ParameterId::Noise2EnabledId] = [noise](ParamValue v) {
                noise->enable(v >= 0.5);
        };
        paramMap[ParameterId::Noise2TypeId] = [noise](ParamValue v) {
                noise->setType(DspNoiseProxyVst::noiseTypeFromNormalized(v));
        };
        paramMap[ParameterId::Noise2DensityId] = [noise](ParamValue v) {
                noise->setDensity(static_cast<float>(v));
        };
        paramMap[ParameterId::Noise2BrightnessId] = [noise](ParamValue v) {
                noise->setBrightness(static_cast<float>(v));
        };
        paramMap[ParameterId::Noise2GainId] = [noise](ParamValue v) {
                noise->setGain(DspNoiseProxyVst::gainFromNormalized(v));
        };
        paramMap[ParameterId::Noise2StereoId] = [noise](ParamValue v) {
                noise->setStereo(v);
        };
        paramMap[ParameterId::Noise2FilterTypeId] = [noise](ParamValue v) {
                noise->setFilterType(DspNoiseProxyVst::filterTypeFromNormalized(v));
        };
        paramMap[ParameterId::Noise2CutOffId] = [noise](ParamValue v) {
                noise->setCutOff(DspNoiseProxyVst::cutoffFromNormalized(v));
        };
        paramMap[ParameterId::Noise2ResonanceId] = [noise](ParamValue v) {
                noise->setResonance(static_cast<float>(v));
        };
}

void EntVstProcessor::initCrackleParamMappings()
{
        // Crackle 1
        auto crackle = entropictronDsp->getCrackle(CrackleId::Crackle1);
        paramMap[ParameterId::Crackle1EnabledId] = [crackle](ParamValue v) {
                crackle->enable(v > 0.5);
        };
        paramMap[ParameterId::Crackle1RateId] = [crackle](ParamValue v) {
                // 0.5 - 150Hz
                crackle->setRate(DspCrackleProxyVst::rateFromNormalized(v));
        };
        paramMap[ParameterId::Crackle1DurationId] = [crackle](ParamValue v) {
                crackle->setDuration(DspCrackleProxyVst::durationFromNormalized(v));
        };
        paramMap[ParameterId::Crackle1AmplitudeId] = [crackle](ParamValue v) {
                crackle->setAmplitude(v);
        };
        paramMap[ParameterId::Crackle1RandomnessId] = [crackle](ParamValue v) {
                crackle->setRandomness(v);
        };
        paramMap[ParameterId::Crackle1BrightnessId] = [crackle](ParamValue v) {
                crackle->setBrightness(v);
        };
        paramMap[ParameterId::Crackle1EnvelopeShapeId] = [crackle](ParamValue v) {
                crackle->setEnvelopeShape(DspCrackleProxyVst::envelopeShapeFromNormalized(v));
        };
        paramMap[ParameterId::Crackle1StereoSpreadId] = [crackle](ParamValue v) {
                crackle->setStereoSpread(v);
        };

        // Crackle 1
        crackle = entropictronDsp->getCrackle(CrackleId::Crackle2);
        paramMap[ParameterId::Crackle2EnabledId] = [crackle](ParamValue v) {
                crackle->enable(v > 0.5);
        };
        paramMap[ParameterId::Crackle2RateId] = [crackle](ParamValue v) {
                crackle->setRate(DspCrackleProxyVst::rateFromNormalized(v));
        };
        paramMap[ParameterId::Crackle2DurationId] = [crackle](ParamValue v) {
                crackle->setDuration(DspCrackleProxyVst::durationFromNormalized(v));
        };
        paramMap[ParameterId::Crackle2AmplitudeId] = [crackle](ParamValue v) {
                crackle->setAmplitude(v);
        };
        paramMap[ParameterId::Crackle2RandomnessId] = [crackle](ParamValue v) {
                crackle->setRandomness(v);
        };
        paramMap[ParameterId::Crackle2BrightnessId] = [crackle](ParamValue v) {
                crackle->setBrightness(v);
        };
        paramMap[ParameterId::Crackle2EnvelopeShapeId] = [crackle](ParamValue v) {
                crackle->setEnvelopeShape(DspCrackleProxyVst::envelopeShapeFromNormalized(v));
        };
        paramMap[ParameterId::Crackle2StereoSpreadId] = [crackle](ParamValue v) {
                crackle->setStereoSpread(v);
        };
}

void EntVstProcessor::initGlitchParamMappings()
{
        // Glitch 1
        auto glitch = entropictronDsp->getGlitch(GlitchId::Glitch1);
        paramMap[ParameterId::Glitch1EnabledId] = [glitch](ParamValue v) {
                glitch->enable(v > 0.5);
        };
        paramMap[ParameterId::Glitch1ProbabilityId] = [glitch](ParamValue v) {
                glitch->setProbability(DspGlitchProxyVst::probabilityFromNormalized(v));
        };
        paramMap[ParameterId::Glitch1MinJumpId] = [glitch](ParamValue v) {
                glitch->setJumpMin(DspGlitchProxyVst::minJumpFromNormalized(v));
        };
        paramMap[ParameterId::Glitch1MaxJumpId] = [glitch](ParamValue v) {
                glitch->setJumpMax(DspGlitchProxyVst::maxJumpFromNormalized(v));
        };
        paramMap[ParameterId::Glitch1LengthId] = [glitch](ParamValue v) {
                glitch->setLength(DspGlitchProxyVst::lengthFromNormalized(v));
        };
        paramMap[ParameterId::Glitch1RepeatsId] = [glitch](ParamValue v) {
                glitch->setRepeatCount(DspGlitchProxyVst::repeatsFromNormalized(v));
        };

        // Glitch 2
        glitch = entropictronDsp->getGlitch(GlitchId::Glitch2);
        paramMap[ParameterId::Glitch2EnabledId] = [glitch](ParamValue v) {
                glitch->enable(v > 0.5);
        };
        paramMap[ParameterId::Glitch1ProbabilityId] = [glitch](ParamValue v) {
                glitch->setProbability(DspGlitchProxyVst::probabilityFromNormalized(v));
        };
        paramMap[ParameterId::Glitch2MinJumpId] = [glitch](ParamValue v) {
                glitch->setJumpMin(DspGlitchProxyVst::minJumpFromNormalized(v));
        };
        paramMap[ParameterId::Glitch2MaxJumpId] = [glitch](ParamValue v) {
                glitch->setJumpMax(DspGlitchProxyVst::maxJumpFromNormalized(v));
        };
        paramMap[ParameterId::Glitch2LengthId] = [glitch](ParamValue v) {
                glitch->setLength(DspGlitchProxyVst::lengthFromNormalized(v));
        };
        paramMap[ParameterId::Glitch2RepeatsId] = [glitch](ParamValue v) {
                glitch->setRepeatCount(DspGlitchProxyVst::repeatsFromNormalized(v));
        };
}

tresult EntVstProcessor::setState (IBStream *state)
{
        if (state == nullptr)
                return kInvalidArgument;

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
        entState.getState(dspState);

        isPendingState.store(true, std::memory_order_release);

        return kResultOk;
}

tresult EntVstProcessor::getState (IBStream *state)
{
        if (state == nullptr)
                return kInvalidArgument;

        EntState entState{dspState};
        int32 nBytes = 0;
        auto data = entState.toJson();
        if (state->write(data.data(), data.size(), &nBytes) == kResultFalse) {
                ENT_LOG_ERROR("error on saving the state");
                return kResultFalse;
        }

        if (static_cast<decltype(nBytes)>(data.size()) != nBytes) {
                ENT_LOG_ERROR("error on saving the state");
                return kResultFalse;
        }

        return kResultOk;
}
