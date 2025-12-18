/**
 * File name: DspProxyVst.h
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

#ifndef DSP_PROXY_VST_H
#define DSP_PROXY_VST_H

#include "DspProxy.h"
#include "EntVstParameters.h"
#include "EntVstController.h"

using namespace EntVst;
using namespace Steinberg::Vst;

class EntVstController;
class DspNoiseProxyVst;
class DspCrackleProxyVst;
class DspGlitchProxyVst;

class DspProxyVst: public DspProxy {
 public:
        explicit DspProxyVst(EntVstController *controller);
        bool setPlayMode(PlayMode mode) override;
        PlayMode playMode() const override;
        DspNoiseProxy* getNoise(NoiseId id) const override;
        DspCrackleProxy* getCrackle(CrackleId id) const override;
        DspGlitchProxy* getGlitch(GlitchId id) const override;

        static double playModeToNormalized(PlayMode mode);
        static PlayMode playModeFromNormalized(double value);

protected:
        void onParameterChanged(ParameterId paramId, ParamValue value);

 private:
        EntVstController *vstController;
        DspNoiseProxyVst *dspNoise1Proxy;
        DspNoiseProxyVst *dspNoise2Proxy;
        DspCrackleProxyVst *dspCrackle1Proxy;
        DspCrackleProxyVst *dspCrackle2Proxy;
        DspGlitchProxyVst *dspGlitch1Proxy;
        DspGlitchProxyVst *dspGlitch2Proxy;
};

#endif // DSP_PROXY_VST_H
