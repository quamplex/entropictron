/**
 * File name: GlitchModel.cpp
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

#include "GlitchModel.h"
#include "DspGlitchProxy.h"

GlitchModel::GlitchModel(RkObject *parent,
                         DspGlitchProxy* dspGlitchProxy)
        : EntAbstractModel(parent),
          dspGlitchProxy{dspGlitchProxy},
          repeatsDefaultValue{ENT_GLITCH_DEFAULT_REPEATS},
          repeatsRange{ENT_GLITCH_MIN_REPEATS, ENT_GLITCH_MAX_REPEATS},
          probabilityDefaultValue{ENT_GLITCH_DEFAULT_PROB},
          probabilityRange{ENT_GLITCH_MIN_PROB, ENT_GLITCH_MAX_PROB},
          lengthDefaultValue{ENT_GLITCH_DEFAULT_LENGH}, // 50ms
          lengthRange{ENT_GLITCH_MIN_LENGH, ENT_GLITCH_MAX_LENGH}, // [10ms - 2000ms]
          minJumpDefaultValue{ENT_GLITCH_DEFAULT_MIN_JUMP}, // 0ms
          minJumpRange{ENT_GLITCH_MIN_MIN_JUMP, ENT_GLITCH_MAX_MIN_JUMP}, // [0ms - 2000ms]
          maxJumpDefaultValue{ENT_GLITCH_DEFAULT_MAX_JUMP}, // 200 ms
          maxJumpRange{ENT_GLITCH_MIN_MAX_JUMP, ENT_GLITCH_MAX_MAX_JUMP} // [0ms - 2000ms]
{
        RK_ACT_BIND(dspGlitchProxy,
                    enabled,
                    RK_ACT_ARGS(bool b),
                    this, enabled(b));
}

GlitchId GlitchModel::getId() const
{
        return dspGlitchProxy->getGlitchId();
}

void GlitchModel::enable(bool b)
{
        if (dspGlitchProxy->enable(b))
                action enabled(b);
}

bool GlitchModel::isEnabled() const
{
        return dspGlitchProxy->isEnabled();
}

void GlitchModel::setRepeats(int value)
{
        if (dspGlitchProxy->setRepeats(value))
                action repeatsUpdated(value);
}

int GlitchModel::repeats() const
{
        return dspGlitchProxy->repeats();
}

void GlitchModel::setRepeatsDefaultValue(int value)
{
        repeatsDefaultValue = value;
}

int GlitchModel::getRepeatsDefaultValue() const
{
        return repeatsDefaultValue;
}

void GlitchModel::setRepeatsRange(int from, int to)
{
        repeatsRange = {from, to};
}

std::pair<int, int> GlitchModel::getRepeatsRange() const
{
        return repeatsRange;
}

void GlitchModel::setProbability(double value)
{
        if (dspGlitchProxy->setProbability(value))
                action probabilityUpdated(value);
}

double GlitchModel::probability() const
{
        return dspGlitchProxy->probability();
}

void GlitchModel::setProbabilityDefaultValue(double value)
{
        probabilityDefaultValue = value;
}

double GlitchModel::getProbabilityDefaultValue() const
{
        return probabilityDefaultValue;
}

void GlitchModel::setProbabilityRange(double from, double to)
{
        probabilityRange = {from, to};
}

std::pair<double, double> GlitchModel::getProbabilityRange() const
{
        return probabilityRange;
}

void GlitchModel::setLength(double value)
{
        if (dspGlitchProxy->setLength(value))
                action lengthUpdated(value);
}

double GlitchModel::length() const
{
        return dspGlitchProxy->length();
}

void GlitchModel::setLengthDefaultValue(double value)
{
        lengthDefaultValue = value;
}

double GlitchModel::getLengthDefaultValue() const
{
        return lengthDefaultValue;
}

void GlitchModel::setLengthRange(double from, double to)
{
        lengthRange = {from, to};
}

std::pair<double, double> GlitchModel::getLengthRange() const
{
        return lengthRange;
}

void GlitchModel::setMaxJump(double value)
{
        if (dspGlitchProxy->setMaxJump(value))
                action maxJumpUpdated(value);
}

double GlitchModel::maxJump() const
{
        return dspGlitchProxy->maxJump();
}

void GlitchModel::setMaxJumpDefaultValue(double value)
{
        maxJumpDefaultValue = value;
}

double GlitchModel::getMaxJumpDefaultValue() const
{
        return maxJumpDefaultValue;
}

void GlitchModel::setMaxJumpRange(double from, double to)
{
        maxJumpRange = {from, to};
}

std::pair<double, double> GlitchModel::getMaxJumpRange() const
{
        return maxJumpRange;
}

void GlitchModel::setMinJump(double value)
{
        if (dspGlitchProxy->setMinJump(value))
                action minJumpUpdated(value);
}

double GlitchModel::minJump() const
{
        return dspGlitchProxy->minJump();
}

void GlitchModel::setMinJumpDefaultValue(double value)
{
        minJumpDefaultValue = value;
}

double GlitchModel::getMinJumpDefaultValue() const
{
        return minJumpDefaultValue;
}

void GlitchModel::setMinJumpRange(double from, double to)
{
        minJumpRange = {from, to};
}

std::pair<double, double> GlitchModel::getMinJumpRange() const
{
        return minJumpRange;
}
