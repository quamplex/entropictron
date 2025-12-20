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

#include "globals.h"
#include "DspCrackleProxyVst.h"
#include "EntVstController.h"
#include "EntState.h"

using namespace EntVst;
using namespace Steinberg::Vst;

DspCrackleProxyVst::DspCrackleProxyVst(RkObject* parent,
                                       EntVstController *controller,
                                       CrackleId id)
        : DspCrackleProxy(parent, id)
        , vstController{controller}
{
        vstController->addStateCallback([this](const EntState &state){
                setState(state);
        });

        auto paramCallback = [this](ParameterId paramId, ParamValue value){
                onParameterChanged(paramId, value);
        };

        std::vector<EntVst::ParameterId> params;
        if (getCrackleId() == CrackleId::Crackle1) {
                params = {
                        ParameterId::Crackle1RateId,
                        ParameterId::Crackle1DurationId,
                        ParameterId::Crackle1AmplitudeId,
                        ParameterId::Crackle1RandomnessId,
                        ParameterId::Crackle1BrightnessId,
                        ParameterId::Crackle1EnvelopeShapeId,
                        ParameterId::Crackle1StereoSpreadId
                };

                vstController->setParamNormalized(ParameterId::Crackle1EnabledId, 0.0);
                vstController->setParamNormalized(ParameterId::Crackle1RateId,
                                                  rateToNormalized(20.0));
                vstController->setParamNormalized(ParameterId::Crackle1DurationId,
                                                  durationToNormalized(1.0));
                vstController->setParamNormalized(ParameterId::Crackle1AmplitudeId, 1.0);
                vstController->setParamNormalized(ParameterId::Crackle1RandomnessId, 1.0);
                vstController->setParamNormalized(ParameterId::Crackle1BrightnessId, 0.0);
                vstController->setParamNormalized(ParameterId::Crackle1EnvelopeShapeId,
                                                  envelopeShapeToNormalized(CrackleEnvelopeShape::Exponential));
                vstController->setParamNormalized(ParameterId::Crackle1StereoSpreadId, 0.0);

        } else {
                params = {
                        ParameterId::Crackle2RateId,
                        ParameterId::Crackle2DurationId,
                        ParameterId::Crackle2AmplitudeId,
                        ParameterId::Crackle2RandomnessId,
                        ParameterId::Crackle2BrightnessId,
                        ParameterId::Crackle2EnvelopeShapeId,
                        ParameterId::Crackle2StereoSpreadId
                };

                vstController->setParamNormalized(ParameterId::Crackle2EnabledId, 0.0);
                vstController->setParamNormalized(ParameterId::Crackle2RateId,
                                                  rateToNormalized(20.0));
                vstController->setParamNormalized(ParameterId::Crackle2DurationId,
                                                  durationToNormalized(1.0));
                vstController->setParamNormalized(ParameterId::Crackle2AmplitudeId, 1.0);
                vstController->setParamNormalized(ParameterId::Crackle2RandomnessId, 1.0);
                vstController->setParamNormalized(ParameterId::Crackle2BrightnessId, 0.0);
                vstController->setParamNormalized(ParameterId::Crackle2EnvelopeShapeId,
                                                  envelopeShapeToNormalized(CrackleEnvelopeShape::Exponential));
                vstController->setParamNormalized(ParameterId::Crackle2StereoSpreadId, 0.0);
        }

        for (const auto& paramId : params)
                vstController->setParamterCallback(paramId, paramCallback);
}

DspCrackleProxyVst::~DspCrackleProxyVst()
{
        if (getCrackleId() == CrackleId::Crackle1) {
                vstController->removeParamterCallback(ParameterId::Crackle1RateId);
                vstController->removeParamterCallback(ParameterId::Crackle1DurationId);
                vstController->removeParamterCallback(ParameterId::Crackle1AmplitudeId);
                vstController->removeParamterCallback(ParameterId::Crackle1RandomnessId);
                vstController->removeParamterCallback(ParameterId::Crackle1BrightnessId);
                vstController->removeParamterCallback(ParameterId::Crackle1EnvelopeShapeId);
                vstController->removeParamterCallback(ParameterId::Crackle1StereoSpreadId);
        } else {
                vstController->removeParamterCallback(ParameterId::Crackle2RateId);
                vstController->removeParamterCallback(ParameterId::Crackle2DurationId);
                vstController->removeParamterCallback(ParameterId::Crackle2AmplitudeId);
                vstController->removeParamterCallback(ParameterId::Crackle2RandomnessId);
                vstController->removeParamterCallback(ParameterId::Crackle2BrightnessId);
                vstController->removeParamterCallback(ParameterId::Crackle2EnvelopeShapeId);
                vstController->removeParamterCallback(ParameterId::Crackle2StereoSpreadId);
        }
}

