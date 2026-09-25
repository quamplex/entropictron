/**
 * File name: DspBitcrasherProxy.h
 * Project: Entropictron (A texture synthesizer)
 */

#ifndef DSP_BITCRASHER_PROXY_H
#define DSP_BITCRASHER_PROXY_H

#include "RkObject.h"

class DspBitcrasherProxy : public RkObject
{
public:
        explicit DspBitcrasherProxy(RkObject *parent = nullptr);
        virtual bool enable(bool enabled = true) = 0;
        virtual bool isEnabled() const = 0;
        virtual bool setBits(int value) = 0;
        virtual int bits() const = 0;
        virtual bool setRate(double value) = 0;
        virtual double rate() const = 0;
        virtual bool setChaos(double value) = 0;
        virtual double chaos() const = 0;
        virtual bool setGain(double value) = 0;
        virtual double gain() const = 0;
        virtual bool setMix(double value) = 0;
        virtual double mix() const = 0;

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
        RK_DECL_ACT(gainUpdated,
                    gainUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(mixUpdated,
                    mixUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
};

#endif // DSP_BITCRASHER_PROXY_H
