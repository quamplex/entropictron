/**
 * File name: ent_lfo.h
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

#ifndef ENT_LFO_H
#define ENT_LFO_H

#include "ent_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

enum ent_lfo_type {
        ENT_LFO_TYPE_SINE,
        ENT_LFO_TYPE_SAW,
        ENT_LFO_TYPE_SQUARE,
        ENT_LFO_TYPE_TRIANGLE,
        ENT_LFO_TYPE_ENTROPIC,
};

        
#ifdef __cplusplus
}
#endif
#endif // ENT_LFO_H

