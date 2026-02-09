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
        void setMinInterval(double value) override;
        double getMinInterval() const override;
        void setMaxInterval(double value) override;
        double getMaxInterval() const override;
        void setMinDuration(double value) override;
        double getMinDuration() const override;
        void setMaxDuration(double value) override;
        double getMaxDuration() const override;
        void setMinGain(double value) override;
        double getMinGain() const override;
        void setMaxGain(double value) override;
        double getMaxGain() override const;
        double setProbability(double value) override;
        double getProbability() const override;
        double setInverted(bool b = true) override;
        double isInverted() const override;

protected:
        void onParameterChanged(ParameterId paramId, ParamValue value);

 private:
        EntVstController *vstController;
};

#endif // DSP_RGATE_PROXY_VST_H
