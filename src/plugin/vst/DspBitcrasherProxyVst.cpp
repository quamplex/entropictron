#include "DspBitcrasherProxyVst.h"
#include "EntVstController.h"
#include "ent_bitcrasher.h"

#include <cmath>

using namespace EntVst;
using namespace Steinberg::Vst;

DspBitcrasherProxyVst::DspBitcrasherProxyVst(RkObject *parent,
                                             EntVstController *controller)
        : DspBitcrasherProxy(parent)
        , vstController{controller}
{
        auto callback = [this](ParameterId id, ParamValue value) {
                onParameterChanged(id, value);
        };

        auto paramsId = {ParameterId::BitcrasherEnabledId,
                         ParameterId::BitcrasherBitsId,
                         ParameterId::BitcrasherRateId,
                         ParameterId::BitcrasherChaosId,
                         ParameterId::BitcrasherGainId,
                         ParameterId::BitcrasherMixId};
        for (auto id : paramsId)
                vstController->setParamterCallback(id, callback);

        vstController->setParamNormalized(ParameterId::BitcrasherEnabledId, 0.0);
        vstController->setParamNormalized(ParameterId::BitcrasherBitsId,
                                          bitcrasherBitsToNormalized(ENT_BITCRASHER_DEFAULT_BITS));
        vstController->setParamNormalized(ParameterId::BitcrasherRateId,
                                          rateToNormalized(
                                                  ENT_BITCRASHER_DEFAULT_RATE));
        vstController->setParamNormalized(ParameterId::BitcrasherChaosId,
                                          ENT_BITCRASHER_DEFAULT_CHAOS);
        vstController->setParamNormalized(
                ParameterId::BitcrasherGainId,
                gainToNormalized(Entropictron::fromDecibel(
                        ENT_BITCRASHER_DEFAULT_GAIN)));
        vstController->setParamNormalized(ParameterId::BitcrasherMixId,
                                          ENT_BITCRASHER_DEFAULT_MIX);
}

DspBitcrasherProxyVst::~DspBitcrasherProxyVst()
{
        auto paramsId = {ParameterId::BitcrasherEnabledId,
                         ParameterId::BitcrasherBitsId,
                         ParameterId::BitcrasherRateId,
                         ParameterId::BitcrasherChaosId,
                         ParameterId::BitcrasherGainId,
                         ParameterId::BitcrasherMixId};
        for (auto id : paramsId)
                vstController->removeParamterCallback(id);
}

bool DspBitcrasherProxyVst::enable(bool value)
{
        auto *handler = vstController->getComponentHandler();
        handler->beginEdit(ParameterId::BitcrasherEnabledId);
        handler->performEdit(ParameterId::BitcrasherEnabledId, value ? 1.0 : 0.0);
        handler->endEdit(ParameterId::BitcrasherEnabledId);
        return true;
}

bool DspBitcrasherProxyVst::isEnabled() const
{
        return vstController->getParamNormalized(ParameterId::BitcrasherEnabledId) > 0.5;
}

bool DspBitcrasherProxyVst::setBits(int value)
{
        auto *handler = vstController->getComponentHandler();
        handler->beginEdit(ParameterId::BitcrasherBitsId);
        handler->performEdit(ParameterId::BitcrasherBitsId,
                             bitcrasherBitsToNormalized(value));
        handler->endEdit(ParameterId::BitcrasherBitsId);
        return true;
}

int DspBitcrasherProxyVst::bits() const
{
        return bitcrasherBitsFromNormalized(
                vstController->getParamNormalized(ParameterId::BitcrasherBitsId));
}

bool DspBitcrasherProxyVst::setRate(double value)
{
        auto *handler = vstController->getComponentHandler();
        handler->beginEdit(ParameterId::BitcrasherRateId);
        handler->performEdit(ParameterId::BitcrasherRateId,
                             rateToNormalized(value));
        handler->endEdit(ParameterId::BitcrasherRateId);
        return true;
}

