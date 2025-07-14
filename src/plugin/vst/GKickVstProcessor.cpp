/**
 * File name: GKickVstProcessor.h
 * Project: Geonkick (A percussion synthesizer)
 *
 * Copyright (C) 2019 Iurie Nistor
 *
 * This file is part of Geonkick.
 *
 * GeonKick is free software; you can redistribute it and/or modify
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

#include "GKickVstProcessor.h"
#include "GKickVstEditor.h"
#include "VstIds.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstevents.h"

#include "geonkick_api.h"
#include "kit_state.h"
#include "percussion_state.h"

#ifndef GEONKICK_OS_WINDOWS
bool ModuleEntry (void*)
{
        return true;
}

bool ModuleExit (void)
{
        return true;
}
#endif // GEONKICK_OS_WINDOWS


class MemoryGlitcherEffect
{
public:
    MemoryGlitcherEffect(int sampleRate = 48000)
    {
        setSampleRate(sampleRate);
    }

    void setSampleRate(int sr)
    {
        sampleRate = sr;
        bufferSize = sr; // 1 second
        buffer.assign(bufferSize, 0.0f);
        writePos = 0;
        glitchPos = -1;
        glitchCount = 0;
    }

    void setParameters(float probability, float jumpMinMs, float jumpMaxMs, float glitchLenMs, int glitchRepeats)
    {
        glitchProbability = probability;
        jumpBackMinSamples = static_cast<int>(jumpMinMs * sampleRate / 1000.0f);
        jumpBackMaxSamples = static_cast<int>(jumpMaxMs * sampleRate / 1000.0f);
        glitchLengthSamples = static_cast<int>(glitchLenMs * sampleRate / 1000.0f);
        glitchRepeatCount = std::max(1, glitchRepeats);
    }

        float processSample(float input)
        {
                buffer[writePos] = input;
                float output = input;

                if (glitchCount > 0 && glitchPos >= 0)
                        {
                                int playbackIndex = (glitchPos + glitchPlayPos) % bufferSize;
                                output = buffer[playbackIndex];

                                glitchPlayPos++;
                                glitchCount--;
                        }
                else if (((float)rand() / RAND_MAX) < glitchProbability)
                        {
                                int jumpBackRange = std::max(1, jumpBackMaxSamples - jumpBackMinSamples);
                                int jumpBack = jumpBackMinSamples + rand() % jumpBackRange;

                                glitchPos = (writePos - jumpBack + bufferSize) % bufferSize;
                                glitchCount = glitchLengthSamples * glitchRepeatCount;
                                glitchPlayPos = 0; // start from beginning of glitch
                        }

                writePos = (writePos + 1) % bufferSize;
                return output;
        }

private:
    int sampleRate = 48000;
    int bufferSize = 48000;
    std::vector<float> buffer;
    int writePos = 0;
    int glitchPos = -1;
    int glitchCount = 0;

    int glitchPlayPos = 0;
    float glitchProbability = 0.1f;
    int jumpBackMinSamples = 2400;
    int jumpBackMaxSamples = 14400;
    int glitchLengthSamples = 1920;
    int glitchRepeatCount = 2;
};


MemoryGlitcherEffect glitchEffect;


GKickVstProcessor::GKickVstProcessor()
        : geonkickApi{nullptr}
	, sampleRate{Geonkick::defaultSampleRate}
{
}

GKickVstProcessor::~GKickVstProcessor()
{
}

FUnknown* GKickVstProcessor::createInstance(void*)
{
        return static_cast<Vst::IAudioProcessor*>(new GKickVstProcessor());
}


tresult PLUGIN_API
GKickVstProcessor::initialize(FUnknown* context)
{
        auto res = Vst::SingleComponentEffect::initialize(context);
        if (res != kResultTrue)
                return res;

        addAudioOutput(reinterpret_cast<const Steinberg::Vst::TChar*>(u"Stereo Out"),
                       Vst::SpeakerArr::kStereo);
        parameters.addParameter(STR16("Noise Type"),
                                nullptr,
                                2,
                                1.0,
                                Vst::ParameterInfo::kCanAutomate,
                                1000);
        parameters.addParameter(STR16("Gain"),
                                nullptr,
                                0,
                                1.0,
                                Vst::ParameterInfo::kCanAutomate,
                                1001);
        parameters.addParameter(STR16("Density"),
                                nullptr,
                                0,
                                1.0,
                                Vst::ParameterInfo::kCanAutomate,
                                1002);

        return kResultTrue;
}

tresult PLUGIN_API
GKickVstProcessor::setBusArrangements(Vst::SpeakerArrangement* inputs,
                                      int32 numIns,
                                      Vst::SpeakerArrangement* outputs,
                                      int32 numOuts)
{
        return Vst::SingleComponentEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
}

tresult PLUGIN_API
GKickVstProcessor::setupProcessing(Vst::ProcessSetup& setup)
{
        return Vst::SingleComponentEffect::setupProcessing(setup);
}

tresult PLUGIN_API
GKickVstProcessor::setActive(TBool state)
{
        return Vst::SingleComponentEffect::setActive(state);
}

tresult PLUGIN_API GKickVstProcessor::process(Vst::ProcessData& data)
{
        if (!data.inputs || data.numInputs < 1 || !data.outputs || data.numOutputs < 1)
                return kResultOk;

        float* inL = data.inputs[0].channelBuffers32[0];
        float* outL = data.outputs[0].channelBuffers32[0];
        float* inR = data.inputs[0].channelBuffers32[1];
        float* outR = data.outputs[0].channelBuffers32[1];

        int32 numSamples = data.numSamples;

        // Read parameters from automation queues
        float glitchProb = 0.1f;
        float jumpMin = 50.0f;
        float jumpMax = 300.0f;
        float glitchLen = 40.0f;
        int glitchRepeats = 2;

        readParameters(data);



        // Process audio
        for (int32 i = 0; i < numSamples; ++i)
                {
                        outL[i] = glitchEffect.processSample(inL[i]);
                        outR[i] = glitchEffect.processSample(inR[i]);
                }

        return kResultOk;
}

tresult PLUGIN_API
GKickVstProcessor::setState(IBStream* state)
{
        if (state == nullptr)
                return kResultTrue;

        if (state->seek(0, IBStream::kIBSeekEnd, 0) == kResultFalse) {
                GEONKICK_LOG_ERROR("can't seek in stream");
                return kResultFalse;
        }

        int64 endStream = 0;
        if (state->tell(&endStream) == kResultFalse) {
                GEONKICK_LOG_ERROR("can't get current position in stream");
                return kResultFalse;
        } else if (endStream < 1) {
                GEONKICK_LOG_ERROR("stream is empty");
                return kResultFalse;
        }

        if (state->seek(0, IBStream::kIBSeekSet, 0) == kResultFalse) {
                GEONKICK_LOG_ERROR("can't seek in stream");
                return kResultFalse;
        }

        std::string data(endStream, '\0');
        int32 nBytes = 0;
        if (state->read(data.data(), data.size(), &nBytes) == kResultFalse) {
                GEONKICK_LOG_ERROR("error on reading the state");
                return kResultFalse;
        }

        if (static_cast<decltype(nBytes)>(data.size()) != nBytes) {
                GEONKICK_LOG_ERROR("error on reading the state");
                return kResultFalse;
        }

	if (!geonkickApi) {
	  stateData = std::move(data);
	} else {
	  geonkickApi->setKitState(data);
	  geonkickApi->notifyUpdateGui();
	  geonkickApi->notifyKitUpdated();
	}
        return kResultOk;
}

tresult PLUGIN_API
GKickVstProcessor::getState(IBStream* state)
{
        if (state == nullptr || geonkickApi == nullptr)
                return kResultTrue;

        int32 nBytes = 0;
        auto data = geonkickApi->getKitState()->toJson();
        if (state->write(data.data(), data.size(), &nBytes) == kResultFalse) {
                GEONKICK_LOG_ERROR("error on saving the state");
                return kResultFalse;
        }

        if (static_cast<decltype(nBytes)>(data.size()) != nBytes) {
                GEONKICK_LOG_ERROR("error on saving the state");
                return kResultFalse;
        }
        return kResultOk;
}

IPlugView* PLUGIN_API
GKickVstProcessor::createView(FIDString name)
{
        //        if (geonkickApi && name && std::string(name) == std::string("editor"))
        //                return static_cast<IPlugView*>(new GKickVstEditor(this, geonkickApi.get()));
        return nullptr;
}

tresult PLUGIN_API
GKickVstProcessor::setComponentState(IBStream* state)
{
        return kResultOk;
}

float
GKickVstProcessor::getLastParamValueFromQueue(Steinberg::Vst::IParamValueQueue* queue,
                                                    float fallback)
{
        if (!queue)
                return fallback;

        auto numPoints = queue->getPointCount();
        if (numPoints > 0) {
                Steinberg::int32 sampleOffset = 0;
                Steinberg::Vst::ParamValue value = 0.0;
                if (queue->getPoint(numPoints - 1, sampleOffset, value) == Steinberg::kResultTrue)
                        return static_cast<float>(value);
        }

        return fallback;
}

void GKickVstProcessor::readParameters(const Vst::ProcessData& data)
{
        if (!data.inputParameterChanges)
                return;

        auto numParams = data.inputParameterChanges->getParameterCount();
        for (size_t i = 0; i < numParams; i) {
                auto queue = data.inputParameterChanges->getParameterData(i);
                if (!queue) continue;

                auto id = queue->getParameterId();
                float val = getLastParamValueFromQueue(queue, 0.0f);

                switch (id) {
                case 1000: glitchProb = val; break;
                case 1001: jumpMin = val * 1000.0f; break;
                case 1002: jumpMax = val * 1000.0f; break;
                case 1003: glitchLen = val * 1000.0f; break;
                case 1004: glitchRepeats = std::max(1, static_cast<int>(val * 10)); break;
                }
        }
}
