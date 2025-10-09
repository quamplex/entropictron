/**
 * File name: DspWrapperGlitcher.cpp
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

#include "DspWrapperGlitcher.h"
#include "globals.h"

#include "ent_glitcher.h"

DspWrapperGlitcher::DspWrapperGlitcher(struct ent_glitcher* dsp)
    : glitcherDsp{dsp}
{
}

void DspWrapperGlitcher::enable(bool b)
{
    ENTROPICTRON_LOG_DEBUG("called: " << b);
    ent_glitcher_enable(glitcherDsp, b);
}

bool DspWrapperGlitcher::isEnabled() const
{
    return ent_glitcher_is_enabled(glitcherDsp);
}

void DspWrapperGlitcher::setProbability(double value)
{
    ENTROPICTRON_LOG_DEBUG("called: " << value);
    ent_glitcher_set_probability(glitcherDsp, value);
}

double DspWrapperGlitcher::probability() const
{
    return ent_glitcher_get_probability(glitcherDsp);
}

void DspWrapperGlitcher::setJumpMin(double value)
{
    ENTROPICTRON_LOG_DEBUG("called: " << value);
    ent_glitcher_set_jump_min(glitcherDsp, value);
}

double DspWrapperGlitcher::jumpMin() const
{
    return ent_glitcher_get_jump_min(glitcherDsp);
}

void DspWrapperGlitcher::setJumpMax(double value)
{
    ENTROPICTRON_LOG_DEBUG("called: " << value);
    ent_glitcher_set_jump_max(glitcherDsp, value);
}

double DspWrapperGlitcher::jumpMax() const
{
    return ent_glitcher_get_jump_max(glitcherDsp);
}

void DspWrapperGlitcher::setLength(double value)
{
    ENTROPICTRON_LOG_DEBUG("called: " << value);
    ent_glitcher_set_length(glitcherDsp, value);
}

double DspWrapperGlitcher::length() const
{
    return ent_glitcher_get_length(glitcherDsp);
}

void DspWrapperGlitcher::setRepeatCount(int value)
{
    ENTROPICTRON_LOG_DEBUG("called: " << value);
    ent_glitcher_set_repeat_count(glitcherDsp, value);
}

int DspWrapperGlitcher::repeatCount() const
{
    return ent_glitcher_get_repeat_count(glitcherDsp);
}