bool DspCrackleProxyVst::enable(bool b)
{
        ENT_LOG_DEBUG("called: ID : " << (int)getCrackleId());
        auto paramId = (getCrackleId() == CrackleId::Crackle1) ? ParameterId::Crackle1EnabledId : ParameterId::Crackle2EnabledId;
        vstController->getComponentHandler()->beginEdit(paramId);
        vstController->getComponentHandler()->performEdit(paramId, b ? 1.0 : 0.0);
        vstController->getComponentHandler()->endEdit(paramId);
        return true;
}

bool DspCrackleProxyVst::isEnabled() const
{
        auto id = (getCrackleId() == CrackleId::Crackle1) ?
                ParameterId::Crackle1EnabledId : ParameterId::Crackle2EnabledId;
        return vstController->getParamNormalized(id) > 0.5;
}

bool DspCrackleProxyVst::setRate(double value)
{
        ENT_LOG_INFO("value: " << value);
        auto id = (getCrackleId() == CrackleId::Crackle1) ?
                ParameterId::Crackle1RateId : ParameterId::Crackle2RateId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, rateToNormalized(value));
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

double DspCrackleProxyVst::rate() const
{
        auto id = (getCrackleId() == CrackleId::Crackle1) ?
                ParameterId::Crackle1RateId : ParameterId::Crackle2RateId;
        return rateFromNormalized(vstController->getParamNormalized(id));
}

bool DspCrackleProxyVst::setDuration(double value)
{
        auto id = (getCrackleId() == CrackleId::Crackle1) ?
                ParameterId::Crackle1DurationId : ParameterId::Crackle2DurationId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, durationToNormalized(value));
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

double DspCrackleProxyVst::duration() const
{
        auto id = (getCrackleId() == CrackleId::Crackle1) ?
                ParameterId::Crackle1DurationId : ParameterId::Crackle2DurationId;
        return durationFromNormalized(vstController->getParamNormalized(id));
}

bool DspCrackleProxyVst::setAmplitude(double value)
{
        auto id = (getCrackleId() == CrackleId::Crackle1) ?
                ParameterId::Crackle1AmplitudeId : ParameterId::Crackle2AmplitudeId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, value);
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

double DspCrackleProxyVst::amplitude() const
{
        auto id = (getCrackleId() == CrackleId::Crackle1) ?
                ParameterId::Crackle1AmplitudeId : ParameterId::Crackle2AmplitudeId;
        return vstController->getParamNormalized(id);
}

bool DspCrackleProxyVst::setRandomness(double value)
{
        auto id = (getCrackleId() == CrackleId::Crackle1) ?
                ParameterId::Crackle1RandomnessId : ParameterId::Crackle2RandomnessId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, value);
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

double DspCrackleProxyVst::randomness() const
{
        auto id = (getCrackleId() == CrackleId::Crackle1) ?
                ParameterId::Crackle1RandomnessId : ParameterId::Crackle2RandomnessId;
        return vstController->getParamNormalized(id);
}

bool DspCrackleProxyVst::setBrightness(double value)
{
        auto id = (getCrackleId() == CrackleId::Crackle1) ?
                ParameterId::Crackle1BrightnessId : ParameterId::Crackle2BrightnessId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, value);
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

double DspCrackleProxyVst::brightness() const
{
        auto id = (getCrackleId() == CrackleId::Crackle1) ?
                ParameterId::Crackle1BrightnessId : ParameterId::Crackle2BrightnessId;
        return vstController->getParamNormalized(id);
}

