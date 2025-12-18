/**
 * File name: DspWrapper.h
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

#ifndef ENT_DSP_WRAPER_H
#define ENT_DSP_WRAPER_H

#include "globals.h"
#include "DspTypes.h"

struct entropictron;
struct ent_state;
class EntState;
class DspWrapperNoise;
class DspWrapperCrackle;
class DspWrapperGlitch;

class DspWrapper
{
public:
        explicit DspWrapper();
        void setSampleRate(unsigned int srate);
        int getSampleRate() const;
        void setState(std::unique_ptr<EntState> state);
        void getState(struct ent_state *state) const;
        void process(float** data, size_t size);
        void pressKey(bool on = true,
                      int pitch = Entropictron::defaultMidiKey,
                      int velocity = Entropictron::maxKeyVelocity);
        void setPlayMode(PlayMode mode);
        PlayMode playMode() const;
        DspWrapperNoise* getNoise(NoiseId id) const;
        DspWrapperCrackle* getCrackle(CrackleId id) const;
        DspWrapperGlitch* getGlitch(GlitchId id) const;

protected:

private:
        struct DspDeleter {
                void operator()(struct entropictron* dsp) const
                {
                        ent_free(&dsp);
                }
        };
        std::unique_ptr<struct entropictron, DspDeleter> entropictronDsp;
        std::unique_ptr<DspWrapperNoise> dspNoise1;
        std::unique_ptr<DspWrapperNoise> dspNoise2;
        std::unique_ptr<DspWrapperCrackle> dspCrackle1;
        std::unique_ptr<DspWrapperCrackle> dspCrackle2;
        std::unique_ptr<DspWrapperGlitch> dspGlitch1;
        std::unique_ptr<DspWrapperGlitch> dspGlitch2;
};

#endif // ENT_DSP_WRAPER_H
