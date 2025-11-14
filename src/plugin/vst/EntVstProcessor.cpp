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
{
        ENTROPICTRON_LOG_DEBUG("called");
        initParamMappings();
}

EntVstProcessor::~EntVstProcessor()
{
}

FUnknown* EntVstProcessor::createInstance(void*)
{
        ENTROPICTRON_LOG_DEBUG("called");
        return static_cast<IAudioProcessor*>(new EntVstProcessor());
}

tresult PLUGIN_API
EntVstProcessor::initialize(FUnknown* context)
{
        ENTROPICTRON_LOG_DEBUG("called");
        auto res = AudioEffect::initialize(context);
        if (res != kResultTrue) {
                ENTROPICTRON_LOG_ERROR("called1");
                return res;
        }

        ENTROPICTRON_LOG_DEBUG("called1");

        setControllerClass(EntVstControllerUID);
        addAudioInput(reinterpret_cast<const TChar*>(u"Stereo In"),
                      SpeakerArr::kStereo);
        addAudioOutput(reinterpret_cast<const TChar*>(u"Stereo Out"),
                       SpeakerArr::kStereo);
        return kResultTrue;
}

tresult PLUGIN_API EntVstProcessor::terminate()
{
        ENTROPICTRON_LOG_DEBUG("called");
        entropictronDsp.reset();
        return AudioEffect::terminate();
}

tresult PLUGIN_API
EntVstProcessor::setBusArrangements(SpeakerArrangement* inputs,
                                    int32 numIns,
                                    SpeakerArrangement* outputs,
                                    int32 numOuts)
{
        ENTROPICTRON_LOG_DEBUG("numIns : " << numIns << ", numOuts: " << numOuts);
        return AudioEffect::setBusArrangements(inputs,
                                               numIns,
                                               outputs,
                                               numOuts);
}

tresult PLUGIN_API
EntVstProcessor::setupProcessing(ProcessSetup& setup)
{
        ENTROPICTRON_LOG_DEBUG("called");
        if (entropictronDsp)
                entropictronDsp->setSampleRate(setup.sampleRate);
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
         if (!entropictronDsp || data.numSamples < 1)
                 return kResultOk;

         // --- Collect MIDI events ---
         auto midiEvents = data.inputEvents;
         int32 nMidiEvents = midiEvents ? midiEvents->getEventCount() : 0;

         // --- Collect automation events ---
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
                         entropictronDsp->pressKey(eventQueue[i].note.pitch, eventQueue[i].note.velocity, true);
                         break;
                 case QueuedEvent::Type::NoteOff:
                         entropictronDsp->pressKey(eventQueue[i].note.pitch, eventQueue[i].note.velocity, false);
                         break;
                 case QueuedEvent::Type::Automation:
                         updateParameters(static_cast<ParameterId>(eventQueue[i].automation.pid),
                                          eventQueue[i].automation.value);
                         break;
                 }
         }

         // Process remaining buffer after last event
         if (currentFrame < static_cast<size_t>(data.numSamples)) {
                 size_t remaining = data.numSamples - currentFrame;
                 entropictronDsp->process(buffer, remaining);
         }

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

void EntVstProcessor::updateParameters(ParameterId id, ParamValue value)
 {
         ENTROPICTRON_LOG_DEBUG("called");
        if (auto it = paramMap.find(id); it != paramMap.end())
                it->second(value);
}

void EntVstProcessor::initParamMappings()
{
        initNoiseParamMappings();
        initCrackleParamMappings();
        initGlitchParamMappings();
}

