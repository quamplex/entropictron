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
#include "DspNoiseProxyVst.h"
#include "DspCrackleProxyVst.h"
#include "DspGlitchProxyVst.h"
#include "EntState.h"

#include "pluginterfaces/base/ibstream.h"

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
        vstController->setStateCallback([this]() {
                action stateChanged();
        });

        auto paramCallback = [this](ParameterId paramId, ParamValue value){
                onParameterChanged(paramId, value);
        };

        vstController->setParamterCallback(ParameterId::PlayModeId, paramCallback);
        vstController->setParamterCallback(ParameterId::EntropyRateId, paramCallback);
        vstController->setParamterCallback(ParameterId::EntropyDepthId, paramCallback);
        vstController->setParamterCallback(ParameterId::StateChangedId, paramCallback);
}

DspProxyVst::~DspProxyVst()
{
        vstController->removeParamterCallback(ParameterId::StateChangedId);
        vstController->removeParamterCallback(ParameterId::EntropyRateId);
        vstController->removeParamterCallback(ParameterId::EntropyDepthId);
        vstController->removeParamterCallback(ParameterId::PlayModeId);
        vstController->clearStateCallback();
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

bool DspProxyVst::setEntropyRate(double rate)
{
        vstController->getComponentHandler()->beginEdit(ParameterId::EntropyRateId);
        vstController->getComponentHandler()->performEdit(ParameterId::EntropyRateId,
                                                          rate);
        vstController->getComponentHandler()->endEdit(ParameterId::EntropyRateId);
        return true;
}

double DspProxyVst::getEntropyRate() const
{
        return vstController->getParamNormalized(ParameterId::EntropyRateId);
}

bool DspProxyVst::setEntropyDepth(double depth)
{
        vstController->getComponentHandler()->beginEdit(ParameterId::EntropyDepthId);
        vstController->getComponentHandler()->performEdit(ParameterId::EntropyDepthId,
                                                          depth);
        vstController->getComponentHandler()->endEdit(ParameterId::EntropyDepthId);
        return true;
}

double DspProxyVst::getEntropyDepth() const
{
        return vstController->getParamNormalized(ParameterId::EntropyDepthId);
}

double DspProxyVst::getEntropy() const
{
        return entropyFromNormalized(vstController->getParamNormalized(ParameterId::EntropyMeterId));
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
        case ParameterId::StateChangedId:
                action stateChanged();
                break;
        case ParameterId::PlayModeId:
                action playModeUpdated(playModeFromNormalized(value));
                break;
        case ParameterId::EntropyRateId:
                action entropyRateUpdated(value);
                break;
        case ParameterId::EntropyDepthId:
                action entropyDepthUpdated(value);
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

double DspProxyVst::playModeToNormalized(PlayMode mode)
{
        auto numPlayModes = static_cast<double>(PlayMode::OnMode);
        return static_cast<double>(mode) / numPlayModes;
}

PlayMode DspProxyVst::playModeFromNormalized(double value)
{
        auto numPlayModes = static_cast<double>(PlayMode::OnMode);
        return static_cast<PlayMode>(std::round(value * numPlayModes));
}

double DspProxyVst::entropyToNormalized(double val)
{
        return toNormalized(val, -1.0, 1.0);
}

double DspProxyVst::entropyFromNormalized(double val)
{
        return fromNormalized(val, -1.0, 1.0);
}

