/**
 * File name: DspRgateProxyVst.cpp
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
                ParameterId::RgateMinInterval,
                ParameterId::RgateMaxInterval,
                ParameterId::RgateMinDuration,
                ParameterId::RgateMaxDuration,
                ParameterId::RgateMinGain,
                ParameterId::RgateMaxGain,
                ParameterId::RgateRandomness,
                ParameterId::RgateInverted,
        };

        vstController->setParamNormalized(ParameterId::RgateEnabledId, 0);
        vstController->setParamNormalized(ParameterId::RgateMinIntervalId,
                                          intervalToNormalized(ENT_RGATE_DEFAULT_MIN_INTERVAL));
        vstController->setParamNormalized(ParameterId::RgateMaxIntervalId,
                                          intervalToNormalized(ENT_RGATE_DEFAULT_MAX_INTERVAL));
        vstController->setParamNormalized(ParameterId::RgateMinDurationId,
                                          durationToNormalized(ENT_RGATE_DEFAULT_MIN_DURATION));
        vstController->setParamNormalized(ParameterId::RgateMaxDurationId,
                                          durationToNormalized(ENT_RGATE_DEFAULT_MAX_DURATION));
        vstController->setParamNormalized(ParameterId::RgateMinGainId,
                                          gainToNormalized(ENT_RGATE_DEFAULT_MIN_GAIN));
        vstController->setParamNormalized(ParameterId::RgateMaxGainId,
                                          gainToNormalized(ENT_RGATE_DEFAULT_MAX_GAIN));
        vstController->setParamNormalized(ParameterId::RgateInvertedId, ENT_RGATE_DEFAULT_INVERTED ? 1.0 : 0.0);

        for (const auto& paramId : params)
                vstController->setParamterCallback(paramId, paramCallback);
}

DspRgateProxyVst::~DspRgateProxyVst()
{
        vstController->removeParamterCallback(ParameterId::RgateEnabledId, 0);
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
        vstController->getComponentHandler()->beginEdit(ParameterId::RgateEnabledId);
        vstController->getComponentHandler()->performEdit(ParameterId::RgateEnabledId, b ? 1.0 : 0.0);
        vstController->getComponentHandler()->endEdit(ParameterId::RgateEnabledId);
        return true;
}

bool DspRgateProxyVst::isEnabled() const
{
        return vstController->getParamNormalized(ParameterId::RgateEnabledId) > 0.5;
}

bool DspGlitchProxyVst::setMinInterval(double value)
{
        auto handler = vstController->getComponentHandler();
        handler->beginEdit(ParameterId::RgateMinIntervalId);
        handler->performEdit(ParameterId::RgateMinIntervalId),
                             minIntervalToNormalized(value));
        handler->endEdit(id);
        return true;
}

double DspGlitchProxyVst::getMinInterval() const
{
        auto id = (getGlitchId() == GlitchId::Glitch1) ?
                  ParameterId::Glitch1MaxJumpId : ParameterId::Glitch2MaxJumpId;
        return maxJumpFromNormalized(vstController->getParamNormalized(id));
}

void DspRgateProxyVst::onParameterChanged(ParameterId paramId, ParamValue value)
{
        switch (paramId) {
        case ParameterId::Rgate1EnabledId:
        case ParameterId::Rgate2EnabledId:
                action enabled(value > 0.5);
                break;
        case ParameterId::Rgate1RepeatsId:
        case ParameterId::Rgate2RepeatsId:
                action repeatsUpdated(value);
                break;
        case ParameterId::Rgate1ProbabilityId:
        case ParameterId::Rgate2ProbabilityId:
                action probabilityUpdated(value);
                break;
        case ParameterId::Rgate1LengthId:
        case ParameterId::Rgate2LengthId:
                action lengthUpdated(value);
                break;
        case ParameterId::Rgate1MaxJumpId:
        case ParameterId::Rgate2MaxJumpId:
                action maxJumpUpdated(value);
                break;
        case ParameterId::Rgate1MinJumpId:
        case ParameterId::Rgate2MinJumpId:
                action minJumpUpdated(value);
                break;
        default:
                break;
        }
}

static double toNormalized(double value, double min, double max)
{
        return (value - min) / (max - min);
}

static double fromNormalized(double normalized, double min, double max)
{
        return min + normalized * (max - min);
}

double DspRgateProxyVst::repeatsToNormalized(int value)
{
        return toNormalized(static_cast<double>(value),
                            ENT_RGATE_MIN_REPEATS,
                            ENT_RGATE_MAX_REPEATS);
}

int DspRgateProxyVst::repeatsFromNormalized(double normalized)
{
        return static_cast<int>(std::round(fromNormalized(normalized,
                                                          ENT_RGATE_MIN_REPEATS,
                                                          ENT_RGATE_MAX_REPEATS)));
}

double DspRgateProxyVst::probabilityToNormalized(double value)
{
        return toNormalized(value,
                            ENT_RGATE_MIN_PROB,
                            ENT_RGATE_MAX_PROB);
}

double DspRgateProxyVst::probabilityFromNormalized(double normalized)
{
        return fromNormalized(normalized,
                              ENT_RGATE_MIN_PROB,
                              ENT_RGATE_MAX_PROB);
}

double DspRgateProxyVst::lengthToNormalized(double value)
{
        return toNormalized(value,
                            ENT_RGATE_MIN_LENGH,
                            ENT_RGATE_MAX_LENGH);
}

double DspRgateProxyVst::lengthFromNormalized(double normalized)
{
        return fromNormalized(normalized,
                              ENT_RGATE_MIN_LENGH,
                              ENT_RGATE_MAX_LENGH);
}

double DspRgateProxyVst::minJumpToNormalized(double value)
{
        return toNormalized(value,
                            ENT_RGATE_MIN_MIN_JUMP,
                            ENT_RGATE_MAX_MIN_JUMP);
}

double DspRgateProxyVst::minJumpFromNormalized(double normalized)
{
        return fromNormalized(normalized,
                              ENT_RGATE_MIN_MIN_JUMP,
                              ENT_RGATE_MAX_MIN_JUMP);
}

double DspRgateProxyVst::maxJumpToNormalized(double value)
{
        return toNormalized(value,
                            ENT_RGATE_MIN_MAX_JUMP,
                            ENT_RGATE_MAX_MAX_JUMP);
}

double DspRgateProxyVst::maxJumpFromNormalized(double normalized)
{
        return fromNormalized(normalized,
                              ENT_RGATE_MIN_MAX_JUMP,
                              ENT_RGATE_MAX_MAX_JUMP);
}
