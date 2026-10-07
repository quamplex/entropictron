/**
 * File name: VstIds.h
 * Project: Entropictron (A context generator and audio effect)
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

#ifndef ENT_VST_IDS_H
#define ENT_VST_IDS_H

#include "globals.h"

#include "pluginterfaces/base/funknown.h"

using namespace Steinberg;
static const FUID EntVstProcessorUID (0x4C4302E9, 0xDFE24902, 0xB3B49DE4, 0x9C179F91);
static const FUID EntVstControllerUID(0x8A3844E3, 0xBAF94A6F, 0x9D544306, 0x5CA27CFE);
#endif // ENT_VST_IDS_H
