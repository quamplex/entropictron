/**
 * File name: EntropictronModel.cpp
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

#include "EntropictronModel.h"
#include "DspProxy.h"
#include "NoiseModel.h"
#include "CrackleModel.h"
#include "GlitchModel.h"

EntropictronModel::EntropictronModel(RkObject *parent, DspProxy *dspProxy)
        : RkObject(parent)
        , dspProxy{dspProxy}
        , noise1Model{new NoiseModel(this, dspProxy->getNoise(NoiseId::Noise1))}
        , noise2Model{new NoiseModel(this, dspProxy->getNoise(NoiseId::Noise2))}
        , crackle1Model{new CrackleModel(this, dspProxy->getCrackle(CrackleId::Crackle1))}
        , crackle2Model{new CrackleModel(this, dspProxy->getCrackle(CrackleId::Crackle2))}
        , glitch1Model{new GlitchModel(this, dspProxy->getGlitch(GlitchId::Glitch1))}
        , glitch2Model{new GlitchModel(this, dspProxy->getGlitch(GlitchId::Glitch2))}
{
        dspProxy->setParent(this);
}

NoiseModel* EntropictronModel::getNoise1() const
{
        return  noise1Model;
}

NoiseModel* EntropictronModel::getNoise2() const
{
        return  noise2Model;
}

CrackleModel* EntropictronModel::getCrackle1() const
{
        return  crackle1Model;
}

CrackleModel* EntropictronModel::getCrackle2() const
{
        return  crackle2Model;
}

GlitchModel* EntropictronModel::getGlitch1() const
{
        return  glitch1Model;
}

GlitchModel* EntropictronModel::getGlitch2() const
{
        return  glitch2Model;
}


