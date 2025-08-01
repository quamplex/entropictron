/**
 * File name: EntVstLoopTimer.cpp
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

#include "EntVstLoopTimer.h"

#include "RkMain.h"

using namespace Steinberg;

EntVstLoopTimer::EntVstLoopTimer(IPlugFrame *frame)
        : pluginFrame{frame}
        , guiApp{nullptr}
        , countT{1}
{
}

EntVstLoopTimer::~EntVstLoopTimer()
{
        unregisterTimer();
}

void EntVstLoopTimer::registerTimer(RkMain* app)
{
        guiApp = app;
        IRunLoop* loop = nullptr;
        if (pluginFrame->queryInterface(IRunLoop::iid,
                                        reinterpret_cast<void**>(&loop))
                                        == kResultOk) {
                loop->registerTimer(this, 1);
        }
}

void EntVstLoopTimer::unregisterTimer()
{
        guiApp = nullptr;
        IRunLoop* loop = nullptr;
        if (pluginFrame->queryInterface(IRunLoop::iid,
                                        reinterpret_cast<void**>(&loop))
                                        == kResultOk) {
                loop->unregisterTimer(this);
        }
}

tresult PLUGIN_API EntVstLoopTimer::queryInterface (const TUID _iid, void **obj)
{
        *obj = nullptr;
        return kNoInterface;
}

uint32 PLUGIN_API EntVstLoopTimer::addRef()
{
        return ++countT;
}

uint32 PLUGIN_API EntVstLoopTimer::release()
{
        auto newCount = --countT;
        if (newCount == 0)
                delete this;
        return newCount;
}

void PLUGIN_API EntVstLoopTimer::onTimer()
{
        if (guiApp)
                guiApp->exec(false);
}
