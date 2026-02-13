/**
 * File name: DspRgateProxyVst.h
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

#ifndef DSP_RGATE_PROXY_VST_H
#define DSP_RGATE_PROXY_VST_H

#include "DspRgateProxy.h"
#include "EntVstParameters.h"

using namespace EntVst;
using namespace Steinberg::Vst;

class EntVstController;
class EntState;

class DspRgateProxyVst: public DspRgateProxy {
 public:
        explicit DspRgateProxyVst(RkObject* parent,
                                  EntVstController *controller);
        ~DspRgateProxyVst();
        bool enable(bool b = true) override;
        bool isEnabled() const override;
        bool setMinInterval(double value) override;
        double getMinInterval() const override;
        bool setMaxInterval(double value) override;
        double getMaxInterval() const override;
        bool setMinDuration(double value) override;
        double getMinDuration() const override;
        bool setMaxDuration(double value) override;
        double getMaxDuration() const override;
        bool setMinGain(double value) override;
        double getMinGain() const override;
        bool setMaxGain(double value) override;
        double getMaxGain() const override;
        bool setRandomness(double value) override;
        double getRandomness() const override;
        bool setInverted(bool b = true) override;
        bool getInverted() const override;

        static double enabledToNormalized(bool b);
        static bool enabledFromNormalized(double value);
        static double minIntervalToNormalized(int value);
        static double minIntervalFromNormalized(double value);
        static double maxIntervalToNormalized(int value);
        static double maxIntervalFromNormalized(double value);
        static double minDurationToNormalized(int value);
        static double minDurationFromNormalized(double value);
        static double maxDurationToNormalized(int value);
        static double maxDurationFromNormalized(double value);
        static double minGainToNormalized(int value);
        static double minGainFromNormalized(double value);
        static double maxGainToNormalized(int value);
        static double maxGainFromNormalized(double value);
        static double randomnessToNormalized(double value);
        static double randomnessFromNormalized(double value);
        static double invertedToNormalized(bool b);
        static bool invertedFromNormalized(double value);

protected:
        void onParameterChanged(ParameterId paramId, ParamValue value);

 private:
        EntVstController *vstController;
};

#endif // DSP_RGATE_PROXY_VST_H
