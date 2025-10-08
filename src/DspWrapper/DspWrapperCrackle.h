/**
 * File name: DspWrapperCrackle.h
 * Project: Entropictron (A texture synthesizer)
 *
 * Copyright (C) 2025 Iurie Nistor
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

#ifndef ENT_DSP_WRAPPER_CRACKLE_H
#define ENT_DSP_WRAPPER_CRACKLE_H

#include "DspTypes.h"

struct ent_crackle;

class DspWrapperCrackle
{
public:
    explicit DspWrapperCrackle(struct ent_crackle* dsp);
    void enable(bool b = true);
    bool isEnabled() const;
    void setRate(double value);
    double rate() const;
    void setDuration(double value);
    double duration() const;
    void setAmplitude(double value);
    double amplitude() const;
    void setRandomness(double value);
    double randomness() const;
    void setBrightness(double value);
    double brightness() const;
    void setEnvelopeShape(CrackleEnvelopeShape shape);
    CrackleEnvelopeShape envelopeShape() const;
    void setStereoSpread(double value);
    double stereoSpread() const;

private:
    struct ent_crackle* crackleDsp;
};

#endif // ENT_DSP_WRAPPER_CRACKLE_H