void EntVstProcessor::initNoiseParamMappings()
{
        ENTROPICTRON_LOG_DEBUG("called");
        // Noise 1
        auto noise = entropictronDsp->getNoise(NoiseId::Noise1);
        ENTROPICTRON_LOG_DEBUG("called-0.1");
        paramMap[ParameterId::Noise1EnabledId] = [noise](ParamValue v) {
                ENTROPICTRON_LOG_DEBUG("called-0.2");
                noise->enable(v >= 0.5);
        };
        ENTROPICTRON_LOG_DEBUG("called-0.3");
        paramMap[ParameterId::Noise1TypeId] = [noise](ParamValue v) {
                auto n = static_cast<int>(NoiseType::BrownNoise);
                auto type = static_cast<NoiseType>(v * n  + 0.5);
                noise->setType(type);
        };
        paramMap[ParameterId::Noise1DensityId] = [noise](ParamValue v) {
                noise->setDensity(static_cast<float>(v));
        };
        paramMap[ParameterId::Noise1BrightnessId] = [noise](ParamValue v) {
                noise->setBrightness(static_cast<float>(v));
        };
        paramMap[ParameterId::Noise1GainId] = [noise](ParamValue v) {
                noise->setGain(static_cast<float>(v));
        };
        paramMap[ParameterId::Noise1StereoId] = [noise](ParamValue v) {
                noise->setStereo(static_cast<float>(v));
        };
        paramMap[ParameterId::Noise1FilerTyped] = [noise](ParamValue v) {
                auto n = static_cast<int>(FilterType::HighPass);
                auto type = static_cast<NoiseType>(v * n  + 0.5);
                noise->setFilerType(type);
        };
        paramMap[ParameterId::Noise1CutOffId] = [noise](ParamValue v) {
                noise->setCutOff(static_cast<float>(v));
        };
        paramMap[ParameterId::Noise1ResonanceId] = [noise](ParamValue v) {
                noise->setResonance(static_cast<float>(v));
        };

        ENTROPICTRON_LOG_DEBUG("called1");
        // Noise 2
        noise = entropictronDsp->getNoise(NoiseId::Noise2);
        paramMap[ParameterId::Noise2EnabledId] = [noise](ParamValue v) {
                ENTROPICTRON_LOG_INFO("Noise2EnabledId: v: " << v);
                noise->enable(v >= 0.5);
        };
        paramMap[ParameterId::Noise2TypeId] = [noise](ParamValue v) {
                auto n = static_cast<int>(NoiseType::BrownNoise);
                auto type = static_cast<NoiseType>(v * n  + 0.5);
                noise->setType(type);
        };
        paramMap[ParameterId::Noise2DensityId] = [noise](ParamValue v) {
                noise->setDensity(static_cast<float>(v));
        };
        paramMap[ParameterId::Noise2BrightnessId] = [noise](ParamValue v) {
                noise->setBrightness(static_cast<float>(v));
        };
        paramMap[ParameterId::Noise2GainId] = [noise](ParamValue v) {
                noise->setGain(static_cast<float>(v));
        };
        paramMap[ParameterId::Noise2StereoId] = [noise](ParamValue v) {
                noise->setStereo(static_cast<float>(v));
        };
        paramMap[ParameterId::Noise2FilerTyped] = [noise](ParamValue v) {
                auto n = static_cast<int>(FilterType::HighPass);
                auto type = static_cast<NoiseType>(v * n  + 0.5);
                noise->setFilerType(type);
        };
        paramMap[ParameterId::Noise2CutOffId] = [noise](ParamValue v) {
                noise->setCutOff(static_cast<float>(v));
        };
        paramMap[ParameterId::Noise2ResonanceId] = [noise](ParamValue v) {
                noise->setResonance(static_cast<float>(v));
        };

        ENTROPICTRON_LOG_DEBUG("END");
}

