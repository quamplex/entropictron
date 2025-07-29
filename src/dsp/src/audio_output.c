/**
 * File name: audio_output.c
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

#include "audio_output.h"

enum entropictron_error
ent_audio_output_create(struct ent_audio_output **audio_output, int sample_rate)
{
        if (audio_output == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        *audio_output = (struct ent_audio_output*)calloc(1, sizeof(struct ent_audio_output));
        if (*audio_output == NULL) {
                ent_log_error("can't allocate memory");
                return ENTROPICTRON_ERROR;
        }
        (*audio_output)->play         = false;
        (*audio_output)->start_play   = false;
        (*audio_output)->decay        = -1;
	(*audio_output)->enabled      = true;
        (*audio_output)->muted        = false;
        (*audio_output)->solo         = false;
        (*audio_output)->channel      = 0;
        (*audio_output)->playing_key  = ENTROPICTRON_ANY_KEY;
        (*audio_output)->midi_channel = ENTROPICTRON_ANY_MIDI_CHANNEL;
        (*audio_output)->sample_rate  = sample_rate;
        (*audio_output)->note_off     = false;

        ent_buffer_new(&(*audio_output)->updated_buffer,
                         (*audio_output)->sample_rate * ENTROPICTRON_MAX_LENGTH);
        if ((*audio_output)->updated_buffer == NULL) {
                ent_log_error("can't create updated buffer");
                ent_audio_output_free(audio_output);
                return ENTROPICTRON_ERROR;
        }
        ent_buffer_set_size((*audio_output)->updated_buffer, 0);

        ent_buffer_new(&(*audio_output)->playing_buffer,
                         (*audio_output)->sample_rate * ENTROPICTRON_MAX_LENGTH);
        if ((*audio_output)->playing_buffer == NULL) {
                ent_log_error("can't create playing buffer");
                ent_audio_output_free(audio_output);
                return ENTROPICTRON_ERROR;
        }
        ent_buffer_set_size((*audio_output)->playing_buffer, 0);

        /**
         * Create the instrument ring buffer.
         * The ring buffer is twice bigger than the
         * maximum instrument sound length. This permits
         * to tune the instrument sound up to an octave.
         */
        enum entropictron_error err;
        err = ring_buffer_new(&(*audio_output)->ring_buffer,
                              (*audio_output)->sample_rate * 2 * ENTROPICTRON_MAX_LENGTH);
        if (err != ENTROPICTRON_OK) {
                ent_log_error("can'r create ring buffer");
                ent_audio_output_free(audio_output);
                return ENTROPICTRON_ERROR;
        }

        if (pthread_mutex_init(&(*audio_output)->lock, NULL) != 0) {
                ent_log_error("error on init mutex");
                ent_audio_output_free(audio_output);
                return ENTROPICTRON_ERROR;
	}

        return ENTROPICTRON_OK;
}

void ent_audio_output_free(struct ent_audio_output **audio_output)
{
        if (audio_output != NULL && *audio_output != NULL) {
                ent_buffer_free(&(*audio_output)->playing_buffer);
                ent_buffer_free(&(*audio_output)->updated_buffer);
                ring_buffer_free(&(*audio_output)->ring_buffer);
                pthread_mutex_destroy(&(*audio_output)->lock);
                free(*audio_output);
                *audio_output = NULL;
        }
}

enum entropictron_error
ent_audio_output_key_pressed(struct ent_audio_output *audio_output,
                               struct ent_note_info *key)
{
        if (audio_output->muted)
                return ENTROPICTRON_OK;

        if (key->state == GKICK_KEY_STATE_PRESSED) {
                audio_output->key   = *key;
                audio_output->play  = true;
                audio_output->decay = -1;
                ent_audio_output_swap_buffers(audio_output);
                if (!ent_audio_output_note_off(audio_output)) {
                        // Add all the buffer.
                        ent_audio_add_playing_buffer_to_ring(audio_output, SIZE_MAX);
                }
        } else if (key->state == GKICK_KEY_STATE_RELEASED) {
                audio_output->key.state = key->state;
                audio_output->decay     = GEKICK_KEY_RELESE_DECAY_TIME;
        }

        return ENTROPICTRON_OK;
}

/* This funciton is called from the audio thread. */
void
ent_audio_set_play(struct ent_audio_output *audio_output)
{
        if (audio_output->muted)
                return;

        struct ent_note_info key;
        key.channel        = ENTROPICTRON_ANY_MIDI_CHANNEL;
        key.note_number    = ENTROPICTRON_ANY_KEY;
        key.velocity       = 127;
        key.state          = GKICK_KEY_STATE_PRESSED;
        ent_audio_output_key_pressed(audio_output, &key);
}

ent_real ent_audio_get_decay_val(struct ent_audio_output *audio_output)
{
        ent_real decay_val = 1.0f;
        int release_time = GEKICK_KEY_RELESE_DECAY_TIME;
        if (audio_output->key.state == GKICK_KEY_STATE_RELEASED) {
                audio_output->decay--;
                if (audio_output->decay < 0) {
                        audio_output->play = false;
                        decay_val = 0.0f;
                        audio_output->decay = -1;
                } else {
                        decay_val = - 1.0f * ((ent_real)(release_time - audio_output->decay)
                                              / release_time) + 1.0f;
                }
        }
        return decay_val;
}


