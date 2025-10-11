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
                params = { ParameterId::Glitch1EnabledId };
        } else {
                params = { ParameterId::Glitch2EnabledId};
        }

        for (const auto& paramId : params)
                vstController->setParamterCallback(paramId, paramCallback);
}

DspGlitchProxyVst::~DspGlitchProxyVst()
{
        if (getGlitchId() == GlitchId::Glitch1) {
                vstController->removeParamterCallback(ParameterId::Glitch1EnabledId);
        } else {
                vstController->removeParamterCallback(ParameterId::Glitch2EnabledId);
        }
}

bool DspGlitchProxyVst::enable(bool b)
{
        ENTROPICTRON_LOG_DEBUG("called: ID : " << (int)getGlitchId());
        auto paramId = (getGlitchId() == GlitchId::Glitch1) ? ParameterId::Glitch1EnabledId : ParameterId::Glitch2EnabledId;
        vstController->getComponentHandler()->beginEdit(paramId);
        vstController->getComponentHandler()->performEdit(paramId, b ? 1.0 : 0.0);
        vstController->getComponentHandler()->endEdit(paramId);
        return true;
}

bool DspGlitchProxyVst::isEnabled() const
{
        auto id = (getGlitchId() == GlitchId::Glitch1) ? ParameterId::Glitch1EnabledId : ParameterId::Glitch2EnabledId;
        return vstController->getParamNormalized(id) > 0.5;
}

void DspGlitchProxyVst::onParameterChanged(ParameterId paramId, ParamValue value)
{
        switch (paramId) {
        case ParameterId::Glitch1EnabledId:
        case ParameterId::Glitch2EnabledId:
                action enabled(value > 0.5);
                break;
        default:
                break;
        }
}
