/**
 * File name: DspWrapperBitcrasher.cpp
 * Project: Entropictron (A texture synthesizer)
 *
 * Copyright (C) 2026 Iurie Nistor
 *
 * This file is part of Entropictron.
 */

#include "DspWrapperBitcrasher.h"
#include "ent_bitcrasher.h"

#include <cmath>

DspWrapperBitcrasher::DspWrapperBitcrasher(struct ent_bitcrasher *dsp)
        : bitcrasherDsp{dsp}
{
}

void DspWrapperBitcrasher::enable(bool enabled)
{
        ent_bitcrasher_enable(bitcrasherDsp, enabled);
}

bool DspWrapperBitcrasher::isEnabled() const
{
        return ent_bitcrasher_is_enabled(bitcrasherDsp);
}

void DspWrapperBitcrasher::setBits(int value)
{
        ent_bitcrasher_set_bits(bitcrasherDsp, value);
}

int DspWrapperBitcrasher::bits() const
{
        return ent_bitcrasher_get_bits(bitcrasherDsp);
}

void DspWrapperBitcrasher::setRate(double value)
{
        ent_bitcrasher_set_rate(bitcrasherDsp, value);
}

double DspWrapperBitcrasher::rate() const
{
        return ent_bitcrasher_get_rate(bitcrasherDsp);
}

void DspWrapperBitcrasher::setChaos(double value)
{
        ent_bitcrasher_set_chaos(bitcrasherDsp, value);
}

double DspWrapperBitcrasher::chaos() const
{
        return ent_bitcrasher_get_chaos(bitcrasherDsp);
}

void DspWrapperBitcrasher::setFold(double value)
{
        ent_bitcrasher_set_fold(bitcrasherDsp, value);
}

double DspWrapperBitcrasher::fold() const
{
        return ent_bitcrasher_get_fold(bitcrasherDsp);
}

void DspWrapperBitcrasher::setMix(double value)
{
        ent_bitcrasher_set_mix(bitcrasherDsp, value);
}

double DspWrapperBitcrasher::mix() const
{
        return ent_bitcrasher_get_mix(bitcrasherDsp);
}

double DspWrapperBitcrasher::bitsToNormalized(int value)
{
        return (static_cast<double>(value) - ENT_BITCRASHER_MIN_BITS)
                / (ENT_BITCRASHER_MAX_BITS - ENT_BITCRASHER_MIN_BITS);
}

int DspWrapperBitcrasher::bitsFromNormalized(double value)
{
        return static_cast<int>(std::lround(
                ENT_BITCRASHER_MIN_BITS
                + value * (ENT_BITCRASHER_MAX_BITS - ENT_BITCRASHER_MIN_BITS)));
}
