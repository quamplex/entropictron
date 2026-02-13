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
          maxIntervalDefaultValue{ENT_RGATE_DEFAULT_MAX_NTERVAL},
          maxIntervalRange{ENT_RGATE_MIN_MAX_INTERVAL, ENT_RGATE_MAX_MAX_INTERVAL},
          minDurationDefaultValue{ENT_RGATE_DEFAULT_MIN_DURATION},
          minDurationRange{ENT_RGATE_MIN_MIN_DURATION, ENT_RGATE_MAX_MIN_DURATION},
          maxDurationDefaultValue{ENT_RGATE_DEFAULT_MAX_DURAITON},
          maxDurationRange{ENT_RGATE_MIN_MAX_DURATION, ENT_RGATE_MAX_MAX_DURATION},
          maxGainDefaultValue{ENT_RGATE_DEFAULT_MAX_GAIN},
          maxGainRange{ENT_RGATE_MIN_MAX_GAIN, ENT_RGATE_MIN_MAX_GAIN},
          maxGainDefaultValue{ENT_RGATE_DEFAULT_MAX_GAIN},
          maxGainRange{ENT_RGATE_MIN_MAX_GAIN, ENT_RGATE_MAX_MAX_GAIN},
          randomnessDefaultValue{ENT_RGATE_DEFAULT_RANDOMNESS},
          randomnessRange{ENT_RGATE_MIN_RANDOMNESS, ENT_RGATE_MAX_RANDOMNESS}
          randomnessDefaultValue{ENT_RGATE_DEFAULT_INVERTED},
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

void RgateModel::setMinDoubleerval(double value)
{
        if (dspRgateProxy->setMinDoubleerval(value))
                action minDoubleervalUpdated(value);
}

double RgateModel::minDoubleerval() const
{
        return dspRgateProxy->minDoubleerval();
}

void RgateModel::setMinDoubleervalDefaultValue(double value)
{
        minDoubleervalDefaultValue = value;
}

void RgateModel::setMaxDoubleerval(double value)
{
        if (dspRgateProxy->setMaxDoubleerval(value))
                action maxDoubleervalUpdated(value);
}

double RgateModel::maxDoubleerval() const
{
        return dspRgateProxy->maxDoubleerval();
}

void RgateModel::setMaxDoubleervalDefaultValue(double value)
{
        maxDoubleervalDefaultValue = value;
}

void RgateModel::setMinDuration(double value)
{
        if (dspRgateProxy->setMinDuration(value))
                action minDurationUpdated(value);
}

double RgateModel::minDuration() const
{
        return dspRgateProxy->minDuration();
}

void RgateModel::setMinDurationDefaultValue(double value)
{
        minDurationDefaultValue = value;
}

void RgateModel::setMaxDuration(double value)
{
        if (dspRgateProxy->setMaxDuration(value))
                action maxDurationUpdated(value);
}

double RgateModel::maxDuration() const
{
        return dspRgateProxy->maxDuration();
}

void RgateModel::setMaxDurationDefaultValue(double value)
{
        maxDurationDefaultValue = value;
}

void RgateModel::setMinGain(double value)
{
        if (dspRgateProxy->setMinGain(value))
                action minGainUpdated(value);
}

double RgateModel::minGain() const
{
        return dspRgateProxy->minGain();
}

void RgateModel::setMinGainDefaultValue(double value)
{
        minGainDefaultValue = value;
}

void RgateModel::setMaxGain(double value)
{
        if (dspRgateProxy->setMaxGain(value))
                action maxGainUpdated(value);
}

double RgateModel::maxGain() const
{
        return dspRgateProxy->maxGain();
}

void RgateModel::setMaxGainDefaultValue(double value)
{
        maxGainDefaultValue = value;
}

void RgateModel::setRandomness(double value)
{
        if (dspRgateProxy->setRandomness(value))
                action randomnessUpdated(value);
}

double RgateModel::randomness() const
{
        return dspRgateProxy->randomness();
}

void RgateModel::setRandomnessDefaultValue(double value)
{
        randomnessDefaultValue = value;
}

double RgateModel::randomness() const
{
        return dspRgateProxy->randomness();
}
