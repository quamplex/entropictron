/**
 * File name: ent_audio.c
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

#include "ent_audio.h"
#include "audio_output.h"
#include "mixer.h"
#ifdef ENTROPICTRON_AUDIO_JACK
#include "ent_jack.h"
#endif

enum entropictron_error
ent_audio_create(struct ent_audio** audio, int sample_rate)
{
        if (audio == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }


        *audio = (struct ent_audio*)calloc(1, sizeof(struct ent_audio));
        if (*audio == NULL) {
                ent_log_error("can't allocate memory");
		return ENTROPICTRON_ERROR_MEM_ALLOC;
	}

        (*audio)->sample_rate = sample_rate;
#ifdef ENTROPICTRON_AUDIO_JACK
        if (ent_create_jack(&(*audio)->jack) != ENTROPICTRON_OK) {
                ent_log_warning("can't create jack module. "
				  "Jack server is either not running or not installed");
        } else {
                (*audio)->sample_rate = ent_jack_sample_rate((*audio)->jack);
        }
#endif // ENTROPICTRON_AUDIO_JACK
        ent_log_debug("sample rate: %d", (*audio)->sample_rate);

        for (size_t i = 0; i < ENTROPICTRON_MAX_INSTRUMENTS + 1; i++) {
                if (ent_audio_output_create(&(*audio)->audio_outputs[i], sample_rate) != ENTROPICTRON_OK) {
                        ent_log_error("can't create audio output");
                        ent_audio_free(audio);
                        return ENTROPICTRON_ERROR;
                }
		(*audio)->audio_outputs[i]->enabled = true;
                if (i == ENTROPICTRON_AUDITION_CHANNEL_INDEX)
                        (*audio)->audio_outputs[i]->channel = ENTROPICTRON_AUDITION_CHANNEL_INDEX;
        }
        (*audio)->audio_outputs[ENTROPICTRON_MAX_INSTRUMENTS]->limiter = 1000000;

	if (ent_mixer_create(&(*audio)->mixer) != ENTROPICTRON_OK) {
		ent_log_error("can't create mixer");
		ent_audio_free(audio);
		return ENTROPICTRON_ERROR;
	}
	(*audio)->mixer->audio_outputs = (*audio)->audio_outputs;

        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_start_audio(struct ent_audio *audio)
{
#ifdef ENTROPICTRON_AUDIO_JACK
        if (audio->jack != NULL
            && entropictron_jack_start(audio->jack, audio->mixer) != ENTROPICTRON_OK) {
                ent_log_warning("can't start jack module");
                return ENTROPICTRON_ERROR;
        }
#endif // ENTROPICTRON_AUDIO_JACK

        return ENTROPICTRON_OK;
}

void ent_audio_free(struct ent_audio** audio)
{
        if (audio != NULL && *audio != NULL) {
#ifdef ENTROPICTRON_AUDIO_JACK
                ent_jack_free(&(*audio)->jack);
#endif // ENTROPICTRON_AUDIO_JACK
		ent_mixer_free(&(*audio)->mixer);
                for (size_t i = 0; i < ENTROPICTRON_MAX_INSTRUMENTS + 1; i++)
                        ent_audio_output_free(&(*audio)->audio_outputs[i]);
                free(*audio);
                *audio = NULL;
        }
}

enum entropictron_error
ent_audio_set_limiter_val(struct ent_audio *audio,
                            size_t index,
                            ent_real limit)
{
        if (limit < 0.0f)
                limit = 0.0f;
        else if (limit > 10.0f)
                limit = 10.0f;
        return ent_mixer_limiter_set(audio->mixer,
                                       index,
                                       limit);
}

enum entropictron_error
ent_audio_get_limiter_val(struct ent_audio *audio,
                            size_t index,
                            ent_real *limit)
{
	return ent_mixer_limiter_get(audio->mixer,
                                       index,
                                       limit);
}

enum entropictron_error
ent_audio_play(struct ent_audio *audio,
                 size_t id)
{
        if (audio == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_log_info("play %d", id);
        if (id < ENTROPICTRON_MAX_INSTRUMENTS && audio->audio_outputs[id]->enabled) {
                ent_audio_output_play(audio->audio_outputs[id]);
        } else if (id == ENTROPICTRON_AUDITION_CHANNEL_INDEX) {
                // Play sample preview.
                ent_audio_output_play(audio->audio_outputs[id]);
        }
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_audio_key_pressed(struct ent_audio *audio,
                        bool pressed,
                        int note,
                        int velocity)
{
        if (audio == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        struct ent_note_info key;
        key.channel     = 1;
        key.note_number = note;
        key.velocity    = velocity;
        key.state       = pressed ? GKICK_KEY_STATE_PRESSED : GKICK_KEY_STATE_RELEASED;
        return ent_mixer_key_pressed(audio->mixer, &key);
}

enum entropictron_error
ent_audio_process(struct ent_audio *audio,
                    float** out,
                    size_t channel,
                    size_t size)
{
        return ent_mixer_process(audio->mixer, out, channel, size);
}

enum entropictron_error
ent_audio_set_limiter_callback(struct ent_audio *audio,
                                 void (*callback)(void*, size_t index, ent_real val),
                                 void *arg)
{
        return ent_mixer_set_limiter_callback(audio->mixer,
                                                callback,
                                                arg);
}
