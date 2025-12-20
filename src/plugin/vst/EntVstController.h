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

#include "EntVstParameters.h"
#include "GuiTypes.h"

#include "public.sdk/source/vst/vsteditcontroller.h"
#include "pluginterfaces/gui/iplugview.h"

#include <unordered_map>
#include <functional>

using namespace Steinberg;
using namespace Steinberg::Vst;
using namespace EntVst;

class EntState;

class EntVstController : public Vst::EditControllerEx1 {
 public:
        using ParameterCallback = std::function<void(ParameterId paramId, ParamValue value)>;
        using StateCallback = std::function<void()>;
        EntVstController() = default;
        static FUnknown* createInstance(void*);
        tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
        IPlugView* PLUGIN_API createView(FIDString name) SMTG_OVERRIDE;
        void setParamterCallback(ParameterId id, const ParameterCallback &callback);
        void removeParamterCallback(ParameterId id);
        tresult setParamNormalized (ParamID tag, ParamValue value) SMTG_OVERRIDE;
        void setStateCallback(StateCallback callback);
        void clearStateCallbacks();
        void restartComponent();

protected:
        tresult PLUGIN_API setComponentState(IBStream* state) SMTG_OVERRIDE;
        void setNoiseState(const EntentState& state)
        void setCrackleState(const EntentState& state)
        void setGlitchState(const EntentState& state)

        void addNoiseParameters();
        void addCrackleParameters();
        void addGlitchParameters();

private:
        std::unordered_map<ParameterId, ParameterCallback> parametersCallbacks;
        StateCallback stateCallback;
};

#endif // ENT_VST_CONTROLLER_H
