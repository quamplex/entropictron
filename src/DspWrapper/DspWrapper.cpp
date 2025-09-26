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
#include "DspWrapperNoise.h"

DspWrapper::DspWrapper()
{
        entropictron_create(&entropictronDsp, ENTROPICTRON_DEFAULT_SAMPLE_RATE);
        dspNoise1 = std::make_unique<DspWrapperNoise>(entropictronDsp);
        dspNoise2 = std::make_unique<DspWrapperNoise>(entropictronDsp);
}

DspWrapper::~DspWrapper()
{
        if (entropictronDsp)
                entropictron_free(&entropictronDsp);
}

void DspWrapper::setSampleRate(unsigned int srate)
{
}

int DspWrapper::getSampleRate() const
{
        int sampleRate = ENTROPICTRON_DEFAULT_SAMPLE_RATE;
        if (entropictronDsp)
                entropictron_get_sample_rate(entropictronDsp, &sampleRate);

        return sampleRate;
}

void DspWrapper::process(float** data, size_t size)
{
}

DspWrapperNoise* DspWrapper::getNoise(NoiseId id) const
{
        if (id == NoiseId::Noise1)
                return dspNoise1.get();
        else
                return dspNoise2.get();
}

