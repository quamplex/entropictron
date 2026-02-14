/**
 * File name: RgateModel.cpp
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

#include "RgateModel.h"
#include "DspRgateProxy.h"

RgateModel::RgateModel(RkObject *parent,
                         DspRgateProxy* dspRgateProxy)
        : EntAbstractModel(parent),
          dspRgateProxy{dspRgateProxy},
          minIntervalDefaultValue{ENT_RGATE_DEFAULT_MIN_INTERVAL},
          minIntervalRange{ENT_RGATE_MIN_MIN_INTERVAL, ENT_RGATE_MAX_MIN_INTERVAL},
          maxIntervalDefaultValue{ENT_RGATE_DEFAULT_MAX_INTERVAL},
          maxIntervalRange{ENT_RGATE_MIN_MAX_INTERVAL, ENT_RGATE_MAX_MAX_INTERVAL},
          minDurationDefaultValue{ENT_RGATE_DEFAULT_MIN_DURATION},
          minDurationRange{ENT_RGATE_MIN_MIN_DURATION, ENT_RGATE_MAX_MIN_DURATION},
          maxDurationDefaultValue{ENT_RGATE_DEFAULT_MAX_DURATION},
          maxDurationRange{ENT_RGATE_MIN_MAX_DURATION, ENT_RGATE_MAX_MAX_DURATION},
          minGainDefaultValue{ENT_RGATE_DEFAULT_MIN_GAIN},
          minGainRange{ENT_RGATE_MIN_MIN_GAIN, ENT_RGATE_MAX_MIN_GAIN},
          maxGainDefaultValue{ENT_RGATE_DEFAULT_MAX_GAIN},
          maxGainRange{ENT_RGATE_MIN_MAX_GAIN, ENT_RGATE_MAX_MAX_GAIN},
          randomnessDefaultValue{ENT_RGATE_DEFAULT_RANDOMNESS},
          randomnessRange{ENT_RGATE_MIN_RANDOMNESS, ENT_RGATE_MAX_RANDOMNESS},
          invertedDefaultValue{ENT_RGATE_DEFAULT_INVERTED}
{
        RK_ACT_BIND(dspRgateProxy,
                    enabled,
                    RK_ACT_ARGS(bool b),
                    this, enabled(b));
        RK_ACT_BIND(dspRgateProxy,
                    minIntervalUpdated,
                    RK_ACT_ARGS(double value),
                    this, minIntervalUpdated(value));
        RK_ACT_BIND(dspRgateProxy,
                    maxIntervalUpdated,
                    RK_ACT_ARGS(double value),
                    this, maxIntervalUpdated(value));
        RK_ACT_BIND(dspRgateProxy,
                    minDurationUpdated,
                    RK_ACT_ARGS(double value),
                    this, minDurationUpdated(value));
        RK_ACT_BIND(dspRgateProxy,
                    maxDurationUpdated,
                    RK_ACT_ARGS(double value),
                    this, maxDurationUpdated(value));
        RK_ACT_BIND(dspRgateProxy,
                    minGainUpdated,
                    RK_ACT_ARGS(double value),
                    this, minGainUpdated(value));
        RK_ACT_BIND(dspRgateProxy,
                    maxGainUpdated,
                    RK_ACT_ARGS(double value),
                    this, maxGainUpdated(value));
        RK_ACT_BIND(dspRgateProxy,
                    minGainUpdated,
                    RK_ACT_ARGS(double value),
                    this, minGainUpdated(value));
        RK_ACT_BIND(dspRgateProxy,
                    maxGainUpdated,
                    RK_ACT_ARGS(double value),
                    this, maxGainUpdated(value));
        RK_ACT_BIND(dspRgateProxy,
                    randomnessUpdated,
                    RK_ACT_ARGS(double value),
                    this, randomnessUpdated(value));
        RK_ACT_BIND(dspRgateProxy,
                    invertedUpdated,
                    RK_ACT_ARGS(bool value),
                    this, invertedUpdated(value));
}

void RgateModel::enable(bool b)
{
        if (dspRgateProxy->enable(b))
                action enabled(b);
}

bool RgateModel::isEnabled() const
{
        return dspRgateProxy->isEnabled();
}

void RgateModel::setMinInterval(double value)
{
        if (dspRgateProxy->setMinInterval(value))
                action minIntervalUpdated(value);
}

double RgateModel::minInterval() const
{
        return dspRgateProxy->getMinInterval();
}

void RgateModel::setMinIntervalDefaultValue(double value)
{
        minIntervalDefaultValue = value;
}

double RgateModel::getMinIntervalDefaultValue() const
{
        return minIntervalDefaultValue;
}

void RgateModel::setMinIntervalRange(double from, double to)
{
        minIntervalRange = {from, to};
}

std::pair<double, double> RgateModel::getMinIntervalRange() const
{
        return minIntervalRange;
}

void RgateModel::setMaxInterval(double value)
{
        if (dspRgateProxy->setMaxInterval(value))
                action maxIntervalUpdated(value);
}

double RgateModel::maxInterval() const
{
        return dspRgateProxy->getMaxInterval();
}

void RgateModel::setMaxIntervalDefaultValue(double value)
{
        maxIntervalDefaultValue = value;
}

double RgateModel::getMaxIntervalDefaultValue() const
{
        return maxIntervalDefaultValue;
}

void RgateModel::setMaxIntervalRange(double from, double to)
{
        maxIntervalRange = {from, to};
}

std::pair<double, double> RgateModel::getMaxIntervalRange() const
{
        return maxIntervalRange;
}

void RgateModel::setMinDuration(double value)
{
        if (dspRgateProxy->setMinDuration(value))
                action minDurationUpdated(value);
}

double RgateModel::minDuration() const
{
        return dspRgateProxy->getMaxDuration();
}

void RgateModel::setMinDurationDefaultValue(double value)
{
        minDurationDefaultValue = value;
}

double RgateModel::getMinDurationDefaultValue() const
{
        return minDurationDefaultValue;
}

void RgateModel::setMinDurationRange(double from, double to)
{
        minDurationRange = {from, to};
}

std::pair<double, double> RgateModel::getMinDurationRange() const
{
        return minDurationRange;
}

void RgateModel::setMaxDuration(double value)
{
        if (dspRgateProxy->setMaxDuration(value))
                action maxDurationUpdated(value);
}

double RgateModel::maxDuration() const
{
        return dspRgateProxy->getMaxDuration();
}

void RgateModel::setMaxDurationDefaultValue(double value)
{
        maxDurationDefaultValue = value;
}

double RgateModel::getMaxDurationDefaultValue() const
{
        return maxDurationDefaultValue;
}

void RgateModel::setMaxDurationRange(double from, double to)
{
        maxDurationRange = {from, to};
}

std::pair<double, double> RgateModel::getMaxDurationRange() const
{
        return maxDurationRange;
}

void RgateModel::setMinGain(double value)
{
        if (dspRgateProxy->setMinGain(value))
                action minGainUpdated(value);
}

double RgateModel::minGain() const
{
        return dspRgateProxy->getMinGain();
}

void RgateModel::setMinGainDefaultValue(double value)
{
        minGainDefaultValue = value;
}

double RgateModel::getMinGainDefaultValue() const
{
        return minGainDefaultValue;
}

void RgateModel::setMinGainRange(double from, double to)
{
        minGainRange = {from, to};
}

std::pair<double, double> RgateModel::getMinGainRange() const
{
        return minGainRange;
}

void RgateModel::setMaxGain(double value)
{
        if (dspRgateProxy->setMaxGain(value))
                action maxGainUpdated(value);
}

double RgateModel::maxGain() const
{
        return dspRgateProxy->getMaxGain();
}

void RgateModel::setMaxGainDefaultValue(double value)
{
        maxGainDefaultValue = value;
}

double RgateModel::getMaxGainDefaultValue() const
{
        return maxGainDefaultValue;
}

void RgateModel::setMaxGainRange(double from, double to)
{
        maxGainRange = {from, to};
}

std::pair<double, double> RgateModel::getMaxGainRange() const
{
        return maxGainRange;
}

void RgateModel::setRandomness(double value)
{
        if (dspRgateProxy->setRandomness(value))
                action randomnessUpdated(value);
}

double RgateModel::randomness() const
{
        return dspRgateProxy->getRandomness();
}

void RgateModel::setRandomnessDefaultValue(double value)
{
        randomnessDefaultValue = value;
}

double RgateModel::getRandomnessDefaultValue() const
{
        return randomnessDefaultValue;
}

void RgateModel::setRandomnessRange(double from, double to)
{
        randomnessRange = {from, to};
}

std::pair<double, double> RgateModel::getRandomnessRange() const
{
        return randomnessRange;
}

void RgateModel::setInverted(bool value)
{
        if (dspRgateProxy->setInverted(value))
                action invertedUpdated(value);
}

bool RgateModel::isInverted() const
{
        return dspRgateProxy->getInverted();
}
