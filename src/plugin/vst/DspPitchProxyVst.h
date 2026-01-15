/**
 * File name: DspPitchProxyVst.h
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

#ifndef DSP_PITCH_PROXY_VST_H
#define DSP_PITCH_PROXY_VST_H

#include "DspPitchProxy.h"
#include "EntVstParameters.h"

using namespace EntVst;
using namespace Steinberg::Vst;

class EntVstController;
class EntState;

class DspPitchProxyVst: public DspPitchProxy {
 public:
        explicit DspPitchProxyVst(RkObject* parent,
                                  EntVstController *controller);
        ~DspPitchProxyVst();
        bool enable(bool b = true) override;
        bool isEnabled() const override;
        bool setPitch(int value) override;
        int pitch() const override;
        bool setFine(double value) override;
        double fine() const override;
        bool setDepth(double value) override;
        double depth() const override;
        bool setDrift(double value) override;
        double drift() const override;

        static double pitchToNormalized(int value);
        static int pitchFromNormalized(double normalized);
        static double fineToNormalized(double value);
        static double fineFromNormalized(double normalized);
        static double depthToNormalized(double value);
        static double depthFromNormalized(double normalized);
        static double driftToNormalized(double value);
        static double driftFromNormalized(double normalized);

protected:
        void onParameterChanged(ParameterId paramId, ParamValue value);

 private:
        EntVstController *vstController;
};

#endif // DSP_PITCH_PROXY_VST_H
