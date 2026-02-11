/**
 * File name: DspRgateProxy.h
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

#ifndef DSP_RGATE_PROXY_H
#define DSP_RGATE_PROXY_H

#include "RkObject.h"
#include "GuiTypes.h"

class DspRgateProxy : public RkObject {
 public:
        explicit DspRgateProxy(RkObject* parent = nullptr);
        virtual bool enable(bool b = true) = 0;
        virtual bool isEnabled() const = 0;
        virtual bool setMinInterval(double value) = 0;
        virtual double getMinInterval() const = 0;
        virtual bool setMaxInterval(double value) = 0;
        virtual double getMaxInterval() const = 0;
        virtual bool setMinDuration(double value) = 0;
        virtual double getMinDuration() const = 0;
        virtual bool setMaxDuration(double value) = 0;
        virtual double getMaxDuration() const = 0;
        virtual bool setMinGain(double value) = 0;
        virtual double getMinGain() const = 0;
        virtual bool setMaxGain(double value) = 0;
        virtual double getMaxGain() const = 0;
        virtual bool setRandomness(double value) = 0;
        virtual double getRandomness() const = 0;
        virtual bool setInverted(bool b = true) = 0;
        virtual bool getInverted() const = 0;
        virtual bool setDrywet(double value) = 0;
        virtual double getDrywet() const = 0;

        RK_DECL_ACT(enabled,
                    enabled(bool b),
                    RK_ARG_TYPE(bool b),
                    RK_ARG_VAL(b));
        RK_DECL_ACT(minIntervalUpdated,
                    minIntervalUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(maxIntervalUpdated,
                    maxIntervalUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(minDurationUpdated,
                    minDurationUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(maxDurationUpdated,
                    maxDurationUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(minGainUpdated,
                    minGainUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(maxGainUpdated,
                    maxGainUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(randomnessUpdated,
                    randomnessUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(invertedUpdated,
                    invertedUpdated(bool b),
                    RK_ARG_TYPE(bool),
                    RK_ARG_VAL(b));
        RK_DECL_ACT(drywetUpdated,
                    drywetUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
};

#endif // DSP_RGATE_PROXY_H
