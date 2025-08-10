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
#include <array>
#include <algorithm> // for std::sort

static constexpr int MAX_EVENTS = 512;

struct QueuedEvent {
    int32 sampleOffset;
    enum class Type { NoteOn, NoteOff, Automation } type;
    union {
        struct { int32 pitch; float velocity; bool on; } note;
        struct { Steinberg::Vst::ParamID pid; Steinberg::Vst::ParamValue value; } automation;
    };
};

 tresult PLUGIN_API EntVstProcessor::process(ProcessData& data)
 {
         using namespace Steinberg::Vst;

         if (!entropictronDsp || data.numSamples < 1)
                 return kResultOk;

         // --- Collect MIDI events ---
         auto midiEvents = data.inputEvents;
         int32 nMidiEvents = midiEvents ? midiEvents->getEventCount() : 0;

         // --- Collect automation events ---
         InputParameterChanges* inputParams = data.inputParameterChanges;
         int32 nAutomationEvents = 0;
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
                 if (eventFrame > data.numSamples)
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
                         updateParameters(eventQueue[i].automation.pid, eventQueue[i].automation.value);
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
