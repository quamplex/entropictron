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
        : DspNoiseProxy(id)
        , vstController {constroller}
{
        auto paramCallback = [this](ParametersId paramId, const ParamValue &value){
                onParameterChanged(paramId, value);
        };

        std::vector<EntVst::ParamterId> params;
        if (noiseId == NoiseId::Noise1) {
                params = { ParamterId::Noise1EnabledId, ParamterId::Noise1TypeId,
                           ParamterId::Noise1GainId, ParamterId::Noise1BrightnessId };
        } else {
                params = { ParamterId::Noise2EnabledId, ParamterId::Noise2TypeId,
                           ParamterId::Noise2GainId, ParamterId::Noise2BrightnessId };
        }

        for (const auto& paramId : params)
                vstController->setParamterCallback(paramId, paramCallback);
}

DspNoiseProxyVst::~DspNoiseProxyVst()
{
        if (noiseId == NoiseId::Noise1) {
                controller->removeParamterCallback(ParamterId::Noise1EnabledId);
                controller->removeParamterCallback(ParamterId::Noise1TypeId);
                controller->removeParamterCallback(ParamterId::Noise1DensityId);
                controller->removeParamterCallback(ParamterId::Noise1BrightnessId);
                controller->removeParamterCallback(ParamterId::Noise1GainId);
        } else {
                controller->removeParamterCallback(ParamterId::Noise2EnabledId);
                controller->removeParamterCallback(ParamterId::Noise2TypeId);
                controller->removeParamterCallback(ParamterId::Noise2DensityId);
                controller->removeParamterCallback(ParamterId::Noise2BrightnessId);
                controller->removeParamterCallback(ParamterId::Noise2GainId);
        }
}

void DspNoiseProxyVst::enable(bool b = true)
{
    auto id = (getNoiseId() == NoiseId::Noise1) ? ParamterId::Noise1EnabledId : ParamterId::Noise2EnabledId;
    vstController->getComponentHandler()->beginEdit(id);
    vstController->getComponentHandler()->performEdit(id, b ? 1.0 : 0.0);
    vstController->getComponentHandler()->endEdit(id);
}

bool DspNoiseProxyVst::isEnabled() const
{
    auto id = (getNoiseId() == NoiseId::Noise1) ? ParamterId::Noise1EnabledId : ParamterId::Noise2EnabledId;
    return vstController->getParamNormalized(id) > 0.5;
}

void DspNoiseProxyVst::setType(NoiseType type)
{
    auto id = (getNoiseId() == NoiseId::Noise1) ? ParamterId::Noise1TypeId : ParamterId::Noise2TypeId;
    const double numNoiseTypes = static_cast<double>(kNumNoiseTypes);
    double normalizedValue = static_cast<double>(type) / (numNoiseTypes - 1.0);
    vstController->getComponentHandler()->beginEdit(id);
    vstController->getComponentHandler()->performEdit(id, normalizedValue);
    vstController->getComponentHandler()->endEdit(id);
}

NoiseType DspNoiseProxyVst::noiseType() const
{
    auto id = (getNoiseId() == NoiseId::Noise1) ? ParamterId::Noise1TypeId : ParamterId::Noise2TypeId;
    const double numNoiseTypes = static_cast<double>(kNumNoiseTypes);
    double normalizedValue = vstController->getParamNormalized(id);
    return static_cast<NoiseType>(static_cast<int>(std::round(normalizedValue * (numNoiseTypes - 1.0))));
}

void DspNoiseProxyVst::setDensity(double value)
{
    auto id = (getNoiseId() == NoiseId::Noise1) ? ParamterId::Noise1DensityId : ParamterId::Noise2DensityId;
    vstController->getComponentHandler()->beginEdit(id);
    vstController->getComponentHandler()->performEdit(id, value);
    vstController->getComponentHandler()->endEdit(id);
}

double DspNoiseProxyVst::density() const
{
    auto id = (getNoiseId() == NoiseId::Noise1) ? ParamterId::Noise1DensityId : ParamterId::Noise2DensityId;
    return vstController->getParamNormalized(id);
}

void DspNoiseProxyVst::setBrightness(double value)
{
    auto id = (getNoiseId() == NoiseId::Noise1) ? ParamterId::Noise1BrightnessId : ParamterId::Noise2BrightnessId;
    vstController->getComponentHandler()->beginEdit(id);
    vstController->getComponentHandler()->performEdit(id, value);
    vstController->getComponentHandler()->endEdit(id);
}

double DspNoiseProxyVst::brightness() const
{
    auto id = (getNoiseId() == NoiseId::Noise1) ? ParamterId::Noise1BrightnessId : ParamterId::Noise2BrightnessId;
    return vstController->getParamNormalized(id);
}

void DspNoiseProxyVst::setGain(double value)
{
    auto id = (getNoiseId() == NoiseId::Noise1) ? ParamterId::Noise1GainId : ParamterId::Noise2GainId;
    vstController->getComponentHandler()->beginEdit(id);
    vstController->getComponentHandler()->performEdit(id, value);
    vstController->getComponentHandler()->endEdit(id);
}

double DspNoiseProxyVst::gain() const
{
    auto id = (getNoiseId() == NoiseId::Noise1) ? ParamterId::Noise1GainId : ParamterId::Noise2GainId;
    return vstController->getParamNormalized(id);
}

void DspNoiseProxyVst::onParameterChanged(ParamterId paramId,
                                          const ParamValue &value)
{
        switch (paramId) {
        case ParamterId::Noise1EnabledId:
        case ParamterId::Noise2EnabledId:
                action enabled(value > 0.5);
                break;
        case ParamterId::Noise1TypeId:
        case ParamterId::Noise2TypeId:
                action typeUpdated(static_cast<int>(value + 0.5));
                break;
        case ParamterId::Noise1BrightnessId:
        case ParamterId::Noise2BrightnessId:
                actionBrightness(value);
                break;
        case ParamterId::Noise1GainId:
        case ParamterId::Noise2GainId:
                actionGain(value);
                break;
        default:
                break;
        }
}
