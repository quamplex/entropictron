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
{
        initParamMap();
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
        if (res != kResultTrue)
                return res;

        entropictronDsp = std::make_unique<DspWrapper>();

        setControllerClass(EntVstControllerUID);
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

         float* buffer[2] = {
                 data.outputs[0].channelBuffers32[0],
                 data.outputs[0].channelBuffers32[1]  // fixed - was [0] twice
         };
         memset(buffer[0], 0, data.numSamples * sizeof(float));
         memset(buffer[1], 0, data.numSamples * sizeof(float));

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

void EntVstProcessor::initParamMap()
{
        // Noise 1
        paramMap[ParameterId::Noise1EnabledId] = [this](ParamValue v) {
                ENTROPICTRON_LOG_INFO("Noise2EnabledId: v: " << v);
                entropictronDsp->getNoise(NoiseId::Noise1)->enable(v >= 0.5);
        };
        paramMap[ParameterId::Noise1TypeId] = [this](ParamValue v) {
                entropictronDsp->getNoise(NoiseId::Noise1)->setType(static_cast<NoiseType>(v + 0.5));
        };
        paramMap[ParameterId::Noise1DensityId] = [this](ParamValue v) {
                entropictronDsp->getNoise(NoiseId::Noise1)->setDensity(static_cast<float>(v));
        };
        paramMap[ParameterId::Noise1BrightnessId] = [this](ParamValue v) {
                entropictronDsp->getNoise(NoiseId::Noise1)->setBrightness(static_cast<float>(v));
        };
        paramMap[ParameterId::Noise1GainId] = [this](ParamValue v) {
                entropictronDsp->getNoise(NoiseId::Noise1)->setGain(static_cast<float>(v));
        };

        // Noise 2
        paramMap[ParameterId::Noise2EnabledId] = [this](ParamValue v) {
                ENTROPICTRON_LOG_INFO("Noise2EnabledId: v: " << v);
                entropictronDsp->getNoise(NoiseId::Noise2)->enable(v >= 0.5);
        };
        paramMap[ParameterId::Noise2TypeId] = [this](ParamValue v) {
                entropictronDsp->getNoise(NoiseId::Noise2)->setType(static_cast<NoiseType>(v + 0.5));
        };
        paramMap[ParameterId::Noise2DensityId] = [this](ParamValue v) {
                entropictronDsp->getNoise(NoiseId::Noise2)->setDensity(static_cast<float>(v));
        };
        paramMap[ParameterId::Noise2BrightnessId] = [this](ParamValue v) {
                entropictronDsp->getNoise(NoiseId::Noise2)->setBrightness(static_cast<float>(v));
        };
        paramMap[ParameterId::Noise2GainId] = [this](ParamValue v) {
                entropictronDsp->getNoise(NoiseId::Noise2)->setGain(static_cast<float>(v));
        };
}
