/**
 * File name: DspCrackleProxyVst.h
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

#ifndef DSP_CRACKLE_PROXY_VST_H
#define DSP_CRACKLE_PROXY_VST_H

#include "DspCrackleProxy.h"
#include "EntVstParameters.h"

using namespace EntVst;
using namespace Steinberg::Vst;

class EntVstController;

class DspCrackleProxyVst: public DspCrackleProxy {
 public:
        explicit DspCrackleProxyVst(RkObject* parent,
                                    EntVstController *controller,
                                    CrackleId id);
        ~DspCrackleProxyVst();
        bool enable(bool b = true) override;
        bool isEnabled() const override;
        bool setRate(double value) override;
        double rate() const override;
        bool setDuration(double value) override;
        double duration() const override;
        bool setAmplitude(double value) override;
        double amplitude() const override;
        bool setRandomness(double value) override;
        double randomness() const override;
        bool setBrightness(double value) override;
        double brightness() const override;
        bool setEnvelopeShape(CrackleEnvelopeShape shape) override;
        CrackleEnvelopeShape envelopeShape() const override;
        bool setStereospread(double value) override;
        double stereospread() const override;

        static double rateToNormalized(double value);
        static double rateFromNormalized(double value);
        static double durationToNormalized(double value);
        static double durationFromNormalized(double value);
        static double envelopeShapeToNormalized(CrackleEnvelopeShape shape);
        static CrackleEnvelopeShape envelopeShapeFromNormalized(double value);

protected:
        void onParameterChanged(ParameterId paramId, ParamValue value);

 private:
        EntVstController *vstController;
};

#endif // DSP_CRACKLE_PROXY_VST_H
