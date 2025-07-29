/**
 * File name: ent_jack.c
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor 
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

#ifndef GKICK_JACK_H
#define GKICK_JACK_H

#include "entropictron_internal.h"
#include "mixer.h"

#include <jack/jack.h>
#include <jack/midiport.h>

struct ent_jack {
        jack_port_t *output_port[2 * ENTROPICTRON_MAX_CHANNELS];
        jack_port_t *midi_in_port;
        jack_client_t *client;
        jack_nframes_t sample_rate;
        struct ent_mixer *mixer;
        pthread_mutex_t lock;
};

int
ent_jack_process_callback(jack_nframes_t nframes,
			    void *arg);

jack_nframes_t
ent_jack_sample_rate(struct ent_jack *jack);

enum entropictron_error
ent_jack_get_output_buffers(struct ent_jack *jack,
                              jack_default_audio_sample_t **channel_buf,
                              jack_nframes_t nframes);

void ent_jack_get_note_info(jack_midi_event_t *event,
                              struct ent_note_info *note);

jack_port_t*
ent_jack_get_midi_in_port(struct ent_jack *jack);

int ent_jack_srate_callback(jack_nframes_t nframes,
                              void *arg);

enum entropictron_error
ent_jack_enable_midi_in(struct ent_jack *jack,
                          const char *name);

enum entropictron_error
ent_jack_create_output_ports(struct ent_jack *jack);

enum entropictron_error
ent_create_jack(struct ent_jack **jack);

enum entropictron_error
entropictron_jack_start(struct ent_jack *jack, struct ent_mixer *mixer);

int
ent_jack_is_midi_in_enabled(struct ent_jack *jack);

void
ent_jack_free(struct ent_jack **jack);

void
ent_jack_lock(struct ent_jack *jack);

void
ent_jack_unlock(struct ent_jack *jack);

#endif // GKICK_JACK_H
