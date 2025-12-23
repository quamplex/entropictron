/**
 * File name: DspWrapperGlitch.cpp
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

#include "DspWrapperGlitch.h"
#include "globals.h"

#include "ent_glitch.h"

DspWrapperGlitch::DspWrapperGlitch(struct ent_glitch* dsp)
    : glitchDsp{dsp}
{
}

void DspWrapperGlitch::enable(bool b)
{
    ent_glitch_enable(glitchDsp, b);
}

bool DspWrapperGlitch::isEnabled() const
{
    return ent_glitch_is_enabled(glitchDsp);
}

void DspWrapperGlitch::setProbability(double value)
{
    ent_glitch_set_probability(glitchDsp, value);
}

double DspWrapperGlitch::probability() const
{
    return ent_glitch_get_probability(glitchDsp);
}

void DspWrapperGlitch::setJumpMin(double value)
{
    ent_glitch_set_jump_min(glitchDsp, value);
}

double DspWrapperGlitch::jumpMin() const
{
    return ent_glitch_get_jump_min(glitchDsp);
}

void DspWrapperGlitch::setJumpMax(double value)
{
    ent_glitch_set_jump_max(glitchDsp, value);
}

double DspWrapperGlitch::jumpMax() const
{
    return ent_glitch_get_jump_max(glitchDsp);
}

void DspWrapperGlitch::setLength(double value)
{
    ent_glitch_set_length(glitchDsp, value);
}

double DspWrapperGlitch::length() const
{
    return ent_glitch_get_length(glitchDsp);
}

void DspWrapperGlitch::setRepeatCount(int value)
{
    ent_glitch_set_repeat_count(glitchDsp, value);
}

int DspWrapperGlitch::repeatCount() const
{
    return ent_glitch_get_repeat_count(glitchDsp);
}
