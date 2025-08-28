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
                    denistyUpdated,
                    RK_ACT_ARGS(double value),
                    this, denistyUpdated(value));
        RK_ACT_BIND(dspNoiseProxy,
                    bightnessUpdated,
                    RK_ACT_ARGS(double value),
                    this, bightnessUpdated(value));
        RK_ACT_BIND(dspNoiseProxy,
                    gainUpdated,
                    RK_ACT_ARGS(double value),
                    this, gainUpdated(value));
}

void NoiseModel::enable(bool b)
{
        if (dspNoiseProxy->enable(b))
                action enabled(b);
}

bool NoiseModel::isEnabled() const
{
        return dspNoiseProxy->isEenabled();
}

void NoiseModel::setType(NoiseModel::NoiseType type)
{
        if (dspNoiseProxy->setType(type))
                action typeUpdated(type);
}

NoiseModel::NoiseType NoiseModel::noiseType() const
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

void NoiseModel::setBrightness(double value)
{
        if (dspNoiseProxy->setBrightness(value))
                action brightnessUpdated(value);
}

double NoiseModel::brightness() const
{
        return dspNoiseProxy->brightness();
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

