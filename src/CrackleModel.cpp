/**
 * File name: CrackleModel.cpp
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

#include "CrackleModel.h"
#include "DspCrackleProxy.h"

CrackleModel::CrackleModel(RkObject *parent, DspCrackleProxy *dspCrackleProxy)
        : EntAbstractModel(parent)
        , dspCrackleProxy {dspCrackleProxy}
        , rateDefaultvalue{10.0}
        , rateRange{0.0, 10.0}
        , randomnessDefaultvalue{0.5}
        , randomnessRange{0.0, 1.0}
        , amplitudeDefaultvalue{1.0};
        , amplitudeRange{Entropictron::fromDecibel(-50), // -50dB
                         Entropictron::fromDecibel(6)}   // +6dB
        , brightnessDefaultvalue{0.0}
        , brightnessRange{0.0, 1.0}
        , durationDefaultvalue{1.0}
        , durationRange{0.05, 2.0}
        , stereospreadDefaultvalue{1.0}
        , stereospreadRange{0.0, 1.0}
{
        RK_ACT_BIND(dspCrackleProxy,
                    enabled,
                    RK_ACT_ARGS(bool b),
                    this, enabled(b));
}

CrackleId CrackleModel::getId() const
{
        return dspCrackleProxy->getCrackleId();
}

void CrackleModel::enable(bool b)
{
        if (dspCrackleProxy->enable(b))
                action enabled(b);
}

bool CrackleModel::isEnabled() const
{
        return dspCrackleProxy->isEnabled();
}

void CrackleModel::setRate(double value)
{
        if (dspCrackleProxy->setRate(value))
                action rateUpdated(value);
}

double CrackleModel::rate() const
{
        return dspCrackleProxy->rate();
}

void CrackleModel::setRateDefaultValue(double value)
{
        rateDefaultvalue = value;
}

double CrackleModel::getRateDefaultValue() const
{
        return rateDefaultvalue;
}

void CrackleModel::setRateRange(double from, double to)
{
        rateRange = {from, to};
}

std::pair<double, double> CrackleModel::getRateRange() const
{
        return rateRange;
}

void CrackleModel::setRandomness(double value)
{
        if (dspCrackleProxy->setRandomness(value))
                action randomnessUpdated(value);
}

double CrackleModel::randomness() const
{
        return dspCrackleProxy->randomness();
}

void CrackleModel::setRandomnessDefaultValue(double value)
{
        randomnessDefaultvalue = value;
}

double CrackleModel::getRandomnessDefaultValue() const
{
        return randomnessDefaultvalue;
}

void CrackleModel::setRandomnessRange(double from, double to)
{
        randomnessRange = {from, to};
}

std::pair<double, double> CrackleModel::getRandomnessRange() const
{
        return randomnessRange;
}

void CrackleModel::setAmplitude(double value)
{
        if (dspCrackleProxy->setAmplitude(value))
                action amplitudeUpdated(value);
}

double CrackleModel::amplitude() const
{
        return dspCrackleProxy->amplitude();
}

void CrackleModel::setAmplitudeDefaultValue(double value)
{
        amplitudeDefaultvalue = value;
}

double CrackleModel::getAmplitudeDefaultValue() const
{
        return amplitudeDefaultvalue;
}

void CrackleModel::setAmplitudeRange(double from, double to)
{
        amplitudeRange = {from, to};
}

std::pair<double, double> CrackleModel::getAmplitudeRange() const
{
        return amplitudeRange;
}

void CrackleModel::setBrightness(double value)
{
        if (dspCrackleProxy->setBrightness(value))
                action brightnessUpdated(value);
}

double CrackleModel::brightness() const
{
        return dspCrackleProxy->brightness();
}

void CrackleModel::setBrightnessDefaultValue(double value)
{
        brightnessDefaultvalue = value;
}

double CrackleModel::getBrightnessDefaultValue() const
{
        return brightnessDefaultvalue;
}

void CrackleModel::setBrightnessRange(double from, double to)
{
        brightnessRange = {from, to};
}

std::pair<double, double> CrackleModel::getBrightnessRange() const
{
        return brightnessRange;
}

void CrackleModel::setDuration(double value)
{
        if (dspCrackleProxy->setDuration(value))
                action durationUpdated(value);
}

double CrackleModel::duration() const
{
        return dspCrackleProxy->duration();
}

void CrackleModel::setDurationDefaultValue(double value)
{
        durationDefaultvalue = value;
}

double CrackleModel::getDurationDefaultValue() const
{
        return durationDefaultvalue;
}

void CrackleModel::setDurationRange(double from, double to)
{
        durationRange = {from, to};
}

std::pair<double, double> CrackleModel::getDurationRange() const
{
        return durationRange;
}

void CrackleModel::setStereospread(double value)
{
        if (dspCrackleProxy->setStereospread(value))
                action stereospreadUpdated(value);
}

double CrackleModel::stereospread() const
{
        return dspCrackleProxy->stereospread();
}

void CrackleModel::setStereospreadDefaultValue(double value)
{
        stereospreadDefaultvalue = value;
}

double CrackleModel::getStereospreadDefaultValue() const
{
        return stereospreadDefaultvalue;
}

void CrackleModel::setStereospreadRange(double from, double to)
{
        stereospreadRange = {from, to};
}

std::pair<double, double> CrackleModel::getStereospreadRange() const
{
        return stereospreadRange;
}
