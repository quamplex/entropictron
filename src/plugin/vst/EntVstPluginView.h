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

#include "RkEvent.h"

#include <memory>
#include <atomic>

class MainWindow;
class RkMain;
class EntVstLoopTimer;

using namespace Steinberg;
using namespace Steinberg::Vst;

#ifdef ENTROPICTRON_OS_GNU
using namespace Linux;
#endif // ENTROPICTRON_OS_GNU

class EntVstPluginView : public EditorView
{
public:
        explicit EntVstPluginView(EditController* controller);
        ~EntVstPluginView() = default;
        tresult PLUGIN_API isPlatformTypeSupported(FIDString type) override;
        tresult PLUGIN_API setFrame(IPlugFrame* frame) override;
        tresult PLUGIN_API attached(void* parent, FIDString type) override;
        tresult PLUGIN_API removed() override;
        tresult PLUGIN_API getSize(ViewRect* newSize) override;
        tresult PLUGIN_API queryInterface(const TUID iid, void** obj) override;
        uint32 PLUGIN_API addRef() override;
        uint32 PLUGIN_API release() override;

private:
#ifdef ENTROPICTRON_OS_GNU
        std::unique_ptr<EntVstLoopTimer> loopTimer;
#endif
        std::unique_ptr<RkMain> guiApp;
        MainWindow* mainWindow;
        std::atomic<uint32_t> refCount;
};

#endif // ENT_VST_PLUGIN_VIEW_H