double DspBitcrasherProxyVst::rate() const
{
        return rateFromNormalized(
                vstController->getParamNormalized(ParameterId::BitcrasherRateId));
}

bool DspBitcrasherProxyVst::setChaos(double value)
{
        auto *handler = vstController->getComponentHandler();
        handler->beginEdit(ParameterId::BitcrasherChaosId);
        handler->performEdit(ParameterId::BitcrasherChaosId, value);
        handler->endEdit(ParameterId::BitcrasherChaosId);
        return true;
}

double DspBitcrasherProxyVst::chaos() const
{
        return vstController->getParamNormalized(ParameterId::BitcrasherChaosId);
}

bool DspBitcrasherProxyVst::setGain(double value)
{
        auto *handler = vstController->getComponentHandler();
        handler->beginEdit(ParameterId::BitcrasherGainId);
        handler->performEdit(ParameterId::BitcrasherGainId,
                             gainToNormalized(value));
        handler->endEdit(ParameterId::BitcrasherGainId);
        return true;
}

double DspBitcrasherProxyVst::gain() const
{
        return gainFromNormalized(
                vstController->getParamNormalized(ParameterId::BitcrasherGainId));
}

bool DspBitcrasherProxyVst::setMix(double value)
{
        auto *handler = vstController->getComponentHandler();
        handler->beginEdit(ParameterId::BitcrasherMixId);
        handler->performEdit(ParameterId::BitcrasherMixId, value);
        handler->endEdit(ParameterId::BitcrasherMixId);
        return true;
}

double DspBitcrasherProxyVst::mix() const
{
        return vstController->getParamNormalized(ParameterId::BitcrasherMixId);
}

void DspBitcrasherProxyVst::onParameterChanged(ParameterId id, ParamValue value)
{
        switch (id) {
        case ParameterId::BitcrasherEnabledId:
                action enabled(value > 0.5);
                break;
        case ParameterId::BitcrasherBitsId:
                action bitsUpdated(bitcrasherBitsFromNormalized(value));
                break;
        case ParameterId::BitcrasherRateId:
                action rateUpdated(rateFromNormalized(value));
                break;
        case ParameterId::BitcrasherChaosId:
                action chaosUpdated(value);
                break;
        case ParameterId::BitcrasherGainId:
                action gainUpdated(gainFromNormalized(value));
                break;
        case ParameterId::BitcrasherMixId:
                action mixUpdated(value);
                break;
        default: break;
        }
}

double DspBitcrasherProxyVst::bitcrasherBitsToNormalized(int value)
{
        return toNormalized(static_cast<double>(value),
                            ENT_BITCRASHER_MIN_BITS,
                            ENT_BITCRASHER_MAX_BITS);
}

int DspBitcrasherProxyVst::bitcrasherBitsFromNormalized(double value)
{
        return static_cast<int>(std::round(fromNormalized(value,
                                                          ENT_BITCRASHER_MIN_BITS,
                                                          ENT_BITCRASHER_MAX_BITS)));
}

double DspBitcrasherProxyVst::rateToNormalized(double value)
{
        return toNormalized(value,
                            ENT_BITCRASHER_MIN_RATE,
                            ENT_BITCRASHER_MAX_RATE);
}

double DspBitcrasherProxyVst::rateFromNormalized(double value)
{
        return fromNormalized(value,
                              ENT_BITCRASHER_MIN_RATE,
                              ENT_BITCRASHER_MAX_RATE);
}

double DspBitcrasherProxyVst::gainFromNormalized(double value)
{
        return fromNormalized(value,
                              Entropictron::fromDecibel(ENT_BITCRASHER_MIN_GAIN),
                              Entropictron::fromDecibel(ENT_BITCRASHER_MAX_GAIN));
}

double DspBitcrasherProxyVst::gainToNormalized(double value)
{
        return toNormalized(value,
                            Entropictron::fromDecibel(ENT_BITCRASHER_MIN_GAIN),
                            Entropictron::fromDecibel(ENT_BITCRASHER_MAX_GAIN));
}
