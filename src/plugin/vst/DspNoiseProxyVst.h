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

using namespace EntVst;

class EntVstController;

class DspNoiseProxyVst: public DspNoiseProxy {
 public:
        explicit DspNoiseProxyVst(EntVstController *controller, NoiseId id);
        void enable(bool b = true) override;
        bool isEnabled() const override;
        void setType(NoiseType type) override;
        NoiseType noiseType() const override;
        void setDensity(double value) override;
        double density() const override;
        void setBrightness(double value) override;
        double brightness() const override;
        void setGain(double value) override;
        double gain() const override;

protected:
        void onParameterChanged(ParamterId paramId,
                                const ParamValue &value);
private:
        EntVstController *vstController;
};

#endif // DSP_NOISE_PROXY_VST_H
