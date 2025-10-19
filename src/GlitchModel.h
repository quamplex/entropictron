/**
 * File name: GlitchModel.h
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

#ifndef ENT_GLITCH_MODEL_H
#define ENT_GLITCH_MODEL_H

#include "EntAbstractModel.h"
#include "GuiTypes.h"

class DspGlitchProxy;

class GlitchModel: public EntAbstractModel
{
 public:
        explicit GlitchModel(RkObject *parent, DspGlitchProxy *dspGlitchProxy);
        GlitchId getId() const;
        void enable(bool b = true);
        bool isEnabled() const;
        bool setRepeats(double value);
        double repeats(double value) const;
        bool setProbability(double value);
        double probability(double value) const;
        bool setLength(double value);
        double lenth(double value) const;
        bool setMaxJump(double value);
        double maxJump(double value) const;
        bool setMinJump(double value);
        double minJump(double value) const;
        RK_DECL_ACT(enabled,
                    enabled(bool b),
                    RK_ARG_TYPE(bool b),
                    RK_ARG_VAL(b));
        RK_DECL_ACT(repeatsUpdated,
                    repeatsUpdated(double value),
                    RK_ARG_TYPE(double),
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
        DspGlitchProxy *dspGlitchProxy;
};

#endif // ENT_GLITCH_MODEL_H
