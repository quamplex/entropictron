/**
 * File name: DspRgateProxyVst.cpp
 * Project: Entropictron (A context generator and audio effect)
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

#include "globals.h"
#include "DspRgateProxyVst.h"
#include "EntVstController.h"
#include "EntState.h"

using namespace EntVst;
using namespace Steinberg::Vst;

DspRgateProxyVst::DspRgateProxyVst(RkObject* parent,
                                   EntVstController *controller)
        : DspRgateProxy(parent)
        , vstController{controller}
{
        auto paramCallback = [this](ParameterId paramId, ParamValue value){
                onParameterChanged(paramId, value);
        };

        std::vector<EntVst::ParameterId> params {
                ParameterId::RgateMinIntervalId,
                ParameterId::RgateMaxIntervalId,
                ParameterId::RgateMinDurationId,
                ParameterId::RgateMaxDurationId,
                ParameterId::RgateMinGainId,
                ParameterId::RgateMaxGainId,
                ParameterId::RgateRandomnessId,
                ParameterId::RgateInvertedId,
        };

        for (const auto& paramId : params)
                vstController->setParamterCallback(paramId, paramCallback);
}

DspRgateProxyVst::~DspRgateProxyVst()
{
        vstController->removeParamterCallback(ParameterId::RgateEnabledId);
        vstController->removeParamterCallback(ParameterId::RgateMinIntervalId);
        vstController->removeParamterCallback(ParameterId::RgateMaxIntervalId);
        vstController->removeParamterCallback(ParameterId::RgateMinDurationId);
        vstController->removeParamterCallback(ParameterId::RgateMaxDurationId);
        vstController->removeParamterCallback(ParameterId::RgateMinGainId);
        vstController->removeParamterCallback(ParameterId::RgateMaxGainId);
        vstController->removeParamterCallback(ParameterId::RgateRandomnessId);
        vstController->removeParamterCallback(ParameterId::RgateInvertedId);
}

bool DspRgateProxyVst::enable(bool b)
{
        return vstController->editParameter(ParameterId::RgateEnabledId,
                                            enabledToNormalized(b));
}

bool DspRgateProxyVst::isEnabled() const
{
        auto val = vstController->getParamNormalized(ParameterId::RgateEnabledId);
        return enabledFromNormalized(val);
}

bool DspRgateProxyVst::setMinInterval(double value)
{
        return vstController->editParameter(ParameterId::RgateMinIntervalId,
                                            minIntervalToNormalized(value));
}

double DspRgateProxyVst::getMinInterval() const
{
        auto val = vstController->getParamNormalized(ParameterId::RgateMinIntervalId);
        return minIntervalFromNormalized(val);
}

bool DspRgateProxyVst::setMaxInterval(double value)
{
        return vstController->editParameter(ParameterId::RgateMaxIntervalId,
                                            maxIntervalToNormalized(value));
}

double DspRgateProxyVst::getMaxInterval() const
{
        auto val = vstController->getParamNormalized(ParameterId::RgateMaxIntervalId);
        return maxIntervalFromNormalized(val);
}

bool DspRgateProxyVst::setMinDuration(double value)
{
        return vstController->editParameter(ParameterId::RgateMinDurationId,
                                            minDurationToNormalized(value));
}

double DspRgateProxyVst::getMinDuration() const
{
        auto val = vstController->getParamNormalized(ParameterId::RgateMinDurationId);
        return minDurationFromNormalized(val);
}

bool DspRgateProxyVst::setMaxDuration(double value)
{
        return vstController->editParameter(ParameterId::RgateMaxDurationId,
                                            maxDurationToNormalized(value));
}

double DspRgateProxyVst::getMaxDuration() const
{
        auto val = vstController->getParamNormalized(ParameterId::RgateMaxDurationId);
        return maxDurationFromNormalized(val);
}

bool DspRgateProxyVst::setMinGain(double value)
{
        if (value > getMaxGain())
                setMaxGain(value);

        return vstController->editParameter(ParameterId::RgateMinGainId,
                                            minGainToNormalized(value));
}

double DspRgateProxyVst::getMinGain() const
{
        auto val = vstController->getParamNormalized(ParameterId::RgateMinGainId);
        return minGainFromNormalized(val);
}

bool DspRgateProxyVst::setMaxGain(double value)
{
        if (value < getMinGain())
                setMinGain(value);

        return vstController->editParameter(ParameterId::RgateMaxGainId,
                                            maxGainToNormalized(value));
}

double DspRgateProxyVst::getMaxGain() const
{
        auto val = vstController->getParamNormalized(ParameterId::RgateMaxGainId);
        return maxGainFromNormalized(val);
}

bool DspRgateProxyVst::setRandomness(double value)
{
        return vstController->editParameter(ParameterId::RgateRandomnessId,
                                            randomnessToNormalized(value));
}

double DspRgateProxyVst::getRandomness() const
{
        auto val = vstController->getParamNormalized(ParameterId::RgateRandomnessId);
        return randomnessFromNormalized(val);
}

bool DspRgateProxyVst::setInverted(bool b)
{
        return vstController->editParameter(ParameterId::RgateInvertedId,
                                            invertedToNormalized(b));
}

bool DspRgateProxyVst::getInverted() const
{
        auto val = vstController->getParamNormalized(ParameterId::RgateInvertedId);
        return invertedFromNormalized(val);
}

void DspRgateProxyVst::onParameterChanged(ParameterId paramId, ParamValue value)
{
        switch (paramId) {
        case ParameterId::RgateEnabledId:
                action enabled(enabledFromNormalized(value));
                break;
        case ParameterId::RgateMinIntervalId:
                action minIntervalUpdated(minIntervalFromNormalized(value));
                break;
        case ParameterId::RgateMaxIntervalId:
                action maxIntervalUpdated(maxIntervalFromNormalized(value));
                break;
        case ParameterId::RgateMinDurationId:
                action minDurationUpdated(minDurationFromNormalized(value));
                break;
        case ParameterId::RgateMaxDurationId:
                action maxDurationUpdated(maxDurationFromNormalized(value));
                break;
        case ParameterId::RgateMinGainId:
                action minGainUpdated(minGainFromNormalized(value));
                break;
        case ParameterId::RgateMaxGainId:
                action maxGainUpdated(maxGainFromNormalized(value));
                break;
        case ParameterId::RgateRandomnessId:
                action randomnessUpdated(randomnessFromNormalized(value));
                break;
        case ParameterId::RgateInvertedId:
                action invertedUpdated(invertedFromNormalized(value));
                break;
        default:
                break;
        }
}

double DspRgateProxyVst::enabledToNormalized(bool b)
{
        return b;
}

bool DspRgateProxyVst::enabledFromNormalized(double value)
{
        return value > 0.5;
}

double DspRgateProxyVst::minIntervalToNormalized(double value)
{
        return toNormalized(value,
                            ENT_RGATE_MIN_MIN_INTERVAL,
                            ENT_RGATE_MAX_MIN_INTERVAL);
}

double DspRgateProxyVst::minIntervalFromNormalized(double value)
{
        return fromNormalized(value,
                              ENT_RGATE_MIN_MIN_INTERVAL,
                              ENT_RGATE_MAX_MIN_INTERVAL);
}

double DspRgateProxyVst::maxIntervalToNormalized(double value)
{
        return toNormalized(value,
                            ENT_RGATE_MIN_MAX_INTERVAL,
                            ENT_RGATE_MAX_MAX_INTERVAL);
}

double DspRgateProxyVst::maxIntervalFromNormalized(double value)
{
        return fromNormalized(value,
                              ENT_RGATE_MIN_MAX_INTERVAL,
                              ENT_RGATE_MAX_MAX_INTERVAL);
}

double DspRgateProxyVst::minDurationToNormalized(double value)
{
        return toNormalized(value,
                            ENT_RGATE_MIN_MIN_DURATION,
                            ENT_RGATE_MAX_MIN_DURATION);
}

double DspRgateProxyVst::minDurationFromNormalized(double value)
{
        return fromNormalized(value,
                              ENT_RGATE_MIN_MIN_DURATION,
                              ENT_RGATE_MAX_MIN_DURATION);
}

double DspRgateProxyVst::maxDurationToNormalized(double value)
{
        return toNormalized(value,
                            ENT_RGATE_MIN_MAX_DURATION,
                            ENT_RGATE_MAX_MAX_DURATION);
}

double DspRgateProxyVst::maxDurationFromNormalized(double value)
{
        return fromNormalized(value,
                              ENT_RGATE_MIN_MAX_DURATION,
                              ENT_RGATE_MAX_MAX_DURATION);
}

double DspRgateProxyVst::minGainToNormalized(double value)
{
        return toNormalized(value,
                            ENT_RGATE_MIN_MIN_GAIN,
                            ENT_RGATE_MAX_MIN_GAIN);
}

double DspRgateProxyVst::minGainFromNormalized(double value)
{
        return fromNormalized(value,
                              ENT_RGATE_MIN_MIN_GAIN,
                              ENT_RGATE_MAX_MIN_GAIN);
}

double DspRgateProxyVst::maxGainToNormalized(double value)
{
        return toNormalized(value,
                            ENT_RGATE_MIN_MAX_GAIN,
                            ENT_RGATE_MAX_MAX_GAIN);
}

double DspRgateProxyVst::maxGainFromNormalized(double value)
{
        return fromNormalized(value,
                              ENT_RGATE_MIN_MAX_GAIN,
                              ENT_RGATE_MAX_MAX_GAIN);
}

double DspRgateProxyVst::randomnessToNormalized(double value)
{
        return toNormalized(value,
                            ENT_RGATE_MIN_RANDOMNESS,
                            ENT_RGATE_MAX_RANDOMNESS);
}

double DspRgateProxyVst::randomnessFromNormalized(double value)
{
        return fromNormalized(value,
                              ENT_RGATE_MIN_RANDOMNESS,
                              ENT_RGATE_MAX_RANDOMNESS);
}

double DspRgateProxyVst::invertedToNormalized(bool b)
{
        return b;
}

bool DspRgateProxyVst::invertedFromNormalized(double value)
{
        return value > 0.5;
}
