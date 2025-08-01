/**
 * File name: audio_output.h
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

#ifndef GKICK_AUDO_OUTPUT_H
#define GKICK_AUDO_OUTPUT_H

#include "entropictron_internal.h"
#include "ring_buffer.h"

#include <pthread.h>
#include <stdatomic.h>

/**
 * Audio output (which maybe is not the best name)
 * is an interface to an array that holds the synthesised percussion.
 * Is has nothing to do with any real audio interfaces.
 * It provides access to the samples from the array in a "playable mode",
 * i.e. in a state machine fashion.
 */

/* Decay time measured in number of audio frames. */
#define GEKICK_KEY_RELESE_DECAY_TIME 1000

struct ent_note_info {
        enum ent_key_state state;
        signed char channel;
        signed char note_number;
        signed char velocity;
};

struct ent_audio_output
{
        int sample_rate;

	/* Specifies if this audio output is active. */
        _Atomic bool enabled;
        struct ent_buffer *updated_buffer;
        struct ent_buffer* playing_buffer;
        struct ring_buffer* ring_buffer;
        _Atomic bool buffer_updated;

        /* Note info is changed only by the audio thread. */
        struct ent_note_info key;

        /* The key number that triggres playing. */
        _Atomic signed char playing_key;

        /* MIDI channel for playing. */
        _Atomic signed char midi_channel;

        /**
         * Specifies if the audio output is in the
         * playing state (the percussion is playing)
         */
        _Atomic bool play;

        /**
         * Triggers the play, usualy changed from UI call.
         */
        _Atomic bool start_play;

        /**
         * decay - note release time measured in number of audio frames.
         * Relaxation curve for audio is liniear:
         *   - 1.0 * (GEKICK_NOTE_RELEASE_TIME - decay) / GEKICK_NOTE_RELEASE_TIME + 1.0,
         *    decay from GEKICK_NOTE_RELEASE_TIME to 0;
         */
        _Atomic int decay;

        /**
         * Specifies if to tune the output in accordance with
         * the note (central note is A4).
         */
        _Atomic bool tune;

        /* Output channel. */
      	atomic_size_t channel;

        /* The output is muted or not. */
        _Atomic bool muted;

        /* The output is muted or not. */
        _Atomic bool solo;

        /* Output audio limiter value. */
        atomic_int limiter;

        /* Enable/disable note off */
        _Atomic bool note_off;

        pthread_mutex_t lock;
};

enum entropictron_error
ent_audio_output_create(struct ent_audio_output **audio_output, int sample_rate);

void ent_audio_output_free(struct ent_audio_output **audio_output);

struct ent_buffer*
ent_audio_output_get_buffer(struct ent_audio_output  *audio_output);

enum entropictron_error
ent_audio_output_key_pressed(struct ent_audio_output *audio_output,
                               struct ent_note_info *key);

void
ent_audio_add_playing_buffer_to_ring(struct ent_audio_output *audio_output, size_t size);

enum entropictron_error
ent_audio_output_play(struct ent_audio_output *audio_output);

/* This funciton is called from the audio thread. */
void
ent_audio_set_play(struct ent_audio_output *audio_output);

ent_real
ent_audio_get_decay_val(struct ent_audio_output *audio_output);

ent_real
ent_audio_output_tune_factor(int note_number);

void ent_audio_output_lock(struct ent_audio_output *audio_output);

void ent_audio_output_unlock(struct ent_audio_output *audio_output);

void ent_audio_output_swap_buffers(struct ent_audio_output *audio_output);

enum entropictron_error
ent_audio_output_set_playing_key(struct ent_audio_output *audio_output, signed char key);

enum entropictron_error
ent_audio_output_set_midi_channel(struct ent_audio_output *audio_output, signed char channel);

enum entropictron_error
ent_audio_output_get_midi_channel(struct ent_audio_output *audio_output, signed char *channel);

enum entropictron_error
ent_audio_output_get_playing_key(struct ent_audio_output *audio_output, signed char *key);

void ent_audio_output_tune_output(struct ent_audio_output *audio_output, bool tune);

bool ent_audio_output_is_tune_output(struct ent_audio_output *audio_output);

enum entropictron_error
ent_audio_output_set_channel(struct ent_audio_output *audio_output,
                               size_t channel);

enum entropictron_error
ent_audio_output_get_channel(struct ent_audio_output *audio_output,
                               size_t *channel);

void ent_audio_get_data(struct ent_audio_output *audio_output,
                          ent_real **data,
                          ent_real *leveler,
                          size_t size);

void ent_audio_output_enable_note_off(struct ent_audio_output *audio_output,
                                 bool enable);

bool ent_audio_output_note_off(struct ent_audio_output *audio_output);


#endif // GKICK_AUDO_OUTPUT_H
