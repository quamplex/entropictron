/**
 * File name: BitcrasherModel.h
 * Project: Entropictron (A texture synthesizer)
 */

#ifndef ENT_BITCRASHER_MODEL_H
#define ENT_BITCRASHER_MODEL_H

#include "EntAbstractModel.h"

class DspBitcrasherProxy;

class BitcrasherModel : public EntAbstractModel
{
public:
        explicit BitcrasherModel(RkObject *parent,
                                 DspBitcrasherProxy *proxy);
        void enable(bool value = true);
        bool isEnabled() const;
        void setBits(int value);
        int bits() const;
        void setRate(double value);
        double rate() const;
        void setChaos(double value);
        double chaos() const;
        void setFold(double value);
        double fold() const;
        void setMix(double value);
        double mix() const;

        RK_DECL_ACT(enabled,
                    enabled(bool value),
                    RK_ARG_TYPE(bool),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(bitsUpdated,
                    bitsUpdated(int value),
                    RK_ARG_TYPE(int),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(rateUpdated,
                    rateUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(chaosUpdated,
                    chaosUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(foldUpdated,
                    foldUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(mixUpdated,
                    mixUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));

private:
        DspBitcrasherProxy *proxy;
};

#endif // ENT_BITCRASHER_MODEL_H
