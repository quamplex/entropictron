/**
 * File name: VstIds.h
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

#include "EntVstProcessor.h"
#include "EntVstController.h"
#include "VstIds.h"

#include "public.sdk/source/main/pluginfactory.h"
#include "pluginterfaces/base/fplatform.h"

#define entPluginName		"Entropictron"
#define entOriginalFilename	"Entropictron.vst3"
#if SMTG_PLATFORM_64
#define entFileDescription	entPluginName" Entropictron (64Bit)"
#else
#define entFileDescription	entPluginName" Entropictron"
#endif
#define entCompanyName	"Quamplex"
#define entCompanyWeb		"http://quamplex.com/entropictron"
#define entCompanyEmail	"iuriehn@gmail.com"
#define entLegalCopyright	"Copyright (C) 2020 Iurie Nistor. Licensed under the GNU General Public License, Version 3"

BEGIN_FACTORY_DEF(entCompanyName, entCompanyWeb, entCompanyEmail)

// Processor registration
DEF_CLASS2(INLINE_UID_FROM_FUID(EntVstProcessorUID),
           PClassInfo::kManyInstances,
           kVstAudioEffectClass,
           entPluginName,
           0,
           Vst::PlugType::kFx,
           ENT_VERSION_STRING,
           kVstVersionString,
           EntVstProcessor::createInstance)

// Controller registration
DEF_CLASS2(INLINE_UID_FROM_FUID(EntVstControllerUID),
           PClassInfo::kManyInstances,
           kVstComponentControllerClass,
           entPluginName "Controller",
           0,
           "",
           ENT_VERSION_STRING,
           kVstVersionString,
           EntVstController::createInstance)

END_FACTORY

bool InitModule()
{
        return true;
}

bool DeinitModule()
{
        return true;
}
