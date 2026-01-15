/**
 * File name: DspWrapperPitch.cpp
 * Project: Entropictron (A texture synthesizer)
 *
 * Copyright (C) 2026 Iurie Nistor
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

#include "DspWrapperPitch.h"
#include "globals.h"

#include "ent_pitch.h"

DspWrapperPitch::DspWrapperPitch(struct ent_pitch* dsp)
    : pitchDsp{dsp}
{
}

void DspWrapperPitch::enable(bool b)
{
    ent_pitch_enable(pitchDsp, b);
}

bool DspWrapperPitch::isEnabled() const
{
    return ent_pitch_is_enabled(pitchDsp);
}

void DspWrapperPitch::setPitch(double pitch)
{
        ent_pitch_set_pitch(pitchDsp, pitch);
}

double DspWrapperPitch::getPitch(double pitch) const
{
        return  ent_pitch_get_pitch(pitchDsp);
}

void DspWrapperPitch::setFine(double fine)
{
        ent_pitch_set_fine(pitchDsp, fine);
}

double DspWrapperPitch::getFine() const
{
        return ent_pitch_get_fine(pitchDsp);
}

void DspWrapperPitch::setDepth(double depth)
{
        ent_pitch_set_depth(pitchDsp, depth);
}

double DspWrapperPitch::getDepth() const
{
        return ent_pitch_get_depth(pitchDsp);
}

void DspWrapperPitch::setDrift(double drift)
{
        ent_pitch_set_drift(pitchDsp, drift);
}

double DspWrapperPitch::getDrift() const
{
        return ent_pitch_get_drift(pitchDsp);
}
