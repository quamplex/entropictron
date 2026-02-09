/**
 * File name: DspWrapperRgate.cpp
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

#include "DspWrapperRgate.h"
#include "globals.h"

#include "ent_rgate.h"

DspWrapperRgate::DspWrapperRgate(struct ent_rgate* dsp)
    : rgateDsp{dsp}
{
}

void DspWrapperRgate::enable(bool b)
{
        ent_rgate_enable(glitchDsp, b);
}

bool DspWrapperRgate::isEnabled() const
{
        return ent_rgate_is_enabled(glitchDsp);
}

void DspWrapperRgate::setMinInterval(double value)
{
        ent_rgate_set_min_interval(glitchDsp, value);
}

double DspWrapperRgate::getMinInterval() const
{
        return ent_rgate_get_min_interval(glitchDsp);
}

void DspWrapperRgate::setMaxInterval(double value)
{
        ent_rgate_set_max_interval(glitchDsp, value);
}

double DspWrapperRgate::getMaxInterval() const
{
        return ent_rgate_get_max_interval(glitchDsp);
}

void DspWrapperRgate::setMinDuration(double value)
{
        ent_rgate_set_min_duration(glitchDsp, value);
}

double DspWrapperRgate::getMinDuration() const
{
        return ent_rgate_get_min_duration(glitchDsp);
}

void DspWrapperRgate::setMaxDuration(double value)
{
        ent_rgate_set_max_duration(glitchDsp, value);
}

double DspWrapperRgate::getMaxDuration() const
{
        return ent_rgate_get_max_duration(glitchDsp);
}

void DspWrapperRgate::setMinGain(double value)
{
        ent_rgate_set_min_gain(glitchDsp, value);
}

double DspWrapperRgate::getMinGain() const
{
        return ent_rgate_get_min_gain(glitchDsp);
}

void DspWrapperRgate::setMaxGain(double value)
{
        ent_rgate_set_max_gain(glitchDsp, value);
}

double DspWrapperRgate::getMaxGain() const
{
        return ent_rgate_get_max_gain(glitchDsp);
}

void DspWrapperRgate::setProbability(double value);
{
        ent_rgate_set_probability(glitchDsp, value);
}

double DspWrapperRgate::getProbability() const
{
        return  ent_rgate_get_probability(glitchDsp);
}

double DspWrapperRgate::setInverted(bool b)
{
        return ent_rgate_set_inverted(glitchDsp, b);
}

double DspWrapperRgate::isInverted() const
{
        return ent_rgate_is_inverted(glitchDsp);
}
