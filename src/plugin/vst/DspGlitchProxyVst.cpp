/**
 * File name: DspGlitchProxyVst.cpp
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

#include "DspGlitchProxyVst.h"
#include "EntVstController.h"
#include "globals.h"

using namespace EntVst;
using namespace Steinberg::Vst;

DspGlitchProxyVst::DspGlitchProxyVst(RkObject* parent,
                                   EntVstController *controller,
                                   GlitchId id)
        : DspGlitchProxy(parent, id)
        , vstController{controller}
{
        auto paramCallback = [this](ParameterId paramId, ParamValue value){
                onParameterChanged(paramId, value);
        };

        std::vector<EntVst::ParameterId> params;
        if (getGlitchId() == GlitchId::Glitch1) {
                params = {
                        ParameterId::Glitch1EnabledId,
                        ParameterId::Glitch1RepeatsId,
                        ParameterId::Glitch1ProbabilityId,
                        ParameterId::Glitch1LengthId,
                        ParameterId::Glitch1MaxJumpId,
                        ParameterId::Glitch1MinJumpId
                };
        } else {
                params = {
                        ParameterId::Glitch2EnabledId,
                        ParameterId::Glitch2RepeatsId,
                        ParameterId::Glitch2ProbabilityId,
                        ParameterId::Glitch2LengthId,
                        ParameterId::Glitch2MaxJumpId,
                        ParameterId::Glitch2MinJumpId
                };
        }

        for (const auto& paramId : params)
                vstController->setParamterCallback(paramId, paramCallback);
}

DspGlitchProxyVst::~DspGlitchProxyVst()
{
        if (getGlitchId() == GlitchId::Glitch1) {
                vstController->removeParamterCallback(ParameterId::Glitch1EnabledId);
                vstController->removeParamterCallback(ParameterId::Glitch1RepeatsId);
                vstController->removeParamterCallback(ParameterId::Glitch1ProbabilityId);
                vstController->removeParamterCallback(ParameterId::Glitch1LengthId);
                vstController->removeParamterCallback(ParameterId::Glitch1MaxJumpId);
                vstController->removeParamterCallback(ParameterId::Glitch1MinJumpId);
        } else {
                vstController->removeParamterCallback(ParameterId::Glitch2EnabledId);
                vstController->removeParamterCallback(ParameterId::Glitch2RepeatsId);
                vstController->removeParamterCallback(ParameterId::Glitch2ProbabilityId);
                vstController->removeParamterCallback(ParameterId::Glitch2LengthId);
                vstController->removeParamterCallback(ParameterId::Glitch2MaxJumpId);
                vstController->removeParamterCallback(ParameterId::Glitch2MinJumpId);
        }
}

bool DspGlitchProxyVst::enable(bool b)
{
        auto id; = (getGlitchId() == GlitchId::Glitch1) ?
                           ParameterId::Glitch1EnabledId : ParameterId::Glitch2EnabledId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, b ? 1.0 : 0.0);
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

bool DspGlitchProxyVst::isEnabled() const
{
        auto id = (getGlitchId() == GlitchId::Glitch1) ?
                ParameterId::Glitch1EnabledId : ParameterId::Glitch2EnabledId;
        return vstController->getParamNormalized(id) > 0.5;
}

bool DspGlitchProxyVst::setRepeats(double value)
{
        auto id = (getGlitchId() == GlitchId::Glitch1) ?
                ParameterId::Glitch1RepeatsId : ParameterId::Glitch2RepeatsId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, value);
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

double DspGlitchProxyVst::repeats() const
{
        auto id = (getGlitchId() == GlitchId::Glitch1) ?
                ParameterId::Glitch1RepeatsId : ParameterId::Glitch2RepeatsId;
        return vstController->getParamNormalized(id);
}

bool DspGlitchProxyVst::setProbability(double value)
{
        auto id = (getGlitchId() == GlitchId::Glitch1) ?
                ParameterId::Glitch1ProbabilityId : ParameterId::Glitch2ProbabilityId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, value);
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

double DspGlitchProxyVst::probability() const
{
        auto id = (getGlitchId() == GlitchId::Glitch1) ?
                ParameterId::Glitch1ProbabilityId : ParameterId::Glitch2ProbabilityId;
        return vstController->getParamNormalized(id);
}

bool DspGlitchProxyVst::setLength(double value)
{
        auto id = (getGlitchId() == GlitchId::Glitch1) ?
                ParameterId::Glitch1LengthId : ParameterId::Glitch2LengthId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, value);
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

double DspGlitchProxyVst::length() const
{
        auto id = (getGlitchId() == GlitchId::Glitch1) ?
                ParameterId::Glitch1LengthId : ParameterId::Glitch2LengthId;
        return vstController->getParamNormalized(id);
}

bool DspGlitchProxyVst::setMaxJump(double value)
{
        auto id = (getGlitchId() == GlitchId::Glitch1) ? ParameterId::Glitch1MaxJumpId : ParameterId::Glitch2MaxJumpId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, value);
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

double DspGlitchProxyVst::maxJump() const
{
        auto id = (getGlitchId() == GlitchId::Glitch1) ?
                ParameterId::Glitch1MaxJumpId : ParameterId::Glitch2MaxJumpId;
        return vstController->getParamNormalized(id);
}

bool DspGlitchProxyVst::setMinJump(double value)
{
        auto id = (getGlitchId() == GlitchId::Glitch1) ?
                ParameterId::Glitch1MinJumpId : ParameterId::Glitch2MinJumpId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, value);
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

double DspGlitchProxyVst::minJump() const
{
        auto id = (getGlitchId() == GlitchId::Glitch1) ?
                ParameterId::Glitch1MinJumpId : ParameterId::Glitch2MinJumpId;
        return vstController->getParamNormalized(id);
}

void DspGlitchProxyVst::onParameterChanged(ParameterId paramId, ParamValue value)
{
        switch (paramId) {
        case ParameterId::Glitch1EnabledId:
        case ParameterId::Glitch2EnabledId:
                action enable(value > 0.5);
                break;
        case ParameterId::Glitch1RepeatsId:
        case ParameterId::Glitch2RepeatsId:
                action repeatsUpdated(value);
                break;
        case ParameterId::Glitch1ProbabilityId:
        case ParameterId::Glitch2ProbabilityId:
                action probabilityUpdated(value);
                break;
        case ParameterId::Glitch1LengthId:
        case ParameterId::Glitch2LengthId:
                action lengthUpdated(value);
                break;
        case ParameterId::Glitch1MaxJumpId:
        case ParameterId::Glitch2MaxJumpId:
                action maxJumpUpdated(value);
                break;
        case ParameterId::Glitch1MinJumpId:
        case ParameterId::Glitch2MinJumpId:
                action minJumpUpdated(value);
                break;
        default:
                break;
        }
}

