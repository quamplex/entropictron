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
        for (auto id : {ParameterId::BitcrasherEnabledId,
                        ParameterId::BitcrasherBitsId,
                        ParameterId::BitcrasherRateId,
                        ParameterId::BitcrasherChaosId,
                        ParameterId::BitcrasherFoldId,
                        ParameterId::BitcrasherMixId})
                vstController->setParamterCallback(id, callback);
}

DspBitcrasherProxyVst::~DspBitcrasherProxyVst()
{
        for (auto id : {ParameterId::BitcrasherEnabledId,
                        ParameterId::BitcrasherBitsId,
                        ParameterId::BitcrasherRateId,
                        ParameterId::BitcrasherChaosId,
                        ParameterId::BitcrasherFoldId,
                        ParameterId::BitcrasherMixId})
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
        handler->performEdit(ParameterId::BitcrasherBitsId, bitsToNormalized(value));
        handler->endEdit(ParameterId::BitcrasherBitsId);
        return true;
}

int DspBitcrasherProxyVst::bits() const
{
        return bitsFromNormalized(
                vstController->getParamNormalized(ParameterId::BitcrasherBitsId));
}

bool DspBitcrasherProxyVst::setRate(double value)
{
        auto *handler = vstController->getComponentHandler();
        handler->beginEdit(ParameterId::BitcrasherRateId);
        handler->performEdit(ParameterId::BitcrasherRateId, value);
        handler->endEdit(ParameterId::BitcrasherRateId);
        return true;
}

double DspBitcrasherProxyVst::rate() const
{
        return vstController->getParamNormalized(ParameterId::BitcrasherRateId);
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

bool DspBitcrasherProxyVst::setFold(double value)
{
        auto *handler = vstController->getComponentHandler();
        handler->beginEdit(ParameterId::BitcrasherFoldId);
        handler->performEdit(ParameterId::BitcrasherFoldId, value);
        handler->endEdit(ParameterId::BitcrasherFoldId);
        return true;
}

double DspBitcrasherProxyVst::fold() const
{
        return vstController->getParamNormalized(ParameterId::BitcrasherFoldId);
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
        case ParameterId::BitcrasherEnabledId: action enabled(value > 0.5); break;
        case ParameterId::BitcrasherBitsId: action bitsUpdated(bitsFromNormalized(value)); break;
        case ParameterId::BitcrasherRateId: action rateUpdated(value); break;
        case ParameterId::BitcrasherChaosId: action chaosUpdated(value); break;
        case ParameterId::BitcrasherFoldId: action foldUpdated(value); break;
        case ParameterId::BitcrasherMixId: action mixUpdated(value); break;
        default: break;
        }
}

double DspBitcrasherProxyVst::bitsToNormalized(int value)
{
        return toNormalized(value, ENT_BITCRASHER_MIN_BITS, ENT_BITCRASHER_MAX_BITS);
}

int DspBitcrasherProxyVst::bitsFromNormalized(double value)
{
        return static_cast<int>(std::round(fromNormalized(
                value, ENT_BITCRASHER_MIN_BITS, ENT_BITCRASHER_MAX_BITS)));
}
