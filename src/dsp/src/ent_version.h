/**
 * File name: ent_version.h
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

#ifndef ENT_VERSION_H
#define ENT_VERSION_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ENT_VERSION ENT_VERSION_HEX
#define ENT_VERSION_MAJOR ((ENT_VERSION_HEX >> 16) & 0xFF)
#define ENT_VERSION_MINOR ((ENT_VERSION_HEX >> 8) & 0xFF)
#define ENT_VERSION_PATCH (ENT_VERSION_HEX & 0xFF)
#define ENT_NAME "Entropictron"
#define ENT_APP_NAME "entropictron"
#define ENT_VERSION_STRING ENT_VERSION_STR

#ifdef __cplusplus
}
#endif

#endif // ENT_VERSION_H
