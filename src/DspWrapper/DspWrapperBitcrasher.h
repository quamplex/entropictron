/**
 * File name: DspWrapperBitcrasher.h
 * Project: Entropictron (A texture synthesizer)
 *
 * Copyright (C) 2026 Iurie Nistor
 *
 * This file is part of Entropictron.
 */

#ifndef ENT_DSP_WRAPPER_BITCRASHER_H
#define ENT_DSP_WRAPPER_BITCRASHER_H

struct ent_bitcrasher;

class DspWrapperBitcrasher
{
public:
        explicit DspWrapperBitcrasher(struct ent_bitcrasher *dsp);
        void enable(bool enabled = true);
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
        static double bitsToNormalized(int value);
        static int bitsFromNormalized(double value);

private:
        struct ent_bitcrasher *bitcrasherDsp;
};

#endif // ENT_DSP_WRAPPER_BITCRASHER_H