bool DspCrackleProxyVst::setEnvelopeShape(CrackleEnvelopeShape shape)
{
        auto id = (getCrackleId() == CrackleId::Crackle1) ?
                ParameterId::Crackle1EnvelopeShapeId : ParameterId::Crackle2EnvelopeShapeId;
        vstController->getComponentHandler()->beginEdit(id);
        auto value = envelopeShapeToNormalized(shape);
        vstController->getComponentHandler()->performEdit(id, value);
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

CrackleEnvelopeShape DspCrackleProxyVst::envelopeShape() const
{
        auto id = (getCrackleId() == CrackleId::Crackle1) ?
                ParameterId::Crackle1EnvelopeShapeId : ParameterId::Crackle2EnvelopeShapeId;
        double value = vstController->getParamNormalized(id);
        return envelopeShapeFromNormalized(value);
}

bool DspCrackleProxyVst::setStereospread(double value)
{
        auto id = (getCrackleId() == CrackleId::Crackle1) ?
                ParameterId::Crackle1StereoSpreadId : ParameterId::Crackle2StereoSpreadId;
        vstController->getComponentHandler()->beginEdit(id);
        vstController->getComponentHandler()->performEdit(id, value);
        vstController->getComponentHandler()->endEdit(id);
        return true;
}

double DspCrackleProxyVst::stereospread() const
{
        auto id = (getCrackleId() == CrackleId::Crackle1) ?
                ParameterId::Crackle1StereoSpreadId : ParameterId::Crackle2StereoSpreadId;
        return vstController->getParamNormalized(id);
}

void DspCrackleProxyVst::onParameterChanged(ParameterId paramId, ParamValue value)
{
        switch (paramId) {
        case ParameterId::Crackle1EnabledId:
        case ParameterId::Crackle2EnabledId:
                action enabled(value > 0.5);
                break;
        case ParameterId::Crackle1RateId:
        case ParameterId::Crackle2RateId:
                action rateUpdated(rateFromNormalized(value));
                break;
        case ParameterId::Crackle1DurationId:
        case ParameterId::Crackle2DurationId:
                action durationUpdated(durationFromNormalized(value));
                break;
        case ParameterId::Crackle1AmplitudeId:
        case ParameterId::Crackle2AmplitudeId:
                action amplitudeUpdated(value);
                break;
        case ParameterId::Crackle1RandomnessId:
        case ParameterId::Crackle2RandomnessId:
                action rateUpdated(value);
                break;
        case ParameterId::Crackle1BrightnessId:
        case ParameterId::Crackle2BrightnessId:
                action rateUpdated(value);
                break;
        case ParameterId::Crackle1EnvelopeShapeId:
        case ParameterId::Crackle2EnvelopeShapeId:
                action envelopeShapeUpdated(envelopeShapeFromNormalized(value));
                break;
        case ParameterId::Crackle1StereoSpreadId:
        case ParameterId::Crackle2StereoSpreadId:
                action stereospreadUpdated(value);
                break;
        default:
                break;
        }
}

double DspCrackleProxyVst::rateToNormalized(double value)
{
        //[0.5Hz - 150Hz] -> [0, 1]
        return (value - 0.5) / (150 - 0.5);
}

double DspCrackleProxyVst::rateFromNormalized(double value)
{
        // [0, 1] -> [0.5Hz - 150Hz]
        return 0.5 + (150.0 - 0.5) * value;
}

double DspCrackleProxyVst::durationToNormalized(double value)
{
        // 0.1 - 50 ms -> [0, 1]
        return (value - 0.1) / (50.0 - 0.1);
}

double DspCrackleProxyVst::durationFromNormalized(double value)
{
        // [0, 1] -> 0.1 - 50 ms
        return 0.1 + (50.0 - 0.1) * value;
}

double DspCrackleProxyVst::envelopeShapeToNormalized(CrackleEnvelopeShape shape)
{
        auto n = static_cast<double>(CrackleEnvelopeShapeMax);
        return static_cast<double>(shape) / n;
}

CrackleEnvelopeShape DspCrackleProxyVst::envelopeShapeFromNormalized(double value)
{
        auto n = static_cast<double>(CrackleEnvelopeShapeMax);
        return static_cast<CrackleEnvelopeShape>(std::round(value * n));
}


