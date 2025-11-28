/**
 * File name: EntropictronModel.h
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

#ifndef ENTROPICTRON_MODEL_H
#define ENTROPICTRON_MODEL_H

#include "GuiTypes.h"
#include "RkObject.h"

class DspProxy;
class NoiseModel;
class CrackleModel;
class GlitchModel;
class EntState;

class EntropictronModel: public RkObject
{
 public:
        explicit EntropictronModel(RkObject *parent, DspProxy *dspProxy);
        bool loadPreset(const EntState *preset);
        void setPlayMode(PlayMode mode);
        PlayMode playMode() const;
        NoiseModel* getNoise1() const;
        NoiseModel* getNoise2() const;
        CrackleModel* getCrackle1() const;
        CrackleModel* getCrackle2() const;
        GlitchModel* getGlitch1() const;
        GlitchModel* getGlitch2() const;

        RK_DECL_ACT(playModeUpdated,
                    playModeUpdated(PlayMode mode),
                    RK_ARG_TYPE(PlayMode),
                    RK_ARG_VAL(mode));

 private:
        DspProxy *dspProxy;
        NoiseModel *noise1Model;
        NoiseModel *noise2Model;
        CrackleModel *crackle1Model;
        CrackleModel *crackle2Model;
        GlitchModel *glitch1Model;
        GlitchModel *glitch2Model;
};

#endif // ENTROPICTRON_MODEL_H
