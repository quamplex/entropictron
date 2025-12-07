/**
 * File name: DspCrackleProxy.h
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

#ifndef DSP_CRACKLE_PROXY_H
#define DSP_CRACKLE_PROXY_H

#include "RkObject.h"
#include "GuiTypes.h"

class DspCrackleProxy : public RkObject {
 public:
        explicit DspCrackleProxy(CrackleId id = CrackleId::Crackle1);
        explicit DspCrackleProxy(RkObject* parent, CrackleId id = CrackleId::Crackle1);
        void setCrackleId(CrackleId id);
        CrackleId getCrackleId() const;
        virtual bool enable(bool b = true) = 0;
        virtual bool isEnabled() const = 0;
        virtual bool setRate(double value) = 0;
        virtual double rate() const = 0;
        virtual bool setDuration(double value) = 0;
        virtual double duration() const = 0;
        virtual bool setAmplitude(double value) = 0;
        virtual double amplitude() const = 0;
        virtual bool setRandomness(double value) = 0;
        virtual double randomness() const = 0;
        virtual bool setBrightness(double value) = 0;
        virtual double brightness() const = 0;
        virtual bool setEnvelopeShape(CrackleEnvelopeShape shape) = 0;
        virtual CrackleEnvelopeShape envelopeShape() const = 0;
        virtual bool setStereospread(double value) = 0;
        virtual double stereospread() const = 0;

        RK_DECL_ACT(enabled,
                    enabled(bool b),
                    RK_ARG_TYPE(bool b),
                    RK_ARG_VAL(b));
        RK_DECL_ACT(rateUpdated,
                    rateUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(durationUpdated,
                    durationUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(amplitudeUpdated,
                    amplitudeUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(randomnessUpdated,
                    randomnessUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(brightnessUpdated,
                    brightnessUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(envelopeShapeUpdated,
                    envelopeShapeUpdated(CrackleEnvelopeShape value),
                    RK_ARG_TYPE(CrackleEnvelopeShape),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(stereospreadUpdated,
                    stereospreadUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));

private:
        CrackleId crackleId;
};

#endif // DSP_CRACKLE_PROXY_H