void
ent_audio_add_playing_buffer_to_ring(struct ent_audio_output *audio_output,
                                       size_t size)
{
        if (!audio_output->play)
                return;

        size_t i = 0;
        ent_real factor = ent_audio_output_tune_factor(audio_output->key.note_number);
        while (i < size) {
                /**
                 * When NOTE OFF is ignored, the entire buffer needs to be added.
                 * Therefore, the loop will terminate because `size` is set to `SIZE_MAX`.
                 * When tuning is enabled, and the buffer is stretched, and its final
                 * actual size is bigger than the original and we need to count only on
                 * ent_buffer_is_end.
                 */
                if (ent_buffer_is_end(audio_output->playing_buffer))
                        break;

                ent_real val;
                if (audio_output->tune) {
                        val = ent_buffer_stretch_get_next(audio_output->playing_buffer,
                                                            factor);
                } else {
                        val = ent_buffer_get_next(audio_output->playing_buffer);
                }

                ent_real decay_val = ent_audio_get_decay_val(audio_output);
                val *= decay_val * ((ent_real)audio_output->key.velocity / 127);
                ring_buffer_add_value(audio_output->ring_buffer, i++, val);
        }
}

enum entropictron_error
ent_audio_output_play(struct ent_audio_output *audio_output)
{
        audio_output->start_play = true;
        return ENTROPICTRON_OK;
}

ent_real
ent_audio_output_tune_factor(int note_number)
{
        ent_real factor = exp2f((ent_real)(note_number - 69) / 12.0f);
        return GKICK_CLAMP(factor, 0.25f, 3.0f);
}

void ent_audio_output_lock(struct ent_audio_output *audio_output)
{
        if (audio_output != NULL)
                pthread_mutex_lock(&audio_output->lock);
}

void ent_audio_output_unlock(struct ent_audio_output *audio_output)
{
        if (audio_output != NULL)
                pthread_mutex_unlock(&audio_output->lock);
}

struct ent_buffer*
ent_audio_output_get_buffer(struct ent_audio_output  *audio_output)
{
        return audio_output->playing_buffer;
}

void ent_audio_output_swap_buffers(struct ent_audio_output *audio_output)
{
        ent_buffer_reset(audio_output->playing_buffer);

        /**
         * Try lock. If successful, swap buffers. If not, continue with
         * the current one to play until the next press of the key.
         */
        if (pthread_mutex_trylock(&audio_output->lock) == 0) {
                /* Test if the updated buffer is full. Otherwise it means that it was not updated. */
                if (ent_buffer_size(audio_output->updated_buffer) > 0
                    && ent_buffer_is_end(audio_output->updated_buffer)) {
                        struct ent_buffer *buff = audio_output->updated_buffer;
                        audio_output->updated_buffer = audio_output->playing_buffer;
                        audio_output->playing_buffer = buff;
                }
                ent_buffer_reset(audio_output->playing_buffer);
                ent_audio_output_unlock(audio_output);
        }
}

enum entropictron_error
ent_audio_output_set_playing_key(struct ent_audio_output *audio_output, signed char key)
{
        if (key < 21 || key > 108)
                key = ENTROPICTRON_ANY_KEY;
        audio_output->playing_key = key;
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_audio_output_get_playing_key(struct ent_audio_output *audio_output, signed char *key)
{
        *key = audio_output->playing_key;
        return ENTROPICTRON_OK;
}


enum entropictron_error
ent_audio_output_set_midi_channel(struct ent_audio_output *audio_output, signed char channel)
{
        if (channel < -1 && channel > 15) {
                ent_log_error("wrong channel value: %d", channel);
                return ENTROPICTRON_ERROR;
        }

        audio_output->midi_channel = channel;
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_audio_output_get_midi_channel(struct ent_audio_output *audio_output, signed char *channel)
{
        *channel = audio_output->midi_channel;
        return ENTROPICTRON_OK;
}

void ent_audio_output_tune_output(struct ent_audio_output *audio_output, bool tune)
{
        audio_output->tune = tune;
}

bool ent_audio_output_is_tune_output(struct ent_audio_output *audio_output)
{
        return audio_output->tune;
}

enum entropictron_error
ent_audio_output_set_channel(struct ent_audio_output *audio_output,
                               size_t channel)
{
        audio_output->channel = channel;
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_audio_output_get_channel(struct ent_audio_output *audio_output,
                               size_t *channel)
{
        *channel = audio_output->channel;
        return ENTROPICTRON_OK;
}

void ent_audio_get_data(struct ent_audio_output *audio_output,
                          ent_real **data,
                          ent_real *leveler,
                          size_t size)
{
        if (ent_audio_output_note_off(audio_output))
                ent_audio_add_playing_buffer_to_ring(audio_output, size);
        *leveler = ring_buffer_get_cur_data(audio_output->ring_buffer);
        ring_buffer_get_data(audio_output->ring_buffer,
                             data[0],
                             size);
        ent_real limiter = (ent_real)audio_output->limiter / 1000000;
        for (size_t i = 0; i < size; i++) {
                data[0][i] *= limiter;
                data[1][i] = data[0][i];
        }
        ring_buffer_next(audio_output->ring_buffer, size);
}

void ent_audio_output_enable_note_off(struct ent_audio_output *audio_output,
                                 bool enable)
{
        audio_output->note_off = enable;
}

bool ent_audio_output_note_off(struct ent_audio_output *audio_output)
{
        return audio_output->note_off;
}
