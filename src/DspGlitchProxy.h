/**
 * File name: DspGlitchProxy.h
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

#ifndef DSP_GLITCH_PROXY_H
#define DSP_GLITCH_PROXY_H

#include "RkObject.h"
#include "GuiTypes.h"

class DspGlitchProxy : public RkObject {
 public:
        explicit DspGlitchProxy(RkObject* parent = nullptr, GlitchId id = GlitchId::Glitch1);
        void setGlitchId(GlitchId id);
        GlitchId getGlitchId() const;
        virtual bool enable(bool b = true) = 0;
        virtual bool isEnabled() const = 0;
        virtual bool setRepeats(int value) = 0;
        virtual int repeats() const = 0;
        virtual bool setProbability(double value) = 0;
        virtual double probability() const = 0;
        virtual bool setLength(double value) = 0;
        virtual double length() const = 0;
        virtual bool setMaxJump(double value) = 0;
        virtual double maxJump() const = 0;
        virtual bool setMinJump(double value) = 0;
        virtual double minJump() const = 0;

        RK_DECL_ACT(enabled,
                    enabled(bool b),
                    RK_ARG_TYPE(bool b),
                    RK_ARG_VAL(b));
        RK_DECL_ACT(repeatsUpdated,
                    repeatsUpdated(int value),
                    RK_ARG_TYPE(int),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(probabilityUpdated,
                    probabilityUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(lengthUpdated,
                    lengthUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(maxJumpUpdated,
                    maxJumpUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));
        RK_DECL_ACT(minJumpUpdated,
                    minJumpUpdated(double value),
                    RK_ARG_TYPE(double),
                    RK_ARG_VAL(value));

private:
        GlitchId glitchId;
};

#endif // DSP_GLITCH_PROXY_H
