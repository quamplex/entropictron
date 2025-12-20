 /**
 * File name: EntVstProcessor.h
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

#ifndef ENT_VST_PROCESSOR_H
#define ENT_VST_PROCESSOR_H

#include "globals.h"
#include "EntVstParameters.h"
#include "ent_state.h"

#include "public.sdk/source/vst/vstaudioeffect.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef ENTROPICTRON_OS_WINDOWS
__attribute__((visibility("default"))) bool ModuleEntry (void*);
__attribute__((visibility("default"))) bool ModuleExit (void);
#endif // ENTROPICTRON_OS_WINDOWS
#ifdef __cplusplus
}
#endif // __cplusplus

using namespace Steinberg;
using namespace Steinberg::Vst;
using namespace EntVst;

class DspWrapper;

class EntVstProcessor : public AudioEffect {
 public:
        EntVstProcessor();
        ~EntVstProcessor();
        static FUnknown* createInstance(void*);
        tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
        tresult PLUGIN_API terminate() SMTG_OVERRIDE;
        tresult PLUGIN_API setBusArrangements(Vst::SpeakerArrangement* inputs,
                                              int32 numIns,
                                              SpeakerArrangement* outputs,
                                              int32 numOuts) SMTG_OVERRIDE;
        tresult PLUGIN_API setupProcessing(ProcessSetup& setup) SMTG_OVERRIDE;
        tresult PLUGIN_API setActive(TBool state) SMTG_OVERRIDE;
        tresult PLUGIN_API process(ProcessData& data) SMTG_OVERRIDE;

 protected:
        tresult PLUGIN_API setProcessing (TBool state) SMTG_OVERRIDE;
        using UpdateParamFunc = std::function<void(const ParamValue&)>;
        void initParamMappings();
        void initNoiseParamMappings();
        void initCrackleParamMappings();
        void initGlitchParamMappings();
        void updateParameters(ParameterId pid, ParamValue value);
        void storeDspSate();
        tresult setState (IBStream *state) SMTG_OVERRIDE;
        tresult getState (IBStream *state) SMTG_OVERRIDE;

 private:
        std::unordered_map<ParameterId, UpdateParamFunc> paramMap;
        std::unique_ptr<DspWrapper> entropictronDsp;
        bool dspStateUpdated;
        std::atomic<bool> isPendingState;
        struct ent_state dspState;
};

#endif // ENT_VST_PROCESSOR_H
