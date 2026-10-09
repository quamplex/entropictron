/**
 * File name: DspGlitchProxyVst.cpp
 * Project: Entropictron (A context generator and audio effect)
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

#include "globals.h"
#include "DspGlitchProxyVst.h"
#include "EntVstController.h"
#include "EntState.h"

using namespace EntVst;
using namespace Steinberg::Vst;

DspGlitchProxyVst::DspGlitchProxyVst(RkObject* parent,
                                     EntVstController *controller)
        : DspGlitchProxy(parent)
        , vstController{controller}
{
        auto paramCallback = [this](ParameterId paramId, ParamValue value){
                onParameterChanged(paramId, value);
        };

        std::vector<EntVst::ParameterId> params {
                ParameterId::GlitchEnabledId,
                ParameterId::GlitchRepeatsId,
                ParameterId::GlitchProbabilityId,
                ParameterId::GlitchLengthId,
                ParameterId::GlitchMaxJumpId,
                ParameterId::GlitchMinJumpId,
                ParameterId::GlitchDryId,
                ParameterId::GlitchWetId
        };

        vstController->setParamNormalized(ParameterId::GlitchEnabledId, 0);
        vstController->setParamNormalized(ParameterId::GlitchRepeatsId,
                                          repeatsToNormalized(ENT_GLITCH_DEFAULT_REPEATS));
        vstController->setParamNormalized(ParameterId::GlitchProbabilityId,
                                          probabilityToNormalized(ENT_GLITCH_DEFAULT_PROB));
        vstController->setParamNormalized(ParameterId::GlitchLengthId,
                                          lengthToNormalized(ENT_GLITCH_DEFAULT_LENGH));
        vstController->setParamNormalized(ParameterId::GlitchMinJumpId,
                                          minJumpToNormalized(ENT_GLITCH_DEFAULT_MIN_JUMP));
        vstController->setParamNormalized(ParameterId::GlitchMaxJumpId,
                                          maxJumpToNormalized(ENT_GLITCH_DEFAULT_MAX_JUMP));
        vstController->setParamNormalized(ParameterId::GlitchDryId,
                                          dryToNormalized(ENT_GLITCH_DEFAULT_DRY));
        vstController->setParamNormalized(ParameterId::GlitchWetId,
                                          wetToNormalized(ENT_GLITCH_DEFAULT_WET));

        for (const auto& paramId : params)
                vstController->setParamterCallback(paramId, paramCallback);
}

DspGlitchProxyVst::~DspGlitchProxyVst()
{
        vstController->removeParamterCallback(ParameterId::GlitchEnabledId);
        vstController->removeParamterCallback(ParameterId::GlitchRepeatsId);
        vstController->removeParamterCallback(ParameterId::GlitchProbabilityId);
        vstController->removeParamterCallback(ParameterId::GlitchLengthId);
        vstController->removeParamterCallback(ParameterId::GlitchMaxJumpId);
        vstController->removeParamterCallback(ParameterId::GlitchMinJumpId);
        vstController->removeParamterCallback(ParameterId::GlitchDryId);
        vstController->removeParamterCallback(ParameterId::GlitchWetId);
}

bool DspGlitchProxyVst::enable(bool b)
{
        auto id = ParameterId::GlitchEnabledId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, b ? 1.0 : 0.0);
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

bool DspGlitchProxyVst::isEnabled() const
{
        return vstController->getParamNormalized(ParameterId::GlitchEnabledId) > 0.5;
}

bool DspGlitchProxyVst::setRepeats(int value)
{
        auto id = ParameterId::GlitchRepeatsId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, repeatsToNormalized(value));
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

int DspGlitchProxyVst::repeats() const
{
        return repeatsFromNormalized(vstController->getParamNormalized(ParameterId::GlitchRepeatsId));
}

bool DspGlitchProxyVst::setProbability(double value)
{
        auto id = ParameterId::GlitchProbabilityId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, probabilityToNormalized(value));
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

double DspGlitchProxyVst::probability() const
{
        return probabilityFromNormalized(vstController->getParamNormalized(ParameterId::GlitchProbabilityId));
}

bool DspGlitchProxyVst::setLength(double value)
{
        auto id = ParameterId::GlitchLengthId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, lengthToNormalized(value));
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

double DspGlitchProxyVst::length() const
{
        return lengthFromNormalized(vstController->getParamNormalized(ParameterId::GlitchLengthId));
}

bool DspGlitchProxyVst::setMaxJump(double value)
{
        auto id = ParameterId::GlitchMaxJumpId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, maxJumpToNormalized(value));
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

double DspGlitchProxyVst::maxJump() const
{
        return maxJumpFromNormalized(vstController->getParamNormalized(ParameterId::GlitchMaxJumpId));
}

bool DspGlitchProxyVst::setMinJump(double value)
{
        auto id = ParameterId::GlitchMinJumpId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, minJumpToNormalized(value));
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

double DspGlitchProxyVst::minJump() const
{
        return minJumpFromNormalized(vstController->getParamNormalized(ParameterId::GlitchMinJumpId));
}

bool DspGlitchProxyVst::setDry(double value)
{
        auto id = ParameterId::GlitchDryId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, dryToNormalized(value));
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

double DspGlitchProxyVst::dry() const
{
        return dryFromNormalized(vstController->getParamNormalized(ParameterId::GlitchDryId));
}

bool DspGlitchProxyVst::setWet(double value)
{
        auto id = ParameterId::GlitchWetId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, wetToNormalized(value));
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

double DspGlitchProxyVst::wet() const
{
        return wetFromNormalized(vstController->getParamNormalized(ParameterId::GlitchWetId));
}

void DspGlitchProxyVst::onParameterChanged(ParameterId paramId, ParamValue value)
{
        switch (paramId) {
        case ParameterId::GlitchEnabledId:
                action enabled(value > 0.5);
                break;
        case ParameterId::GlitchRepeatsId:
                action repeatsUpdated(value);
                break;
        case ParameterId::GlitchProbabilityId:
                action probabilityUpdated(value);
                break;
        case ParameterId::GlitchLengthId:
                action lengthUpdated(value);
                break;
        case ParameterId::GlitchMaxJumpId:
                action maxJumpUpdated(value);
                break;
        case ParameterId::GlitchMinJumpId:
                action minJumpUpdated(value);
                break;
        case ParameterId::GlitchDryId:
                action dryUpdated(value);
                break;
        case ParameterId::GlitchWetId:
                action wetUpdated(value);
                break;
        default:
                break;
        }
}

double DspGlitchProxyVst::repeatsToNormalized(int value)
{
        return toNormalized(static_cast<double>(value),
                            ENT_GLITCH_MIN_REPEATS,
                            ENT_GLITCH_MAX_REPEATS);
}

int DspGlitchProxyVst::repeatsFromNormalized(double normalized)
{
        return static_cast<int>(std::round(fromNormalized(normalized,
                                                          ENT_GLITCH_MIN_REPEATS,
                                                          ENT_GLITCH_MAX_REPEATS)));
}

double DspGlitchProxyVst::probabilityToNormalized(double value)
{
        return toNormalized(value,
                            ENT_GLITCH_MIN_PROB,
                            ENT_GLITCH_MAX_PROB);
}

double DspGlitchProxyVst::probabilityFromNormalized(double normalized)
{
        return fromNormalized(normalized,
                              ENT_GLITCH_MIN_PROB,
                              ENT_GLITCH_MAX_PROB);
}

double DspGlitchProxyVst::lengthToNormalized(double value)
{
        return toNormalized(value,
                            ENT_GLITCH_MIN_LENGH,
                            ENT_GLITCH_MAX_LENGH);
}

double DspGlitchProxyVst::lengthFromNormalized(double normalized)
{
        return fromNormalized(normalized,
                              ENT_GLITCH_MIN_LENGH,
                              ENT_GLITCH_MAX_LENGH);
}

double DspGlitchProxyVst::minJumpToNormalized(double value)
{
        return toNormalized(value,
                            ENT_GLITCH_MIN_MIN_JUMP,
                            ENT_GLITCH_MAX_MIN_JUMP);
}

double DspGlitchProxyVst::minJumpFromNormalized(double normalized)
{
        return fromNormalized(normalized,
                              ENT_GLITCH_MIN_MIN_JUMP,
                              ENT_GLITCH_MAX_MIN_JUMP);
}

double DspGlitchProxyVst::maxJumpToNormalized(double value)
{
        return toNormalized(value,
                            ENT_GLITCH_MIN_MAX_JUMP,
                            ENT_GLITCH_MAX_MAX_JUMP);
}

double DspGlitchProxyVst::maxJumpFromNormalized(double normalized)
{
        return fromNormalized(normalized,
                              ENT_GLITCH_MIN_MAX_JUMP,
                              ENT_GLITCH_MAX_MAX_JUMP);
}

double DspGlitchProxyVst::dryToNormalized(double value)
{
        return toNormalized(value,
                            ENT_GLITCH_MIN_DRY,
                            ENT_GLITCH_MAX_DRY);
}

double DspGlitchProxyVst::dryFromNormalized(double normalized)
{
        return fromNormalized(normalized,
                              ENT_GLITCH_MIN_DRY,
                              ENT_GLITCH_MAX_DRY);
}

double DspGlitchProxyVst::wetToNormalized(double value)
{
        return toNormalized(value,
                            ENT_GLITCH_MIN_WET,
                            ENT_GLITCH_MAX_WET);
}

double DspGlitchProxyVst::wetFromNormalized(double normalized)
{
        return fromNormalized(normalized,
                              ENT_GLITCH_MIN_WET,
                              ENT_GLITCH_MAX_WET);
}
