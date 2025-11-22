/**
 * File name: DspProxyVst.cpp
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

#include "DspVstProxy.h"
#include "EntVstController.h"
#include "DspNoiseProxyVst.h"
#include "DspCrackleProxyVst.h"
#include "DspGlitchProxyVst.h"

DspProxyVst::DspProxyVst(EntVstController *controller)
        : vstController{controller}
        , dspNoise1Proxy {new DspNoiseProxyVst(this,
                                               controller,
                                               NoiseId::Noise1)}
        , dspNoise2Proxy {new DspNoiseProxyVst(this,
                                               controller,
                                               NoiseId::Noise2)}
        , dspCrackle1Proxy {new DspCrackleProxyVst(this,
                                                   controller,
                                                   CrackleId::Crackle1)}
        , dspCrackle2Proxy {new DspCrackleProxyVst(this,
                                                   controller,
                                                   CrackleId::Crackle2)}
        , dspGlitch1Proxy {new DspGlitchProxyVst(this,
                                                 controller,
                                                 GlitchId::Glitch1)}
        , dspGlitch2Proxy {new DspGlitchProxyVst(this,
                                                 controller,
                                                 GlitchId::Glitch2)}
{
        auto paramCallback = [this](ParameterId paramId, ParamValue value){
                onParameterChanged(paramId, value);
        };

        vstController->setParamterCallback(ParameterId::PlayModeId, paramCallback);
}

bool DspProxyVst::setPlayMode(PlayMode mode)
{
        vstController->getComponentHandler()->beginEdit(ParameterId::PlayModeId);
        vstController->getComponentHandler()->performEdit(ParameterId::PlayModeId,
                                                          playModeToNormalized(mode));
        vstController->getComponentHandler()->endEdit(ParameterId::PlayModeId);
        return true;
}

PlayMode DspProxyVst::playMode() const
{
        auto id = ParameterId::PlayModeId;
        return playModeFromNormalized(vstController->getParamNormalized(id));
}

DspNoiseProxy* DspProxyVst::getNoise(NoiseId id) const
{
        switch(id) {
        case NoiseId::Noise1:
                return dspNoise1Proxy;
        case NoiseId::Noise2:
                return dspNoise2Proxy;
        default:
                return nullptr;
        }
}

DspCrackleProxy* DspProxyVst::getCrackle(CrackleId id) const
{
        switch(id) {
        case CrackleId::Crackle1:
                return dspCrackle1Proxy;
        case CrackleId::Crackle2:
                return dspCrackle2Proxy;
        default:
                return nullptr;
        }
}

DspGlitchProxy* DspProxyVst::getGlitch(GlitchId id) const
{
        switch(id) {
        case GlitchId::Glitch1:
                return dspGlitch1Proxy;
        case GlitchId::Glitch2:
                return dspGlitch2Proxy;
        default:
                return nullptr;
        }
}

void DspProxyVst::onParameterChanged(ParameterId paramId, ParamValue value)
{
        switch (paramId) {
        case ParameterId::PlayModeId:
                action playModeUpdated(playModeFromNormalized(value));
                break;
        default:
                break;
        }
}

double DspProxyVst::playModeToNormalized(PlayMode mode) const
{
        auto numPlayModes = static_cast<double>(PlayMode::OnMode);
        return static_cast<double>(mode) / numPlayModes;
}

PlayMode DspProxyVst::playModeFromNormalized(double value) const
{
        ENTROPICTRON_LOG_INFO("PLAY_MODE: " << value);
        auto numPlayModes = static_cast<double>(PlayMode::OnMode);
        return static_cast<PlayMode>(std::round(value * numPlayModes));
}
