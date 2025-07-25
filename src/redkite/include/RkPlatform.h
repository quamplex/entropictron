/**
 * File name: RkPlatform.h
 * Project: Redkite (A small GUI toolkit)
 *
 * Copyright (C) 2019 Iurie Nistor 
 *
 * This file is part of Redkite.
 *
 * Redkite is free software; you can redistribute it and/or modify
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

#ifndef RK_PLATFORM_H
#define RK_PLATFORM_H

#include "Rk.h"
#include "RkLog.h"
#include "RkEvent.h"

#ifdef RK_OS_WIN
#include <windows.h>

struct RkWindowId {
        RkWindowId(HWND arg = nullptr) : id{arg} {}
        HWND id;
};

class RkNativeWindowInfo {
 public:
 RkNativeWindowInfo(HWND arg = nullptr)
         : window(arg)
        , instance{}
        , scaleFactor{1.0}
        {}
        ~RkNativeWindowInfo() = default;
        HWND window;
        HINSTANCE instance;
	double scaleFactor;
        std::string className;
};

HINSTANCE rk_win_api_instance();
LPCSTR rk_win_api_class_name();
RkNativeWindowInfo rk_from_native_win(HWND window, HINSTANCE instance, LPCSTR className);
RkWindowId rk_id_from_win(HWND window);
Rk::Key rk_convertKey(unsigned int winKey);
void rk_updateKeyModifiers(Rk::Key key, RkEvent::Type type);
int rk_getKeyModifiers();
class RkEventQueue;
class RkEvent;
void rk_processSystemEvent(RkEventQueue *eventQueue, std::unique_ptr<RkEvent> event);

#define RK_WIN_MESSAGE_PAINT (WM_USER + 0x0001)

#elif RK_OS_MAC
#else // X11 as default
#include <X11/Xlib.h>

struct RkWindowId {
        Window id;
};

struct _DndClass;

class RkNativeWindowInfo
{
 public:

 RkNativeWindowInfo() :
        display{nullptr}
        , screenNumber{0}
        , window{0}
        , scaleFactor{1.0}{}
        ~RkNativeWindowInfo() = default;
        Display* display;
        int screenNumber;
        Window window;
        double scaleFactor;
};

RkNativeWindowInfo rk_from_native_x11(Display* display,
                                      int screenNumber,
                                      Window window);
RkWindowId rk_id_from_x11(Window window);

#endif // X11
#endif // RK_PLATFORM_H
