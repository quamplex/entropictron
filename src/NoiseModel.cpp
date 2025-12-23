/**
 * File name: NoiseModel.cpp
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

#include "NoiseModel.h"
#include "DspNoiseProxy.h"

NoiseModel::NoiseModel(RkObject *parent, DspNoiseProxy *dspNoiseProxy)
        : EntAbstractModel(parent)
        , dspNoiseProxy {dspNoiseProxy}
        , desnityDefaultValue {1.0}
        , brightnessDefaultValue {0.0}
        , gainDefaultValue {Entropictron::fromDecibel(0)}
        , stereoDefaultValue {1.0}
        , cutOffDefaultValue {800.0}
        , resonanceDefaultValue {0.5}
        , densityRange {0.0, 1.0}
        , brightnessRange {0.0, 1.0}
        , gainRange {Entropictron::fromDecibel(-50), // -50dB
                     Entropictron::fromDecibel(6)}   // +6dB
        , stereoRange {0.0, 1.0}
        , cutOffRange {20, 18000} // Hz
        , resonanceRange {0.0, 1.0}
{
        RK_ACT_BIND(dspNoiseProxy,
                    enabled,
                    RK_ACT_ARGS(bool b),
                    this, enabled(b));
        RK_ACT_BIND(dspNoiseProxy,
                    typeUpdated,
                    RK_ACT_ARGS(NoiseType type),
                    this, typeUpdated(type));
        RK_ACT_BIND(dspNoiseProxy,
                    densityUpdated,
                    RK_ACT_ARGS(double value),
                    this, densityUpdated(value));
        RK_ACT_BIND(dspNoiseProxy,
                    brightnessUpdated,
                    RK_ACT_ARGS(double value),
                    this, brightnessUpdated(value));
        RK_ACT_BIND(dspNoiseProxy,
                    gainUpdated,
                    RK_ACT_ARGS(double value),
                    this, gainUpdated(value));
        RK_ACT_BIND(dspNoiseProxy,
                    stereoUpdated,
                    RK_ACT_ARGS(double value),
                    this, stereoUpdated(value));
        RK_ACT_BIND(dspNoiseProxy,
                    filterEnabled,
                    RK_ACT_ARGS(bool b),
                    this, filterEnabled(b));
        RK_ACT_BIND(dspNoiseProxy,
                    filterTypeUpdated,
                    RK_ACT_ARGS(FilterType type),
                    this, filterTypeUpdated(type));
        RK_ACT_BIND(dspNoiseProxy,
                    cutOffUpdated,
                    RK_ACT_ARGS(double value),
                    this, cutOffUpdated(value));
        RK_ACT_BIND(dspNoiseProxy,
                    resonanceUpdated,
                    RK_ACT_ARGS(double value),
                    this, resonanceUpdated(value));
}

NoiseId NoiseModel::getId() const
{
        return dspNoiseProxy->getNoiseId();
}

void NoiseModel::enable(bool b)
{
        if (dspNoiseProxy->enable(b))
                action enabled(b);
}

bool NoiseModel::isEnabled() const
{
        return dspNoiseProxy->isEnabled();
}

void NoiseModel::setType(NoiseType type)
{
        if (dspNoiseProxy->setType(type))
                action typeUpdated(type);
}

NoiseType NoiseModel::noiseType() const
{
        return dspNoiseProxy->noiseType();
}

void NoiseModel::setDensity(double value)
{
        if (dspNoiseProxy->setDensity(value))
                action densityUpdated(value);
}

double NoiseModel::density() const
{
        return dspNoiseProxy->density();
}

void NoiseModel::setDensityDefaultValue(double value)
{
        desnityDefaultValue = value;
}

double NoiseModel::getDensityDefaultValue() const
{
        return desnityDefaultValue;
}

void NoiseModel::setDensityRange(double from, double to)
{
        densityRange = {from, to};
}

std::pair<double, double> NoiseModel::getDensityRange() const
{
        return densityRange;
}

void NoiseModel::setBrightness(double value)
{
        if (dspNoiseProxy->setBrightness(value))
                action brightnessUpdated(value);
}

double NoiseModel::brightness() const
{
        return dspNoiseProxy->brightness();
}

void NoiseModel::setBrightnessDefaultValue(double value)
{
        brightnessDefaultValue = value;
}

double NoiseModel::getBrightnessDefaultValue() const
{
        return brightnessDefaultValue;
}

void NoiseModel::setBrightnessRange(double from, double to)
{
        brightnessRange = {from, to};
}

std::pair<double, double> NoiseModel::getBrightnessRange() const
{
        return brightnessRange;
}

void NoiseModel::setGain(double value)
{
        if (dspNoiseProxy->setGain(value))
                action gainUpdated(value);
}

double NoiseModel::gain() const
{
        return dspNoiseProxy->gain();
}

void NoiseModel::setGainDefaultValue(double value)
{
        gainDefaultValue = value;
}

double NoiseModel::getGainDefaultValue() const
{
        return gainDefaultValue;
}

void NoiseModel::setGainRange(double from, double to)
{
        gainRange = {from, to};
}

std::pair<double, double> NoiseModel::getGainRange() const
{
        return gainRange;
}

void NoiseModel::setStereo(double value)
{
        if (dspNoiseProxy->setStereo(value))
                action stereoUpdated(value);
}

double NoiseModel::stereo() const
{
        return dspNoiseProxy->stereo();
}

void NoiseModel::setStereoDefaultValue(double value)
{
        stereoDefaultValue = value;
}

double NoiseModel::getStereoDefaultValue() const
{
        return stereoDefaultValue;
}

void NoiseModel::setStereoRange(double from, double to)
{
        stereoRange = {from, to};
}

std::pair<double, double> NoiseModel::getStereoRange() const
{
        return stereoRange;
}

void NoiseModel::enableFilter(bool b)
{
        if (dspNoiseProxy->enableFilter(b))
                action filterEnabled(b);
}

bool NoiseModel::isFilterEnabled() const
{
        return dspNoiseProxy->isFilterEnabled();
}

void NoiseModel::setFilterType(FilterType type)
{
        if (dspNoiseProxy->setFilterType(type))
                action filterTypeUpdated(type);
}

FilterType NoiseModel::filterType() const
{
        return dspNoiseProxy->filterType();
}

void NoiseModel::setCutOff(double value)
{
        if (dspNoiseProxy->setCutOff(value))
                action cutOffUpdated(value);
}

double NoiseModel::cutOff() const
{
        return dspNoiseProxy->cutOff();
}

void NoiseModel::setCutOffDefaultValue(double value)
{
        cutOffDefaultValue = value;
}

double NoiseModel::getCutOffDefaultValue() const
{
        return cutOffDefaultValue;
}

void NoiseModel::setCutOffRange(double from, double to)
{
        cutOffRange = {from, to};
}

std::pair<double, double> NoiseModel::getCutOffRange() const
{
        return cutOffRange;
}

void NoiseModel::setResonance(double value)
{
        if (dspNoiseProxy->setResonance(value))
                action resonanceUpdated(value);
}

double NoiseModel::resonance() const
{
        return dspNoiseProxy->resonance();
}

void NoiseModel::setResonanceDefaultValue(double value)
{
        resonanceDefaultValue = value;
}

double NoiseModel::getResonanceDefaultValue() const
{
        return resonanceDefaultValue;
}

void NoiseModel::setResonanceRange(double from, double to)
{
        resonanceRange = {from, to};
}

std::pair<double, double> NoiseModel::getResonanceRange() const
{
        return resonanceRange;
}
