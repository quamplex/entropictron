/**
 * File name: ent_audio.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor
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

#ifndef GKICK_AUDIO_H
#define GKICK_AUDIO_H

#include "entropictron_internal.h"

struct audio_output;
struct ent_jack;
struct ent_mixer;

struct ent_audio {
        int sample_rate;
        struct ent_audio_output *audio_outputs[ENTROPICTRON_MAX_CHANNELS];
	struct ent_mixer *mixer;
        struct ent_jack *jack;
};

enum entropictron_error
ent_audio_create(struct ent_audio** audio, int sample_rate);

enum entropictron_error
ent_start_audio(struct ent_audio *audio);

void ent_audio_free(struct ent_audio** audio);

enum entropictron_error
ent_audio_set_limiter_val(struct ent_audio *audio,
                            size_t index,
                            ent_real limit);

enum entropictron_error
ent_audio_get_limiter_val(struct ent_audio *audio,
                            size_t index,
                            ent_real *limit);

enum entropictron_error
ent_audio_play(struct ent_audio *audio,
                 size_t id);

enum entropictron_error
ent_audio_key_pressed(struct ent_audio *audio,
                        bool pressed,
                        int note,
                        int velocity);

enum entropictron_error
ent_audio_process(struct ent_audio *audio,
                    float** out,
                    size_t channel,
                    size_t size);

enum entropictron_error
ent_audio_set_limiter_callback(struct ent_audio *audio,
                                 void (*callback)(void*, size_t index, ent_real val),
                                 void *arg);

enum entropictron_error
ent_audio_enable_note_off(struct entropictron *kick,
                            size_t id,
                            bool enable);
enum entropictron_error
ent_audio_note_off_enabled(struct ent_audio *audio,
                             size_t id,
                             bool *enabled);

#endif // GKICK_AUDIO_H
