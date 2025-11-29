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
#include "EntState.h"

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

        RK_ACT_BIND(dspProxy,
                    playModeUpdated,
                    RK_ACT_ARGS(PlayMode mode),
                    this, playModeUpdated(mode));
}

bool EntropictronModel::loadPreset(const EntState *preset)
{
        ENT_LOG_INFO("load preset: " << preset->getName());
        std::vector<NoiseModel*> noise = {noise1Model, noise2Model};
        for (size_t i = 0; i < noise.size(); i++) {
                noise[i]->enable(preset->noise[i].enabled);
                noise[i]->setType(static_cast<NoiseType>(preset->noise[i].type));
                noise[i]->setDensity(preset->noise[i].density);
                noise[i]->setBrightness(preset->noise[i].brightness);
                noise[i]->setGain(preset->noise[i].gain);
                noise[i]->setStereo(preset->noise[i].stereo);
                noise[i]->setFilterType(static_cast<FilterType>(preset->noise[i].filter_type));
                noise[i]->setCutOff(preset->noise[i].cutoff);
                noise[i]->setResonance(preset->noise[i].resonance);
        }

        std::vector<CrackleModel*> crackle = {crackle1Model, crackle2Model};
        for (size_t i = 0; i < crackle.size(); i++) {
                crackle[i]->enable(preset->crackle[i].enabled);
                crackle[i]->setRate(preset->crackle[i].rate);
                crackle[i]->setRandomness(preset->crackle[i].randomness);
                crackle[i]->setAmplitude(preset->crackle[i].amplitude);
                auto type = static_cast<CrackleEnvelopeShape>(preset->crackle[i].env_type);
                crackle[i]->setEnvelopeShape(type);
                crackle[i]->setBrightness(preset->crackle[i].brightness);
                crackle[i]->setDuration(preset->crackle[i].duration);
                crackle[i]->setStereospread(preset->crackle[i].stereo);
        }

        /*std::vector<GlitchModel*> glitch = {glitch1Model, glitch2Model};
        for (size_t i = 0; i < glitch.size(); i++) {
                glitch[i]->enable(preset->glitch[i].enabled);
                glitch[i]->setRepeats(preset->glitch[i].repeats);
                glitch[i]->setProbability(preset->glitch[i].probability);
                glitch[i]->setLength(preset->glitch[i].length);
                glitch[i]->setMinJump(preset->glitch[i].min_jump);
                glitch[i]->setMaxJump(preset->glitch[i].max_jump);
                }*/

        return true;
}

void EntropictronModel::setPlayMode(PlayMode mode)
{
        if (dspProxy->setPlayMode(mode)) {
                action playModeUpdated(mode);
        }
}

PlayMode EntropictronModel::playMode() const
{
        return dspProxy->playMode();
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


