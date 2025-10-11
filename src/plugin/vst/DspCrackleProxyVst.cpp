/**
 * File name: DspCrackleProxyVst.cpp
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

#include "DspCrackleProxyVst.h"
#include "EntVstController.h"
#include "globals.h"

using namespace EntVst;
using namespace Steinberg::Vst;

DspCrackleProxyVst::DspCrackleProxyVst(RkObject* parent,
                                   EntVstController *controller,
                                   CrackleId id)
        : DspCrackleProxy(parent, id)
        , vstController{controller}
{
        auto paramCallback = [this](ParameterId paramId, ParamValue value){
                onParameterChanged(paramId, value);
        };

        std::vector<EntVst::ParameterId> params;
        if (getCrackleId() == CrackleId::Crackle1) {
                params = { ParameterId::Crackle1EnabledId };
        } else {
                params = { ParameterId::Crackle2EnabledId};
        }

        for (const auto& paramId : params)
                vstController->setParamterCallback(paramId, paramCallback);
}

DspCrackleProxyVst::~DspCrackleProxyVst()
{
        if (getCrackleId() == CrackleId::Crackle1) {
                vstController->removeParamterCallback(ParameterId::Crackle1EnabledId);
        } else {
                vstController->removeParamterCallback(ParameterId::Crackle2EnabledId);
        }
}

bool DspCrackleProxyVst::enable(bool b)
{
        ENTROPICTRON_LOG_DEBUG("called: ID : " << (int)getCrackleId());
        auto paramId = (getCrackleId() == CrackleId::Crackle1) ? ParameterId::Crackle1EnabledId : ParameterId::Crackle2EnabledId;
        vstController->getComponentHandler()->beginEdit(paramId);
        vstController->getComponentHandler()->performEdit(paramId, b ? 1.0 : 0.0);
        vstController->getComponentHandler()->endEdit(paramId);
        return true;
}

bool DspCrackleProxyVst::isEnabled() const
{
        auto id = (getCrackleId() == CrackleId::Crackle1) ? ParameterId::Crackle1EnabledId : ParameterId::Crackle2EnabledId;
        return vstController->getParamNormalized(id) > 0.5;
}

void DspCrackleProxyVst::onParameterChanged(ParameterId paramId, ParamValue value)
{
        switch (paramId) {
        case ParameterId::Crackle1EnabledId:
        case ParameterId::Crackle2EnabledId:
                action enabled(value > 0.5);
                break;
        default:
                break;
        }
}
