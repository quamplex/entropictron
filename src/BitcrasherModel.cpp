#include "BitcrasherModel.h"
#include "DspBitcrasherProxy.h"

BitcrasherModel::BitcrasherModel(RkObject *parent, DspBitcrasherProxy *proxy)
        : EntAbstractModel(parent)
        , proxy{proxy}
{
        RK_ACT_BIND(proxy, enabled, RK_ACT_ARGS(bool value), this, enabled(value));
        RK_ACT_BIND(proxy, bitsUpdated, RK_ACT_ARGS(int value), this, bitsUpdated(value));
        RK_ACT_BIND(proxy, rateUpdated, RK_ACT_ARGS(double value), this, rateUpdated(value));
        RK_ACT_BIND(proxy, chaosUpdated, RK_ACT_ARGS(double value), this, chaosUpdated(value));
        RK_ACT_BIND(proxy, foldUpdated, RK_ACT_ARGS(double value), this, foldUpdated(value));
        RK_ACT_BIND(proxy, mixUpdated, RK_ACT_ARGS(double value), this, mixUpdated(value));
}

void BitcrasherModel::enable(bool value)
{
        if (proxy->enable(value))
                action enabled(value);
}

bool BitcrasherModel::isEnabled() const { return proxy->isEnabled(); }

void BitcrasherModel::setBits(int value)
{
        if (proxy->setBits(value))
                action bitsUpdated(value);
}

int BitcrasherModel::bits() const { return proxy->bits(); }

void BitcrasherModel::setRate(double value)
{
        if (proxy->setRate(value))
                action rateUpdated(value);
}

double BitcrasherModel::rate() const { return proxy->rate(); }

void BitcrasherModel::setChaos(double value)
{
        if (proxy->setChaos(value))
                action chaosUpdated(value);
}

double BitcrasherModel::chaos() const { return proxy->chaos(); }

void BitcrasherModel::setFold(double value)
{
        if (proxy->setFold(value))
                action foldUpdated(value);
}

double BitcrasherModel::fold() const { return proxy->fold(); }

void BitcrasherModel::setMix(double value)
{
        if (proxy->setMix(value))
                action mixUpdated(value);
}

double BitcrasherModel::mix() const { return proxy->mix(); }
