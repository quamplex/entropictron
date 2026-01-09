/**
 * File name: DspProxy.h
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

#ifndef DSP_PROXY_H
#define DSP_PROXY_H

#include "GuiTypes.h"

#include "RkObject.h"

class DspNoiseProxy;
class DspCrackleProxy;
class DspGlitchProxy;
class EntState;

class DspProxy : public RkObject {
 public:
        DspProxy(RkObject *parent = nullptr);
        virtual ~DspProxy() = default;
        virtual bool setPlayMode(PlayMode mode) = 0;
        virtual PlayMode playMode() const = 0;
        virtual bool setEntropyRate(double value) = 0;
        virtual double getEntropyRate() const = 0;
        virtual DspNoiseProxy* getNoise(NoiseId id) const = 0;
        virtual DspCrackleProxy* getCrackle(CrackleId id) const = 0;
        virtual DspGlitchProxy* getGlitch(GlitchId id) const = 0;

        RK_DECL_ACT(stateChanged,
                    stateChanged(),
                    RK_ARG_TYPE(),
                    RK_ARG_VAL());
        RK_DECL_ACT(playModeUpdated,
                    playModeUpdated(PlayMode mode),
                    RK_ARG_TYPE(PlayMode),
                    RK_ARG_VAL(mode));
        RK_DECL_ACT(entropyRateUpdated,
                    entropyRateUpdated(double val),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(val));
};

#endif // DSP_PROXY_H
