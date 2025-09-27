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

#include "EntVstPluginView.h"
#include "MainWindow.h"
#include "EntVstLoopTimer.h"
#include "DspVstProxy.h"
#include "EntVstController.h"

#include "RkPlatform.h"
#include "RkMain.h"

using namespace Steinberg;
using namespace Steinberg::Vst;

EntVstPluginView::EntVstPluginView(EditController *controller)
        : EditorView(controller)
{
}

tresult PLUGIN_API
EntVstPluginView::isPlatformTypeSupported(FIDString type)
{
        return kResultTrue;
}

tresult PLUGIN_API
EntVstPluginView::setFrame(IPlugFrame* frame)
{
#ifdef ENTROPICTRON_OS_GNU
        loopTimer = std::make_unique<EntVstLoopTimer>(frame);
#endif // ENTROPICTRON_OS_GNU
        return kResultOk;
}

tresult PLUGIN_API
EntVstPluginView::attached(void* parent, FIDString type)
{
        ENTROPICTRON_LOG_DEBUG("called");
        guiApp = std::make_unique<RkMain>();

#ifdef ENTROPICTRON_OS_WINDOWS
        auto info = rk_from_native_win(reinterpret_cast<HWND>(parent),
                                       rk_win_api_instance(),
                                       rk_win_api_class_name());
#else // ENTROPICTRON_OS_GNU
        auto xDisplay = XOpenDisplay(nullptr);
        if (!xDisplay)
                return kResultFalse;
        auto screenNumber = DefaultScreen(xDisplay);
        auto info = rk_from_native_x11(xDisplay, screenNumber, reinterpret_cast<Window>(parent));
#endif // ENTROPICTRON_OS_GNU

        auto controller = dynamic_cast<EntVstController*>(getController());
        if (!controller)
                return kResultFalse;

        auto dspProxy = new DspProxyVst(controller);
        dspProxy->setName("DspProxyVst");
        mainWindow = new MainWindow(*guiApp.get(), info, dspProxy);
        mainWindow->show();
        loopTimer->registerTimer(guiApp.get());

        return EditorView::attached(parent, type);
}

tresult PLUGIN_API
EntVstPluginView::removed()
{
        loopTimer->unregisterTimer();
        if (guiApp)
                guiApp = nullptr;
        dspProxy.reset();
        return kResultOk;
}

tresult PLUGIN_API
EntVstPluginView::getSize(ViewRect* newSize)
{
        if (!newSize)
		return kResultFalse;

        auto winRect = MainWindow::getWindowSize();
        newSize->left   = 0;
	newSize->right  = winRect.width();
	newSize->top    = 0;
	newSize->bottom = winRect.height();
	return kResultOk;
}

tresult PLUGIN_API EntVstPluginView::queryInterface(const TUID iid, void** obj)
{
        if (FUnknownPrivate::iidEqual(iid, IPlugView::iid)) {
                *obj = static_cast<IPlugView*>(this);
                addRef();
                return kResultOk;
        }

        *obj = nullptr;
        return kNoInterface;
}

uint32 PLUGIN_API EntVstPluginView::addRef()
{
        return ++refCount;
}

uint32 PLUGIN_API EntVstPluginView::release()
{
        auto count = --refCount;
        if (count == 0)
                delete this;
        return count;
}