void EntVstProcessor::initCrackleParamMappings()
{
        ENTROPICTRON_LOG_DEBUG("called");
        // Crackle 1
        auto crackle = entropictronDsp->getCrackle(CrackleId::Crackle1);
        paramMap[ParameterId::Crackle1EnabledId] = [crackle](ParamValue v) {
                crackle->enable(v > 0.5);
        };

        paramMap[ParameterId::Crackle1RateId] = [crackle](ParamValue v) {
                float rateHz = static_cast<float>(v) * 100.0f;
                crackle->setRate(rateHz);
        };

        paramMap[ParameterId::Crackle1DurationId] = [crackle](ParamValue v) {
                float durationMs = 1.0f + static_cast<float>(v) * (50.0f - 1.0f);
                crackle->setDuration(durationMs);
        };

        paramMap[ParameterId::Crackle1AmplitudeId] = [crackle](ParamValue v) {
                crackle->setAmplitude(static_cast<float>(v));
        };

        paramMap[ParameterId::Crackle1RandomnessId] = [crackle](ParamValue v) {
                crackle->setRandomness(static_cast<float>(v) * 100.0f);
        };

        paramMap[ParameterId::Crackle1BrightnessId] = [crackle](ParamValue v) {
                crackle->setBrightness(static_cast<float>(v));
        };

        paramMap[ParameterId::Crackle1EnvelopeShapeId] = [crackle](ParamValue v) {
                crackle->setEnvelopeShape(static_cast<CrackleEnvelopeShape>(v));
        };

        paramMap[ParameterId::Crackle1StereoSpreadId] = [crackle](ParamValue v) {
                crackle->setStereoSpread(static_cast<float>(v) * 100.0f);
        };

        crackle = entropictronDsp->getCrackle(CrackleId::Crackle1);

        // Crackle 2
        paramMap[ParameterId::Crackle2EnabledId] = [crackle](ParamValue v) {
                crackle->enable(v > 0.5);
        };

        paramMap[ParameterId::Crackle2RateId] = [crackle](ParamValue v) {
                float rateHz = static_cast<float>(v) * 100.0f;
                crackle->setRate(rateHz);
        };

        paramMap[ParameterId::Crackle2DurationId] = [crackle](ParamValue v) {
                float durationMs = 1.0f + static_cast<float>(v) * (50.0f - 1.0f);
                crackle->setDuration(durationMs);
        };

        paramMap[ParameterId::Crackle2AmplitudeId] = [crackle](ParamValue v) {
                crackle->setAmplitude(static_cast<float>(v));
        };

        paramMap[ParameterId::Crackle2RandomnessId] = [crackle](ParamValue v) {
                crackle->setRandomness(static_cast<float>(v) * 100.0f);
        };

        paramMap[ParameterId::Crackle2BrightnessId] = [crackle](ParamValue v) {
                crackle->setBrightness(static_cast<float>(v));
        };

        paramMap[ParameterId::Crackle2EnvelopeShapeId] = [crackle](ParamValue v) {
                crackle->setEnvelopeShape(static_cast<CrackleEnvelopeShape>(v));
        };

        paramMap[ParameterId::Crackle2StereoSpreadId] = [crackle](ParamValue v) {
                crackle->setStereoSpread(static_cast<float>(v) * 100.0f);
        };
}

void EntVstProcessor::initGlitchParamMappings()
{
        ENTROPICTRON_LOG_DEBUG("called");
        // Glitch 1
        auto glitch = entropictronDsp->getGlitch(GlitchId::Glitch1);
        paramMap[ParameterId::Glitch1EnabledId] = [glitch](ParamValue v) {
                glitch->enable(v > 0.5);
        };

        paramMap[ParameterId::Glitch1ProbabilityId] = [glitch](ParamValue v) {
                float prob = static_cast<float>(v) * 100.0f;
                glitch->setProbability(prob);
        };

        paramMap[ParameterId::Glitch1MinJumpId] = [glitch](ParamValue v) {
                float minMs = static_cast<float>(v) * 2000.0f;
                glitch->setJumpMin(minMs);
        };

        paramMap[ParameterId::Glitch1MaxJumpId] = [glitch](ParamValue v) {
                float maxMs = static_cast<float>(v) * 5000.0f;
                glitch->setJumpMax(maxMs);
        };

        paramMap[ParameterId::Glitch1LengthId] = [glitch](ParamValue v) {
                float lenMs = static_cast<float>(v) * 2000.0f;
                glitch->setLength(lenMs);
        };

        paramMap[ParameterId::Glitch1RepeatsId] = [glitch](ParamValue v) {
                int repeats = 1 + static_cast<int>(v * 9.0f);
                glitch->setRepeatCount(repeats);
        };

        // Glitch 2
        glitch = entropictronDsp->getGlitch(GlitchId::Glitch2);
        paramMap[ParameterId::Glitch2EnabledId] = [glitch](ParamValue v) {
                glitch->enable(v > 0.5);
        };

        paramMap[ParameterId::Glitch2ProbabilityId] = [glitch](ParamValue v) {
                float prob = static_cast<float>(v) * 100.0f;
                glitch->setProbability(prob);
        };

        paramMap[ParameterId::Glitch2MinJumpId] = [glitch](ParamValue v) {
                float minMs = static_cast<float>(v) * 2000.0f;
                glitch->setJumpMin(minMs);
        };

        paramMap[ParameterId::Glitch2MaxJumpId] = [glitch](ParamValue v) {
                float maxMs = static_cast<float>(v) * 5000.0f;
                glitch->setJumpMax(maxMs);
        };

        paramMap[ParameterId::Glitch2LengthId] = [glitch](ParamValue v) {
                float lenMs = static_cast<float>(v) * 2000.0f;
                glitch->setLength(lenMs);
        };

        paramMap[ParameterId::Glitch2RepeatsId] = [glitch](ParamValue v) {
                int repeats = 1 + static_cast<int>(v * 9.0f);
                glitch->setRepeatCount(repeats);
        };
}


