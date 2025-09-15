/**
 * File name: DspWrapperNoise.cpp
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

#include "DspWrapperNoise.h"

DspWrapperNoise::DspWrapperNoise(struct entropictron *dsp)
        : entropictronDsp {dsp}
{
}

void DspWrapperNoise::enable(bool b)
{
}

bool DspWrapperNoise::isEnabled() const
{
}

void DspWrapperNoise::setType(NoiseType type)
{
}

NoiseType DspWrapperNoise::noiseType() const
{
        return NoiseType::WhiteNoise;
}

void DspWrapperNoise::setDensity(double value)
{
}

double DspWrapperNoise::density() const
{
        return 0;
}

void DspWrapperNoise::setBrightness(double value)
{
}

double DspWrapperNoise::brightness() const
{
        return 0;
}

void DspWrapperNoise::setGain(double value)
{
}

double DspWrapperNoise::gain() const
{
        return 0;
}
