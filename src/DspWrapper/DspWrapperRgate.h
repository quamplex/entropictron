/**
 * File name: DspWrapperRgate.h
 * Project: Entropictron (A texture synthesizer)
 *
 * Copyright (C) 2026 Iurie Nistor
 *
 * This file is part of Entropictron.
 *
 * Entropictron is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef ENT_DSP_WRAPPER_RGATE_H
#define ENT_DSP_WRAPPER_RGATE_H

#include "DspTypes.h"

struct ent_rgate;

class DspWrapperRgate
{
public:
    explicit DspWrapperRgate(struct ent_rgate* dsp);
    void enable(bool b = true);
    bool isEnabled() const;
    void setMinInterval(double value);
    double getMinInterval() const;
    void setMaxInterval(double value);
    double getMaxInterval() const;
    void setMinDuration(double value);
    double getMinDuration() const;
    void setMaxDuration(double value);
    double getMaxDuration() const;
    void setMinGain(double value);
    double getMinGain() const;
    void setMaxGain(double value);
    double getMaxGain() const;
    void setRandomness(double value);
    double getRandomness() const;
    void setInverted(bool b = true);
    double isInverted() const;

private:
    struct ent_rgate* rgateDsp;
};

#endif // ENT_DSP_WRAPPER_RGATE_H
