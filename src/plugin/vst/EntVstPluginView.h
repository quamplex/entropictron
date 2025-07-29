/**
 * File name: EntVstPluginView.h
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

#ifndef ENT_VST_PLUGIN_VIEW_H
#define ENT_VST_PLUGIN_VIEW_H

#include "public.sdk/source/vst/vsteditcontroller.h"
#include "pluginterfaces/gui/iplugview.h"

#include <RkEvent.h>

#include <memory>

using namespace Steinberg;

#ifdef ENTROPICTRON_OS_GNU
using namespace Linux;
#endif // ENTROPICTRON_OS_GNU

class EntVstPluginView : public IPlugView {
 public:
        EntVstPluginView(Vst::EditController *controller);
        tresult PLUGIN_API isPlatformTypeSupported(Steinberg::FIDString type) override;
        tresult PLUGIN_API attached(void* parent, FIDString type) override;
        //        tresult PLUGIN_API removed() override;
        tresult PLUGIN_API onWheel(float distance) override { return kNotImplemented; }
        tresult PLUGIN_API setFrame(Steinberg::IPlugFrame* frame) override { return kResultOk; }
        //        tresult PLUGIN_API resizeView(Steinberg::ViewRect* newSize) override { return kResultOk; }
        tresult PLUGIN_API canResize() override { return kResultFalse; }
        tresult PLUGIN_API removed() override;
        tresult PLUGIN_API getSize(ViewRect* newSize) override;

 private:
        Vst::EditController* editController;
};

#endif // ENT_VST_PLUGIN_VIEW_H
