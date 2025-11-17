/**
 * File name: ent_defs.h
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

#ifndef ENT_DEFS_H
#define ENT_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ent_version.h"

#define ENT_UNUSED(expr) (void)expr

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

enum ent_play_mode {
        ENT_PLAY_MODE_PLAYBACK = 0,
        ENT_PLAY_MODE_HOLD     = 1,
        ENT_PLAY_MODE_ON       = 2
};

#define ENT_ANY_KEY (-1)
#define ENT_ANY_MIDI_CHANNEL (-1)
#define ENT_MAX_MIDI_CHANNELS (16)

#ifdef __cplusplus
}
#endif
#endif // ENT_DEFS_H
