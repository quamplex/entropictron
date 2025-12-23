/**
 * File name: DspWrapperCrackle.cpp
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

#include "DspWrapperCrackle.h"
#include "globals.h"

#include "ent_crackle.h"

DspWrapperCrackle::DspWrapperCrackle(struct ent_crackle* dsp)
    : crackleDsp{ dsp }
{
}

void DspWrapperCrackle::enable(bool b)
{
    ent_crackle_enable(crackleDsp, b);
}

bool DspWrapperCrackle::isEnabled() const
{
    return ent_crackle_is_enabled(crackleDsp);
}

void DspWrapperCrackle::setRate(double value)
{
    ent_crackle_set_rate(crackleDsp, value);
}

double DspWrapperCrackle::rate() const
{
    return ent_crackle_get_rate(crackleDsp);
}

void DspWrapperCrackle::setDuration(double value)
{
    ent_crackle_set_duration(crackleDsp, value);
}

double DspWrapperCrackle::duration() const
{
    return ent_crackle_get_duration(crackleDsp);
}

void DspWrapperCrackle::setAmplitude(double value)
{
    ent_crackle_set_amplitude(crackleDsp, value);
}

double DspWrapperCrackle::amplitude() const
{
    return ent_crackle_get_amplitude(crackleDsp);
}

void DspWrapperCrackle::setRandomness(double value)
{
    ent_crackle_set_randomness(crackleDsp, value);
}

double DspWrapperCrackle::randomness() const
{
    return ent_crackle_get_randomness(crackleDsp);
}

void DspWrapperCrackle::setBrightness(double value)
{
    ent_crackle_set_brightness(crackleDsp, value);
}

double DspWrapperCrackle::brightness() const
{
    return ent_crackle_get_brightness(crackleDsp);
}

void DspWrapperCrackle::setEnvelopeShape(CrackleEnvelopeShape shape)
{
        auto value = static_cast<enum ent_crackle_envelope>(shape);
        ent_crackle_set_envelope_shape(crackleDsp, value);
}

CrackleEnvelopeShape DspWrapperCrackle::envelopeShape() const
{
        return static_cast<CrackleEnvelopeShape>(ent_crackle_get_envelope_shape(crackleDsp));
}

void DspWrapperCrackle::setStereoSpread(double value)
{
    ent_crackle_set_stereo_spread(crackleDsp, value);
}

double DspWrapperCrackle::stereoSpread() const
{
    return ent_crackle_get_stereo_spread(crackleDsp);
}
