#include "BitcrasherModel.h"
#include "DspBitcrasherProxy.h"
#include "ent_bitcrasher.h"

BitcrasherModel::BitcrasherModel(RkObject *parent, DspBitcrasherProxy *proxy)
        : EntAbstractModel(parent)
        , proxy{proxy}
        , bitsDefaultValue{ENT_BITCRASHER_DEFAULT_BITS}
        , bitsRange{ENT_BITCRASHER_MIN_BITS, ENT_BITCRASHER_MAX_BITS}
        , rateDefaultValue{ENT_BITCRASHER_DEFAULT_RATE}
        , rateRange{ENT_BITCRASHER_MIN_RATE, ENT_BITCRASHER_MAX_RATE}
        , chaosDefaultValue{ENT_BITCRASHER_DEFAULT_CHAOS}
        , chaosRange{ENT_BITCRASHER_MIN_CHAOS, ENT_BITCRASHER_MAX_CHAOS}
        , foldDefaultValue{ENT_BITCRASHER_DEFAULT_FOLD}
        , foldRange{ENT_BITCRASHER_MIN_FOLD, ENT_BITCRASHER_MAX_FOLD}
        , mixDefaultValue{ENT_BITCRASHER_DEFAULT_MIX}
        , mixRange{ENT_BITCRASHER_MIN_MIX, ENT_BITCRASHER_MAX_MIX}
{
        RK_ACT_BIND(proxy,
                    enabled,
                    RK_ACT_ARGS(bool value),
                    this,
                    enabled(value));
        RK_ACT_BIND(proxy,
                    bitsUpdated,
                    RK_ACT_ARGS(int value),
                    this,
                    bitsUpdated(value));
        RK_ACT_BIND(proxy,
                    rateUpdated,
                    RK_ACT_ARGS(double value),
                    this,
                    rateUpdated(value));
        RK_ACT_BIND(proxy,
                    chaosUpdated,
                    RK_ACT_ARGS(double value),
                    this,
                    chaosUpdated(value));
        RK_ACT_BIND(proxy,
                    foldUpdated,
                    RK_ACT_ARGS(double value),
                    this,
                    foldUpdated(value));
        RK_ACT_BIND(proxy,
                    mixUpdated,
                    RK_ACT_ARGS(double value),
                    this,
                    mixUpdated(value));
}

void BitcrasherModel::enable(bool value)
{
        if (proxy->enable(value))
                action enabled(value);
}

bool BitcrasherModel::isEnabled() const
{
        return proxy->isEnabled();
}

void BitcrasherModel::setBits(int value)
{
        if (proxy->setBits(value))
                action bitsUpdated(value);
}

int BitcrasherModel::bits() const
{
        return proxy->bits();
}

void BitcrasherModel::setBitsDefaultValue(int value)
{
        bitsDefaultValue = value;
}

int BitcrasherModel::getBitsDefaultValue() const
{
        return bitsDefaultValue;
}

void BitcrasherModel::setBitsRange(int from, int to)
{
        bitsRange = {from, to};
}

std::pair<int, int> BitcrasherModel::getBitsRange() const
{
        return bitsRange;
}

void BitcrasherModel::setRate(double value)
{
        if (proxy->setRate(value))
                action rateUpdated(value);
}

double BitcrasherModel::rate() const
{
        return proxy->rate();
}

void BitcrasherModel::setRateDefaultValue(double value)
{
        rateDefaultValue = value;
}

double BitcrasherModel::getRateDefaultValue() const
{
        return rateDefaultValue;
}

void BitcrasherModel::setRateRange(double from, double to)
{
        rateRange = {from, to};
}

std::pair<double, double> BitcrasherModel::getRateRange() const
{
        return rateRange;
}

void BitcrasherModel::setChaos(double value)
{
        if (proxy->setChaos(value))
                action chaosUpdated(value);
}

double BitcrasherModel::chaos() const
{
        return proxy->chaos();
}

void BitcrasherModel::setChaosDefaultValue(double value)
{
        chaosDefaultValue = value;
}

double BitcrasherModel::getChaosDefaultValue() const
{
        return chaosDefaultValue;
}

void BitcrasherModel::setChaosRange(double from, double to)
{
        chaosRange = {from, to};
}

std::pair<double, double> BitcrasherModel::getChaosRange() const
{
        return chaosRange;
}

void BitcrasherModel::setFold(double value)
{
        if (proxy->setFold(value))
                action foldUpdated(value);
}

double BitcrasherModel::fold() const { return proxy->fold(); }

void BitcrasherModel::setFoldDefaultValue(double value)
{
        foldDefaultValue = value;
}

double BitcrasherModel::getFoldDefaultValue() const
{
        return foldDefaultValue;
}

void BitcrasherModel::setFoldRange(double from, double to)
{
        foldRange = {from, to};
}

std::pair<double, double> BitcrasherModel::getFoldRange() const
{
        return foldRange;
}

void BitcrasherModel::setMix(double value)
{
        if (proxy->setMix(value))
                action mixUpdated(value);
}

double BitcrasherModel::mix() const
{
        return proxy->mix();
}

void BitcrasherModel::setMixDefaultValue(double value)
{
        mixDefaultValue = value;
}

double BitcrasherModel::getMixDefaultValue() const
{
        return mixDefaultValue;
}

void BitcrasherModel::setMixRange(double from, double to)
{
        mixRange = {from, to};
}

std::pair<double, double> BitcrasherModel::getMixRange() const
{
        return mixRange;
}
