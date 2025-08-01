/**
 * File name: EntVstLoopTimer.h
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

#ifndef ENT_VST_LOOP_TIMER_H
#define ENT_VST_LOOP_TIMER_H

#include "public.sdk/source/common/pluginview.h"

#include <atomic>

class RkMain;

using namespace Steinberg;
using namespace Linux;

class EntVstLoopTimer: public ITimerHandler {
 public:
        explicit EntVstLoopTimer(IPlugFrame *frame);
        virtual ~EntVstLoopTimer();
        void registerTimer(RkMain *app);
        void unregisterTimer();
        tresult PLUGIN_API queryInterface (const TUID _iid, void **obj);
        uint32 PLUGIN_API addRef() override;
        uint32 PLUGIN_API release() override;

 protected:
        void PLUGIN_API onTimer() override;

 private:
        Steinberg::IPlugFrame* pluginFrame;
        RkMain *guiApp;
        std::atomic<uint32> countT;
};

#endif // ENT_VST_LOOP_TIMER_H
