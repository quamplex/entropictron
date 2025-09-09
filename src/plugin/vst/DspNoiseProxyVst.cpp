/**
 * File name: DspNoiseProxyVst.cpp
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

#include "DspNoiseProxy.h"

using namespace EnVst;

DspNoiseProxyVst::DspNoiseProxy(EntVstController *controller,  NoiseId id)
{
        auto paramCallback = [this](const ParametersId& paramId, const ParameterValue &value){
                onParameterChanged(paramId, value);
        };

        std::vector<EntVst::ParamterId> params;
        if (id == NoiseId::Noise1) {
                params = { ParamterId::Noise1EnabledId, ParamterId::Noise1TypeId,
                           ParamterId::Noise1GainId, ParamterId::Noise1BrightnessId };
        } else {
                params = { ParamterId::Noise2EnabledId, ParamterId::Noise2TypeId,
                           ParamterId::Noise2GainId, ParamterId::Noise2BrightnessId };
        }

        for (const auto& paramId : params)
                controller->setParamterCallback(paramId, paramCallback);
}

DspNoiseProxyVst::~DspNoiseProxyVst()
{
        if (id == NoiseId::Noise1) {
                controller->setParamterCallback({}, ParamterId::Noise1EnabledId);
                controller->setParamterCallback({}, ParamterId::Noise1TypeId);
                controller->setParamterCallback({}, ParamterId::Noise1GainId);
                controller->setParamterCallback({}, ParamterId::Noise1BrightnessId);
        } else {
                controller->setParamterCallback({}, ParamterId::Noise2EnabledId);
                controller->setParamterCallback({}, ParamterId::Noise2TypeId);
                controller->setParamterCallback({}, ParamterId::Noise2GainId);
                controller->setParamterCallback({}, ParamterId::Noise2BrightnessId);
        }
}

void DspNoiseProxyVst::enable(bool b = true)
{
}

bool DspNoiseProxyVst::isEnabled() const
{
}

void DspNoiseProxyVst::setType(NoiseType type)
{
}

NoiseType DspNoiseProxyVst::noiseType() const
{
}

void DspNoiseProxyVst::setDensity(double value)
{
}

double DspNoiseProxyVst::density() const
{
}

void DspNoiseProxyVst::setBrightness(double value)
{
}

double DspNoiseProxyVst::brightness() const
{
}

void DspNoiseProxyVst::setGain(double value)
{
}

double DspNoiseProxyVst::gain() const
{
}

void DspNoiseProxyVst::onParameterChanged(const ParamterId &paramId,
                                          const ParameterValue &value)
{
        switch (paramId) {
        case ParamterId::Noise1EnabledId:
        case ParamterId::Noise2EnabledId:
                action enabled(...);
                break;

        case ParamterId::Noise1TypeId:
        case ParamterId::Noise2TypeId:
                action typeUpdated();
                break;

        case ParamterId::Noise1GainId:
        case ParamterId::Noise2GainId:
                actionGain(value);
                break;

        case ParamterId::Noise1BrightnessId:
        case ParamterId::Noise2BrightnessId:
                actionBrightness(value);
                break;

        default:
                break;
        }
}
