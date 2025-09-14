/**
 * File name: DspNoiseProxy.h
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

#ifndef DSP_NOISE_PROXY_H
#define DSP_NOISE_PROXY_H

#include "DspProxy.h"

#include "RkObject.h"

class DspNoiseProxy : public RkObject {
 public:
        explicit DspNoiseProxy(NoiseId id = {});
        explicit DspNoiseProxy(RkObject* parent, NoiseId id = {});
        virtual ~DspNoiseProxy() = default;
        void setNoiseId(NoiseId id);
        NoiseId getNoiseId() const;
        virtual void enable(bool b = true) = 0;
        virtual bool isEnabled() const = 0;
        virtual void setType(NoiseType type) = 0;
        virtual NoiseType noiseType() const = 0;
        virtual void setDensity(double value) = 0;
        virtual double density() const = 0;
        virtual void setBrightness(double value) = 0;
        virtual double brightness() const = 0;
        virtual void setGain(double value) = 0;
        virtual double gain() const = 0;
        RK_DECL_ACT(enabled,
                    enabled(bool b),
                    RK_ARG_TYPE(bool b),
                    RK_ARG_VAL(b));
        RK_DECL_ACT(typeUpdated,
                    typeUpdated(NoiseType type),
                    RK_ARG_TYPE(NoiseType),
                    RK_ARG_VAL(type));
        RK_DECL_ACT(densityUpdated,
                    densityUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(bightnessUpdated,
                    bightnessUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(gainUpdated,
                    gainUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));

private:
        NoiseId noiseId;
};

#endif // DSP_NOISE_PROXY_H
