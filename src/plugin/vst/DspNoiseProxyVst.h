/**
 * File name: DspNoiseProxyVst.h
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

#ifndef DSP_NOISE_PROXY_VST_H
#define DSP_NOISE_PROXY_VST_H

#include "DspNoiseProxy.h"
#include "EntVstParameters.h"

using namespace EntVst;
using namespace Steinberg::Vst;

class EntVstController;

class DspNoiseProxyVst: public DspNoiseProxy {
 public:
        explicit DspNoiseProxyVst(RkObject* parent,
                                  EntVstController *controller,
                                  NoiseId id);
        ~DspNoiseProxyVst();
        bool enable(bool b = true) override;
        bool isEnabled() const override;
        bool setType(NoiseType type) override;
        NoiseType noiseType() const override;
        bool setDensity(double value) override;
        double density() const override;
        bool setBrightness(double value) override;
        double brightness() const override;
        bool setGain(double value) override;
        double gain() const override;
        bool setStereo(double value) override;
        double stereo() const override;
        bool setFilterType(FilterType type) override;
        FilerType filterType() const override;
        bool setCutOff(double value) override;
        double cutOff() const override;
        bool setResonance(double value) override;
        double resonance() const override;

protected:
        void onParameterChanged(ParameterId paramId, ParamValue value);
        NoiseType noiseTypeFromNormalized(double value) const;
        double noiseTypeToNormalized(NoiseType type) const;
        double filterTypeToNormalized(FilterType type) const;
        NoiseType filterTypeFromNormalized(double value) const;
private:
        EntVstController *vstController;
};

#endif // DSP_NOISE_PROXY_VST_H
