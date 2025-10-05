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

#include "DspNoiseProxyVst.h"
#include "EntVstController.h"
#include "globals.h"

#include <cmath>

using namespace EntVst;
using namespace Steinberg::Vst;

DspNoiseProxyVst::DspNoiseProxyVst(RkObject* parent,
                                   EntVstController *controller,
                                   NoiseId id)
        : DspNoiseProxy(parent, id)
        , vstController{controller}
{
        auto paramCallback = [this](ParameterId paramId, ParamValue value){
                onParameterChanged(paramId, value);
        };

        std::vector<EntVst::ParameterId> params;
        if (getNoiseId() == NoiseId::Noise1) {
                params = { ParameterId::Noise1EnabledId,
                           ParameterId::Noise1TypeId,
                           ParameterId::Noise1DensityId,
                           ParameterId::Noise1GainId,
                           ParameterId::Noise1BrightnessId };
        } else {
                params = { ParameterId::Noise2EnabledId,
                           ParameterId::Noise2TypeId,
                           ParameterId::Noise2DensityId,
                           ParameterId::Noise2GainId,
                           ParameterId::Noise2BrightnessId };
        }

        for (const auto& paramId : params)
                vstController->setParamterCallback(paramId, paramCallback);
}

DspNoiseProxyVst::~DspNoiseProxyVst()
{
        if (getNoiseId() == NoiseId::Noise1) {
                vstController->removeParamterCallback(ParameterId::Noise1EnabledId);
                vstController->removeParamterCallback(ParameterId::Noise1TypeId);
                vstController->removeParamterCallback(ParameterId::Noise1DensityId);
                vstController->removeParamterCallback(ParameterId::Noise1BrightnessId);
                vstController->removeParamterCallback(ParameterId::Noise1GainId);
        } else {
                vstController->removeParamterCallback(ParameterId::Noise2EnabledId);
                vstController->removeParamterCallback(ParameterId::Noise2TypeId);
                vstController->removeParamterCallback(ParameterId::Noise2DensityId);
                vstController->removeParamterCallback(ParameterId::Noise2BrightnessId);
                vstController->removeParamterCallback(ParameterId::Noise2GainId);
        }
}

bool DspNoiseProxyVst::enable(bool b)
{
        ENTROPICTRON_LOG_DEBUG("called: ID : " << (int)getNoiseId());
        auto paramId = (getNoiseId() == NoiseId::Noise1) ? ParameterId::Noise1EnabledId : ParameterId::Noise2EnabledId;
        vstController->getComponentHandler()->beginEdit(paramId);
        vstController->getComponentHandler()->performEdit(paramId, b ? 1.0 : 0.0);
        vstController->getComponentHandler()->endEdit(paramId);
        return true;
}

bool DspNoiseProxyVst::isEnabled() const
{
        auto id = (getNoiseId() == NoiseId::Noise1) ? ParameterId::Noise1EnabledId : ParameterId::Noise2EnabledId;
        return vstController->getParamNormalized(id) > 0.5;
}

bool DspNoiseProxyVst::setType(NoiseType type)
{
        auto id = (getNoiseId() == NoiseId::Noise1) ?
                ParameterId::Noise1TypeId : ParameterId::Noise2TypeId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, noiseTypeToNormalized(type));
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

NoiseType DspNoiseProxyVst::noiseType() const
{
        auto id = (getNoiseId() == NoiseId::Noise1) ?
                ParameterId::Noise1TypeId : ParameterId::Noise2TypeId;
        return noiseTypeFromNormalized(vstController->getParamNormalized(id));
}

bool DspNoiseProxyVst::setDensity(double value)
{
        //        ENTROPICTRON_LOG_DEBUG("called: " << value);
        auto id = (getNoiseId() == NoiseId::Noise1) ? ParameterId::Noise1DensityId : ParameterId::Noise2DensityId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, value);
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

double DspNoiseProxyVst::density() const
{
    auto id = (getNoiseId() == NoiseId::Noise1) ? ParameterId::Noise1DensityId : ParameterId::Noise2DensityId;
    return vstController->getParamNormalized(id);
}

bool DspNoiseProxyVst::setBrightness(double value)
{
        ENTROPICTRON_LOG_DEBUG("called: " << value);
        auto id = (getNoiseId() == NoiseId::Noise1) ? ParameterId::Noise1BrightnessId : ParameterId::Noise2BrightnessId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, value);
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

double DspNoiseProxyVst::brightness() const
{
    auto id = (getNoiseId() == NoiseId::Noise1) ? ParameterId::Noise1BrightnessId : ParameterId::Noise2BrightnessId;
    return vstController->getParamNormalized(id);
}

bool DspNoiseProxyVst::setGain(double value)
{
        ENTROPICTRON_LOG_DEBUG("called: " << value);
        auto id = (getNoiseId() == NoiseId::Noise1) ? ParameterId::Noise1GainId : ParameterId::Noise2GainId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, value);
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

double DspNoiseProxyVst::gain() const
{
    auto id = (getNoiseId() == NoiseId::Noise1) ? ParameterId::Noise1GainId : ParameterId::Noise2GainId;
    return vstController->getParamNormalized(id);
}

void DspNoiseProxyVst::onParameterChanged(ParameterId paramId, ParamValue value)
{
        switch (paramId) {
        case ParameterId::Noise1EnabledId:
        case ParameterId::Noise2EnabledId:
                action enabled(value > 0.5);
                break;
        case ParameterId::Noise1TypeId:
        case ParameterId::Noise2TypeId:
                action typeUpdated(noiseTypeFromNormalized(value));
                break;
        case ParameterId::Noise1DensityId:
        case ParameterId::Noise2DensityId:
                action densityUpdated(value);
                break;
        case ParameterId::Noise1BrightnessId:
        case ParameterId::Noise2BrightnessId:
                action brightnessUpdated(value);
                break;
        case ParameterId::Noise1GainId:
        case ParameterId::Noise2GainId:
                action gainUpdated(value);
                break;
        default:
                break;
        }
}

double DspNoiseProxyVst::noiseTypeToNormalized(NoiseType type) const
{
        auto numNoiseTypes = static_cast<double>(NoiseType::BrownNoise);
        return static_cast<double>(type) / numNoiseTypes;
}

NoiseType DspNoiseProxyVst::noiseTypeFromNormalized(double value) const
{
        auto numNoiseTypes = static_cast<double>(NoiseType::BrownNoise);
        return static_cast<NoiseType>(std::round(value * numNoiseTypes));
}


