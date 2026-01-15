/**
 * File name: DspWrapperPitch.h
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

#ifndef ENT_DSP_WRAPPER_PITCH_H
#define ENT_DSP_WRAPPER_PITCH_H

#include "DspTypes.h"

struct ent_pitch;

class DspWrapperPitch
{
public:
    explicit DspWrapperPitch(struct ent_pitch* dsp);
    void enable(bool b = true);
    bool isEnabled() const;
    void setPitch(double pitch);
    double getPitch(double pitch) const;
    void setFine(double fine);
    double getFine() const;
    void setDepth(double depth);
    double getDepth() const;
    void setDrift(double depth);
    double getDrift() const;

private:
    struct ent_pitch* pitchDsp;
};

#endif // ENT_DSP_WRAPPER_PITCH_H
