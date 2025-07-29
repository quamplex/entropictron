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

using namespace Steinberg;

EntVstPluginView::EntVstPluginView(Vst::EditController *controller)
        : editController {controller}
{
}

tresult PLUGIN_API
EntVstPluginView::isPlatformTypeSupported(Steinberg::FIDString type)
{
        //        if (strcmp(type, kPlatformTypeHWND) == 0 || strcmp(type, kPlatformTypeX11) == 0)
        //return kResultTrue;
        return kResultTrue;
}

tresult PLUGIN_API
EntVstPluginView::attached(void* parent, FIDString type)
{
        return IPlugView::attached(parent, type);
}

/*tresult PLUGIN_API
EntVstPluginView::removed()
{
        return kResultOk;
        }*/

tresult PLUGIN_API
EntVstPluginView::getSize(ViewRect* newSize)
{
	return kResultOk;
}
