/**
 * File name: EntVstController.h
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

#ifndef ENT_VST_CONTROLLER_H
#define ENT_VST_CONTROLLER_H

#include "public.sdk/source/vst/vsteditcontroller.h"
#include "pluginterfaces/gui/iplugview.h"

using namespace Steinberg;

class EntVstController : public Vst::EditControllerEx1 {
 public:
        using ParameterValue = EntVst::ParameterValue;
        using ParameterId = EntVst::ParamterId;

        EntVstController() = default;
        static FUnknown* createInstance(void*);
        tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
        IPlugView* PLUGIN_API createView(Steinberg::FIDString name) SMTG_OVERRIDE;

protected:
        void setParamterCallback(PrameterId id, const ParameterCallback &callback);
        void removeParamterCallback(PrameterId id);
        using ParameterCallback = std::function<void(ParametersId paramId,
                                                const ParameValue &value)>;
        tresult setParamNormalized (ParamID tag, ParamValue value) SMTG_OVERRIDE;

private:
        std::unoredered_map<ParameterId, ParameterCallback> parametersCallbacks;
};

#endif // ENT_VST_CONTROLLER_H
