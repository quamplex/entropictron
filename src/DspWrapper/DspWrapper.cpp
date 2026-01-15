/**
 * File name: DspWrapper.cpp
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

#include "DspWrapper.h"
#include "entropictron.h"
#include "DspFrameTimer.h"
#include "DspWrapperNoise.h"
#include "DspWrapperCrackle.h"
#include "DspWrapperGlitch.h"
#include "DspWrapperPitch.h"
#include "ent_state.h"

DspWrapper::DspWrapper()
        : frameTimer{std::make_unique<DspFrameTimer>()}
{
        // Create DSP
        struct entropictron* dsp = nullptr;
        ent_create(&dsp, ENT_DEFAULT_SAMPLE_RATE);
        entropictronDsp = std::unique_ptr<struct entropictron, DspDeleter>(dsp);

        // Noise 1
        auto noise = ent_get_noise(entropictronDsp.get(),
                                   static_cast<int>(NoiseId::Noise1));
        dspNoise1 = std::make_unique<DspWrapperNoise>(noise);

        // Noise 2
        noise = ent_get_noise(entropictronDsp.get(),
                              static_cast<int>(NoiseId::Noise2));
        dspNoise2 = std::make_unique<DspWrapperNoise>(noise);

        // Crackle 1
        auto crackle = ent_get_crackle(entropictronDsp.get(),
                                       static_cast<int>(CrackleId::Crackle1));
        dspCrackle1 = std::make_unique<DspWrapperCrackle>(crackle);

        // Crackle 2
        crackle = ent_get_crackle(entropictronDsp.get(),
                                  static_cast<int>(CrackleId::Crackle2));
        dspCrackle2 = std::make_unique<DspWrapperCrackle>(crackle);

        // Glitch 1
        auto glitch = ent_get_glitch(entropictronDsp.get(),
                                     static_cast<int>(GlitchId::Glitch1));
        dspGlitch1 = std::make_unique<DspWrapperGlitch>(glitch);

        // Glitch 2
        glitch = ent_get_glitch(entropictronDsp.get(),
                                static_cast<int>(GlitchId::Glitch2));
        dspGlitch2 = std::make_unique<DspWrapperGlitch>(glitch);

        // Pitch
        auto pitch = ent_get_pitch(entropictronDsp.get());
        dspPitch = std::make_unique<DspWrapperPitch>(pitch);

        // Set 50ms timeout.
        frameTimer->setTimeout(getSampleRate() * 0.05);
}

void DspWrapper::setSampleRate(unsigned int srate)
{
        ent_set_sample_rate(entropictronDsp.get(), srate);
        // Set 50ms timeout.
        frameTimer->setTimeout(srate * 0.05);
}

int DspWrapper::getSampleRate() const
{
        unsigned int sampleRate = Entropictron::defaultSampleRate;
        ent_get_sample_rate(entropictronDsp.get(), &sampleRate);

        return sampleRate;
}

void DspWrapper::setState(const struct ent_state *state)
{
        ent_set_state(entropictronDsp.get(), state);
}

void DspWrapper::getState(struct ent_state *state) const
{
        ent_get_state(entropictronDsp.get(), state);
}

void DspWrapper::setPlayMode(PlayMode mode)
{
        ent_set_play_mode(entropictronDsp.get(),
                          static_cast<enum ent_play_mode>(mode));
}

PlayMode DspWrapper::playMode() const
{
        return static_cast<PlayMode>(ent_get_play_mode(entropictronDsp.get()));
}

void DspWrapper::setEntropyRate(double rate)
{
        ent_set_entropy_rate(entropictronDsp.get(), rate);
}

double DspWrapper::getEntropyRate() const
{
        return ent_get_entropy_rate(entropictronDsp.get());
}

void DspWrapper::setEntropyDepth(double depth)
{
        ent_set_entropy_depth(entropictronDsp.get(), depth);
}

double DspWrapper::getEntropyDepth() const
{
        return ent_get_entropy_depth(entropictronDsp.get());
}

double DspWrapper::getEntropy() const
{
        return ent_get_entropy(entropictronDsp.get());
}

void DspWrapper::updateEntropy()
{
        ent_update_entropy(entropictronDsp.get());
}

void DspWrapper::process(float** data, size_t size)
{
        ent_process(entropictronDsp.get(), data, size);
}

void DspWrapper::pressKey(bool on, int pitch, int velocity)
{
        ent_press_key(entropictronDsp.get(), on, pitch, velocity);
}


DspWrapperNoise* DspWrapper::getNoise(NoiseId id) const
{
        if (id == NoiseId::Noise1)
                return dspNoise1.get();
        else
                return dspNoise2.get();
}

DspWrapperCrackle* DspWrapper::getCrackle(CrackleId id) const
{
        if (id == CrackleId::Crackle1)
                return dspCrackle1.get();
        else
                return dspCrackle2.get();
}

DspWrapperGlitch* DspWrapper::getGlitch(GlitchId id) const
{
        if (id == GlitchId::Glitch1)
                return dspGlitch1.get();
        else
                return dspGlitch2.get();
}

DspWrapperPitch* DspWrapper::getPitch() const
{
        return dspPitch.get();
}

DspFrameTimer* DspWrapper::getFrameTimer() const
{
        return frameTimer.get();
}


