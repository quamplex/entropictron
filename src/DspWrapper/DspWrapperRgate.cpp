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
        ent_rgate_enable(rgateDsp, b);
}

bool DspWrapperRgate::isEnabled() const
{
        return ent_rgate_is_enabled(rgateDsp);
}

void DspWrapperRgate::setMinInterval(double value)
{
        ent_rgate_set_min_interval(rgateDsp, value);
}

double DspWrapperRgate::getMinInterval() const
{
        return ent_rgate_get_min_interval(rgateDsp);
}

void DspWrapperRgate::setMaxInterval(double value)
{
        ent_rgate_set_max_interval(rgateDsp, value);
}

double DspWrapperRgate::getMaxInterval() const
{
        return ent_rgate_get_max_interval(rgateDsp);
}

void DspWrapperRgate::setMinDuration(double value)
{
        ent_rgate_set_min_duration(rgateDsp, value);
}

double DspWrapperRgate::getMinDuration() const
{
        return ent_rgate_get_min_duration(rgateDsp);
}

void DspWrapperRgate::setMaxDuration(double value)
{
        ent_rgate_set_max_duration(rgateDsp, value);
}

double DspWrapperRgate::getMaxDuration() const
{
        return ent_rgate_get_max_duration(rgateDsp);
}

void DspWrapperRgate::setMinGain(double value)
{
        ent_rgate_set_min_gain(rgateDsp, value);
}

double DspWrapperRgate::getMinGain() const
{
        return ent_rgate_get_min_gain(rgateDsp);
}

void DspWrapperRgate::setMaxGain(double value)
{
        ent_rgate_set_max_gain(rgateDsp, value);
}

double DspWrapperRgate::getMaxGain() const
{
        return ent_rgate_get_max_gain(rgateDsp);
}

void DspWrapperRgate::setRandomness(double value)
{
        ent_rgate_set_randomness(rgateDsp, value);
}

double DspWrapperRgate::getRandomness() const
{
        return  ent_rgate_get_randomness(rgateDsp);
}

void DspWrapperRgate::setInverted(bool b)
{
        ent_rgate_set_inverted(rgateDsp, b);
}

double DspWrapperRgate::isInverted() const
{
        return ent_rgate_get_inverted(rgateDsp);
}

void DspWrapperRgate::setDrywet(double value)
{
        ent_rgate_set_drywet(rgateDsp, value);
}

double DspWrapperRgate::getDrywet() const
{
        return  ent_rgate_get_drywet(rgateDsp);
}
