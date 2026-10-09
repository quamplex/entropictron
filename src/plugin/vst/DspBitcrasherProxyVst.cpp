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
        return vstController->editParameter(ParameterId::BitcrasherEnabledId,
                                            value ? 1.0 : 0.0);
}

bool DspBitcrasherProxyVst::isEnabled() const
{
        return vstController->getParamNormalized(ParameterId::BitcrasherEnabledId) > 0.5;
}

bool DspBitcrasherProxyVst::setBits(int value)
{
        return vstController->editParameter(ParameterId::BitcrasherBitsId,
                                            bitcrasherBitsToNormalized(value));
}

int DspBitcrasherProxyVst::bits() const
{
        return bitcrasherBitsFromNormalized(
                vstController->getParamNormalized(ParameterId::BitcrasherBitsId));
}

bool DspBitcrasherProxyVst::setRate(double value)
{
        return vstController->editParameter(ParameterId::BitcrasherRateId,
                                            rateToNormalized(value));
}

double DspBitcrasherProxyVst::rate() const
{
        return rateFromNormalized(
                vstController->getParamNormalized(ParameterId::BitcrasherRateId));
}

bool DspBitcrasherProxyVst::setChaos(double value)
{
        return vstController->editParameter(ParameterId::BitcrasherChaosId,
                                            value);
}

double DspBitcrasherProxyVst::chaos() const
{
        return vstController->getParamNormalized(ParameterId::BitcrasherChaosId);
}

bool DspBitcrasherProxyVst::setGain(double value)
{
        return vstController->editParameter(ParameterId::BitcrasherGainId,
                                            gainToNormalized(value));
}

double DspBitcrasherProxyVst::gain() const
{
        return gainFromNormalized(
                vstController->getParamNormalized(ParameterId::BitcrasherGainId));
}

bool DspBitcrasherProxyVst::setMix(double value)
{
        return vstController->editParameter(ParameterId::BitcrasherMixId, value);
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
