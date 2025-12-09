/**
 * File name: DspGlitchProxyVst.h
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

#ifndef DSP_GLITCH_PROXY_VST_H
#define DSP_GLITCH_PROXY_VST_H

#include "DspGlitchProxy.h"
#include "EntVstParameters.h"

using namespace EntVst;
using namespace Steinberg::Vst;

class EntVstController;

class DspGlitchProxyVst: public DspGlitchProxy {
 public:
        explicit DspGlitchProxyVst(RkObject* parent,
                                    EntVstController *controller,
                                    GlitchId id);
        ~DspGlitchProxyVst();
        bool enable(bool b = true) override;
        bool isEnabled() const override;
        bool setRepeats(int value) override;
        int repeats() const override;
        bool setProbability(double value) override;
        double probability() const override;
        bool setLength(double value) override;
        double length() const override;
        bool setMaxJump(double value) override;
        double maxJump() const override;
        bool setMinJump(double value) override;
        double minJump() const override;

        static double repeatsToNormalized(int value);
        static int repeatsFromNormalized(double normalized);
        static double probabilityToNormalized(double value);
        static double probabilityFromNormalized(double normalized);
        static double lengthToNormalized(double value);
        static double lengthFromNormalized(double normalized);
        static double minJumpToNormalized(double value);
        static double minJumpFromNormalized(double normalized);
        static double maxJumpToNormalized(double value);
        static double maxJumpFromNormalized(double normalized);

protected:
        void onParameterChanged(ParameterId paramId, ParamValue value);

 private:
        EntVstController *vstController;
};

#endif // DSP_GLITCH_PROXY_VST_H
