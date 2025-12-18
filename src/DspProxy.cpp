/**
 * File name: DspProxy.cpp
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

#include "DspProxy.h"
#include "DspNoiseProxy.h"
#include "EntState.h"

DspProxy::DspProxy(RkObject *parent)
        : RkObject(parent)
{
}

void DspProxy::setState(std::unique_ptr<EntState> state) const
{
        // Noise state
        for (int i = 0; i < static_cast<int>(NoiseId::Noise2); i++) {
                auto noise = getNoise(static_cast<NoiseId>(i));
                auto noiseState = state->getNoise(static_cast<NoiseId>(i));
                noise->enable(noiseState.enabled);
                noise->setType(static_cast<NoiseType>(noiseState.type));
                noise->setDensity(noiseState.density);
                noise->setBrightness(noiseState.brightness);
                noise->setGain(noiseState.gain);
                noise->setStereo(noiseState.stereo);
                noise->setFilterType(static_cast<FilterType>(noiseState.filter_type));
                noise->setCutOff(noiseState.cutoff);
                noise->setResonance(noiseState.resonance);
        }
}

std::unique_ptr<EntState> DspProxy::getState() const
{
        auto state = std::make_unique<EntState>();

        // Noise state
        for (int i = 0; i < static_cast<int>(NoiseId::Noise2); i++) {
                auto& noiseState = state->getNoise(static_cast<NoiseId>(i));
                auto noise = getNoise(static_cast<NoiseId>(i));
                noiseState.enabled = noise->isEnabled();
                noiseState.type = static_cast<int>(noise->noiseType());
                noiseState.density = noise->density();
                noiseState.brightness = noise->brightness();
                noiseState.gain = noise->gain();
                noiseState.stereo = noise->stereo();
                noiseState.filter_type = static_cast<int>(noise->filterType());
                noiseState.cutoff = noise->cutOff();
                noiseState.resonance = noise->resonance();
        }

        return state;
}

