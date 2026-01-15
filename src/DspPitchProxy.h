/**
 * File name: DspPitchProxy.h
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

#ifndef DSP_PITCH_PROXY_H
#define DSP_PITCH_PROXY_H

#include "RkObject.h"
#include "GuiTypes.h"

class DspPitchProxy : public RkObject {
 public:
        DspPitchProxy();
        explicit DspPitchProxy(RkObject* parent);
        virtual bool enable(bool b = true) = 0;
        virtual bool isEnabled() const = 0;
        virtual bool setPitch(int value) = 0;
        virtual int pitch() const = 0;
        virtual bool setFine(double value) = 0;
        virtual double fine() const = 0;
        virtual bool setDepth(double value) = 0;
        virtual double depth() const = 0;
        virtual bool setDrift(double value) = 0;
        virtual double drift() const = 0;

        RK_DECL_ACT(enabled,
                    enabled(bool b),
                    RK_ARG_TYPE(bool b),
                    RK_ARG_VAL(b));
        RK_DECL_ACT(pitchUpdated,
                    pitchUpdated(int value),
                    RK_ARG_TYPE(int),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(fineUpdated,
                    fineUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(depthUpdated,
                    depthUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(driftUpdated,
                    driftUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
};

#endif // DSP_PITCH_PROXY_H
