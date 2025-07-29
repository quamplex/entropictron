/**
 * File name: mixer.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor 
 *
 * This file is part of Geonkick.
 *
 * GeonKick is free software; you can redistribute it and/or modify
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

#ifndef GKICK_MIXER_H
#define GKICK_MIXER_H

#include "audio_output.h"

struct ent_mixer {
	struct ent_audio_output **audio_outputs;
	size_t connection_matrix[127];
        _Atomic int solo;
	_Atomic int limiter;
        _Atomic short forced_midi_channel;
        void (*limiter_callback) (void*, size_t index, ent_real val);
        void *limiter_callback_arg;
        _Atomic size_t limiter_callback_index;
};

enum entropictron_error
ent_mixer_create(struct ent_mixer **mixer);

enum entropictron_error
ent_mixer_key_pressed(struct ent_mixer *mixer,
			struct ent_note_info *note);

enum entropictron_error
ent_mixer_tune_output(struct ent_mixer *mixer,
                        size_t index,
                        bool tune);

enum entropictron_error
ent_mixer_is_output_tuned(struct ent_mixer *mixer,
                            size_t index,
                            bool *tune);

enum entropictron_error
ent_mixer_process(struct ent_mixer *mixer,
                    float** out,
                    size_t offset,
                    size_t size);

void
ent_mixer_set_leveler(struct ent_mixer *mixer,
                        size_t index,
                        ent_real val);

void ent_mixer_apply_limiter(float *out_left,
                               float *out_right,
                               size_t size,
                               float limiter);

void
ent_mixer_free(struct ent_mixer **mixer);

enum entropictron_error
ent_mixer_limiter_set(struct ent_mixer *mixer,
                        size_t index,
                        ent_real val);

enum entropictron_error
ent_mixer_limiter_get(struct ent_mixer *mixer,
                        size_t index,
                        ent_real *val);

enum entropictron_error
ent_mixer_mute(struct ent_mixer *mixer, size_t id, bool b);

enum entropictron_error
ent_mixer_is_muted(struct ent_mixer *mixer, size_t id, bool *b);

enum entropictron_error
ent_mixer_solo(struct ent_mixer *mixer, size_t id, bool b);

enum entropictron_error
ent_mixer_is_solo(struct ent_mixer *mixer, size_t id, bool *b);

enum entropictron_error
ent_mixer_set_limiter_callback(struct ent_mixer *mixer,
				 void (*callback)(void*, size_t index, ent_real val),
				 void *arg);

enum entropictron_error
ent_mixer_mute(struct ent_mixer *mixer, size_t id, bool b);

enum entropictron_error
ent_mixer_force_midi_channel(struct ent_mixer *mixer,
                               signed char channel,
                               bool force);

enum entropictron_error
ent_mixer_get_forced_midi_channel(struct ent_mixer *mixer,
                                    signed char *channel,
                                    bool *force);

enum entropictron_error
ent_mixer_enable_note_off(struct ent_mixer *mixer,
                            size_t id,
                            bool enable);

enum entropictron_error
ent_mixer_note_off_enabled(struct ent_mixer *mixer,
                             size_t id,
                             bool *enabled);

#endif // GKICK_MIXER_H
