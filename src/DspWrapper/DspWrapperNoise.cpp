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

#include "ent_noise.h"

DspWrapperNoise::DspWrapperNoise(struct ent_noise *dsp)
        : noiseDsp {dsp}
{
}

void DspWrapperNoise::enable(bool b)
{
        ent_noise_enable(noiseDsp, b);
}

bool DspWrapperNoise::isEnabled() const
{
        return ent_noise_is_enabled(noiseDsp);
}

void DspWrapperNoise::setType(NoiseType type)
{
        ent_noise_set_type(noiseDsp, static_cast<enum ent_noise_type>(type));
}

NoiseType DspWrapperNoise::noiseType() const
{
        return static_cast<NoiseType> (ent_noise_set_type(noiseDsp));
}

void DspWrapperNoise::setDensity(double value)
{
        ent_noise_set_density(noiseDsp, value);
}

double DspWrapperNoise::density() const
{
        return ent_noise_get_density(noiseDsp);
}

void DspWrapperNoise::setBrightness(double value)
{
        ent_noise_set_brightness(noiseDsp, value);
}

double DspWrapperNoise::brightness() const
{
        return ent_noise_get_brightness(noiseDsp);
}

void DspWrapperNoise::setGain(double value)
{
        ent_noise_set_gain(noiseDsp, value);
}

double DspWrapperNoise::gain() const
{
        return ent_noise_get_gain(noiseDsp);
}
