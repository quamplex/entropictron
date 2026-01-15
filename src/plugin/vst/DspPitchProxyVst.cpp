/**
 * File name: DspPitchProxyVst.cpp
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
#include "DspPitchProxyVst.h"
#include "EntVstController.h"
#include "EntState.h"

using namespace EntVst;
using namespace Steinberg::Vst;

DspPitchProxyVst::DspPitchProxyVst(RkObject* parent,
                                   EntVstController *controller)
        : DspPitchProxy(parent)
        , vstController{controller}
{
        auto paramCallback = [this](ParameterId paramId, ParamValue value){
                onParameterChanged(paramId, value);
        };

        std::vector<EntVst::ParameterId> params;
        params = {
                ParameterId::PitchEnabledId,
                ParameterId::PitchPitchId,
                ParameterId::PitchFineId,
                ParameterId::PitchDepthId,
                ParameterId::PitchDriftId,
        };

        vstController->setParamNormalized (ParameterId::PitchEnabledId, 0);
        vstController->setParamNormalized (ParameterId::PitchPitchId,
                                           pitchToNormalized(ENT_PITCH_DEFAULT_PITCH));
        vstController->setParamNormalized (ParameterId::PitchFineId,
                                           pitchToNormalized(ENT_PITCH_DEFAULT_FINE));
        vstController->setParamNormalized (ParameterId::PitchDepthId,
                                           depthToNormalized(ENT_PITCH_DEFAULT_DEPTH));
        vstController->setParamNormalized (ParameterId::PitchDriftId,
                                           driftToNormalized(ENT_PITCH_DEFAULT_DRIFT));

        for (const auto& paramId : params)
                vstController->setParamterCallback(paramId, paramCallback);
}

DspPitchProxyVst::~DspPitchProxyVst()
{
        vstController->removeParamterCallback(ParameterId::PitchEnabledId);
        vstController->removeParamterCallback(ParameterId::PitchPitchId);
        vstController->removeParamterCallback(ParameterId::PitchFineId);
        vstController->removeParamterCallback(ParameterId::PitchDepthId);
        vstController->removeParamterCallback(ParameterId::PitchDriftId);
}

bool DspPitchProxyVst::enable(bool b)
{
        auto id = ParameterId::PitchEnabledId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, b ? 1.0 : 0.0);
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

bool DspPitchProxyVst::isEnabled() const
{
        auto id = ParameterId::PitchEnabledId;
        return vstController->getParamNormalized(id) > 0.5;
}

bool DspPitchProxyVst::setPitch(int value)
{
        auto id = ParameterId::PitchPitchId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, pitchToNormalized(value));
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

int DspPitchProxyVst::pitch() const
{
        auto id = ParameterId::PitchPitchId;
        return pitchFromNormalized(vstController->getParamNormalized(id));
}

bool DspPitchProxyVst::setFine(double value)
{
        auto id = ParameterId::PitchFineId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, fineToNormalized(value));
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

double DspPitchProxyVst::fine() const
{
        auto id = ParameterId::PitchFineId;
        return fineFromNormalized(vstController->getParamNormalized(id));
}

bool DspPitchProxyVst::setDepth(double value)
{
        auto id = ParameterId::PitchDepthId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, depthToNormalized(value));
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

double DspPitchProxyVst::depth() const
{
        auto id = ParameterId::PitchDepthId;
        return depthFromNormalized(vstController->getParamNormalized(id));
}

bool DspPitchProxyVst::setDrift(double value)
{
        auto id = ParameterId::PitchDriftId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, driftToNormalized(value));
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

double DspPitchProxyVst::drift() const
{
        auto id = ParameterId::PitchDriftId;
        return driftFromNormalized(vstController->getParamNormalized(id));
}

void DspPitchProxyVst::onParameterChanged(ParameterId paramId, ParamValue value)
{
        switch (paramId) {
        case ParameterId::PitchEnabledId:
                action enabled(value > 0.5);
                break;
        case ParameterId::PitchPitchId:
                action pitchUpdated(value);
                break;
        case ParameterId::PitchFineId:
                action fineUpdated(value);
                break;
        case ParameterId::PitchDepthId:
                action depthUpdated(value);
                break;
        case ParameterId::PitchDriftId:
                action driftUpdated(value);
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

double DspPitchProxyVst::pitchToNormalized(int value)
{
        return toNormalized(static_cast<double>(value),
                            ENT_PITCH_PITCH_MIN,
                            ENT_PITCH_PITCH_MAX);
}

int DspPitchProxyVst::pitchFromNormalized(double normalized)
{
        return static_cast<int>(std::round(fromNormalized(normalized,
                                                          ENT_PITCH_PITCH_MIN,
                                                          ENT_PITCH_PITCH_MAX)));
}

double DspPitchProxyVst::fineToNormalized(double value)
{
        return toNormalized(value,
                            ENT_PITCH_FINE_MIN,
                            ENT_PITCH_FINE_MAX);
}

double DspPitchProxyVst::fineFromNormalized(double normalized)
{
        return fromNormalized(normalized,
                              ENT_PITCH_FINE_MIN,
                              ENT_PITCH_FINE_MAX);
}

double DspPitchProxyVst::depthToNormalized(double value)
{
        return toNormalized(value,
                            ENT_PITCH_DEPTH_MIN,
                            ENT_PITCH_DEPTH_MAX);
}

double DspPitchProxyVst::depthFromNormalized(double normalized)
{
        return fromNormalized(normalized,
                              ENT_PITCH_DEPTH_MIN,
                              ENT_PITCH_DEPTH_MAX);
}

double DspPitchProxyVst::driftToNormalized(double value)
{
        return toNormalized(value,
                            ENT_PITCH_DRIFT_MIN,
                            ENT_PITCH_DRIFT_MAX);
}

double DspPitchProxyVst::driftFromNormalized(double normalized)
{
        return fromNormalized(normalized,
                              ENT_PITCH_DRIFT_MIN,
                              ENT_PITCH_DRIFT_MAX);
}
