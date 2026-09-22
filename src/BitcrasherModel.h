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
        void setBitsDefaultValue(int value);
        int getBitsDefaultValue() const;
        void setBitsRange(int from, int to);
        std::pair<int, int> getBitsRange() const;
        void setRate(double value);
        double rate() const;
        void setRateDefaultValue(double value);
        double getRateDefaultValue() const;
        void setRateRange(double from, double to);
        std::pair<double, double> getRateRange() const;
        void setChaos(double value);
        double chaos() const;
        void setChaosDefaultValue(double value);
        double getChaosDefaultValue() const;
        void setChaosRange(double from, double to);
        std::pair<double, double> getChaosRange() const;
        void setFold(double value);
        double fold() const;
        void setFoldDefaultValue(double value);
        double getFoldDefaultValue() const;
        void setFoldRange(double from, double to);
        std::pair<double, double> getFoldRange() const;
        void setMix(double value);
        double mix() const;
        void setMixDefaultValue(double value);
        double getMixDefaultValue() const;
        void setMixRange(double from, double to);
        std::pair<double, double> getMixRange() const;

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
        int bitsDefaultValue;
        std::pair<int, int> bitsRange;
        double rateDefaultValue;
        std::pair<double, double> rateRange;
        double chaosDefaultValue;
        std::pair<double, double> chaosRange;
        double foldDefaultValue;
        std::pair<double, double> foldRange;
        double mixDefaultValue;
        std::pair<double, double> mixRange;
};

#endif // ENT_BITCRASHER_MODEL_H
