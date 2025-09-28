/**
 * File name: entropictron.h
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

#ifndef ENT_H
#define ENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <inttypes.h>
#include <float.h>
#include <stdbool.h>
#include <unistd.h>

#ifdef __FAST_MATH__
#error -ffast-math disables nan detection needed by entropictron
#endif

#ifdef __STDC_NO_ATOMICS__
#error atomic operations are not supported
#endif

#include "ent_log.h"

#define ENT_UNUSED(expr) (void)expr

#define ENT_VERSION ENT_VERSION_HEX
#define ENT_VERSION_MAJOR ((ENT_VERSION_HEX >> 16) & 0xFF)
#define ENT_VERSION_MINOR ((ENT_VERSION_HEX >> 8) & 0xFF)
#define ENT_VERSION_PATCH (ENT_VERSION_HEX & 0xFF)
#define ENT_NAME "Entropictron"
#define ENT_APP_NAME "entropictron"
#define ENT_VERSION_STRING ENT_VERSION_STR

#define ENT_DEFAULT_SAMPLE_RATE 48000

enum ent_error {
  	ENT_OK                    = 0,
	ENT_ERROR                 = 1,
	ENT_ERROR_MEM_ALLOC       = 2,
	ENT_ERROR_WRONG_ARGUMENTS = 3
};

enum ent_key_state {
        ENT_KEY_STATE_DEFAULT  = 0,
        ENT_KEY_STATE_PRESSED  = 1,
        ENT_KEY_STATE_RELEASED = 2
};

enum ent_channel_type {
        ENT_CHANNEL_AUDIO_INPUT  = 0,
        ENT_CHANNEL_AUDIO_OUTPUT = 1,
        ENT_CHANNEL_MIDI_INPUT   = 2,
        ENT_CHANNEL_MIDI_OUTPUT  = 3
};

#define ENT_ANY_KEY (-1)
#define ENT_ANY_MIDI_CHANNEL (-1)
#define ENT_MAX_MIDI_CHANNELS (16)

struct entropictron {
	unsigned int sample_rate;
};

enum ent_error ent_create(struct entropictron **ent, unsigned int sample_rate);

void ent_free(struct entropictron **ent);

enum ent_error ent_set_sample_rate(struct entropictron *ent, unsigned int rate);

enum ent_error ent_get_sample_rate(struct entropictron *ent, unsigned int *sample_rate);

enum ent_error ent_process(struct entropictron *ent, float** data, size_t size);

#ifdef __cplusplus
}
#endif
#endif // ENTROPICTRON_H
