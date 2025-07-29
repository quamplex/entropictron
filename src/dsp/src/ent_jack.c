/**
 * File name: ent_kack.h
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

#include "ent_jack.h"
#include "oscillator.h"

enum entp_noise_type {
        ENTROPICTRON_NOISE_WHITE    = 0,
        ENTROPICTRON_NOISE_BROWNIAN = 1,
        ENTROPICTRON_NOISE_PINK     = 2
};

struct entp_noise {
        enum entp_noise_type noise_type;
        uint32_t seed;
        float density;
        float amplitude;
        float stereo_variation;
};

struct entropictron {
        struct entp_noise noise;
};

struct entropictron entp_low = {
    .noise = {
        .seed = 234,
        .density = 0.5f,
        .amplitude = 0.05f,
        .stereo_variation = 0.2f
    }
};

struct entropictron entp = {
    .noise = {
        .seed = 890,
        .density = 0.001f,
        .amplitude = 0.5f,
        .stereo_variation = 0.2f
    }
};

struct entropictron entp_h = {
    .noise = {
        .seed = 77,
        .density = 0.0005f,
        .amplitude = 1.0f,
        .stereo_variation = 0.2f
    }
};

// Linear Congruential Generator for simple RNG
static inline uint32_t lcg_next(uint32_t *state) {
    *state = (*state * 1664525u + 1013904223u);
    return *state;
}

// Convert 32-bit integer to float in [-1.0, 1.0)
static inline float rand_float(uint32_t *state) {
    uint32_t r = lcg_next(state);
    return ((int32_t)(r >> 1)) / 1073741824.0f; // 2^30
}

static bool entp_noise_process(struct entp_noise *noise,
                               float** out,
                               size_t offset,
                               size_t size)
{
        if (!noise || !out || !out[0] || !out[1])
        return false;

        float *outputR = out[0];
        float *outputL = out[1];
        uint32_t state = noise->seed;

        for (size_t i = 0; i < size; ++i) {
                // Generate a uniform random number in [0,1)
                float p = rand_float(&state);
                float val = 0.0f;
                if (p <= noise->density)
                        val = noise->amplitude * rand_float(&state);
                //                outputL[offset + i] += val;
                //val = 0.0f;
                //p = rand_float(&state);
                //if (p <= noise->density)
                //val = noise->amplitude * rand_float(&state);
                outputR[offset + i] += val;
                outputL[offset + i] += val;
        }
        noise->seed = state;

        return true;
}

static bool entp_process(struct entropictron *enpt,
                         float** out,
                         size_t offset,
                         size_t size)
{
        entp_noise_process(&(enpt->noise), out, offset, size);
        return true;
}

int
ent_jack_process_callback(jack_nframes_t nframes,
			    void *arg)
{
        if (nframes < 1)
                return 0;

        struct ent_jack *jack = (struct ent_jack*)arg;
	jack_default_audio_sample_t *buffer[2 * ENTROPICTRON_MAX_CHANNELS];
        ent_jack_get_output_buffers(jack, buffer, nframes);
        void* port_buf = jack_port_get_buffer(jack->midi_in_port, nframes);

	jack_midi_event_t event;
	jack_nframes_t events_count = jack_midi_get_event_count(port_buf);
        jack_nframes_t event_index  = 0;
        if (events_count > 0)
                jack_midi_event_get(&event, port_buf, event_index);

        for (size_t i = 0; i < 2 * ENTROPICTRON_MAX_CHANNELS; i++)
                memset(buffer[i], 0, nframes * sizeof(float));

        size_t currentFrame = 0;
        size_t offset = 0;
        while (event_index < events_count) {
                size_t eventFrame = event.time;
                size_t size = eventFrame - currentFrame;
                if (size > 0) {
                        entp_process(&entp_low, buffer, offset, nframes - currentFrame);
                        entp_process(&entp, buffer, offset, size);
                        entp_process(&entp_h, buffer, offset, nframes - currentFrame);
                        offset += size;
                }

                struct ent_note_info note;
                memset(&note, 0, sizeof(struct ent_note_info));
                ent_jack_get_note_info(&event, &note);
                if (note.state == GKICK_KEY_STATE_PRESSED
                    || note.state == GKICK_KEY_STATE_RELEASED) {
                        ent_mixer_key_pressed(jack->mixer, &note);
                }
                currentFrame = eventFrame;
                if (++event_index < events_count)
                        jack_midi_event_get(&event, port_buf, event_index);
        }

        if (currentFrame < nframes) {
                entp_process(&entp_low, buffer, offset, nframes - currentFrame);
                entp_process(&entp, buffer, offset, nframes - currentFrame);
                entp_process(&entp_h, buffer, offset, nframes - currentFrame);
        }

        return 0;
}

jack_nframes_t
ent_jack_sample_rate(struct ent_jack *jack)
{
        jack_nframes_t sample_rate;
        if (jack == NULL) {
                ent_log_error("wrong arguments");
                return 0;
        }

        ent_jack_lock(jack);
        sample_rate = jack->sample_rate;
        ent_jack_unlock(jack);

        return sample_rate;
}

enum entropictron_error
ent_jack_get_output_buffers(struct ent_jack *jack,
                              jack_default_audio_sample_t **channel_buf,
                              jack_nframes_t nframes)
{
        for (size_t i = 0; i < ENTROPICTRON_MAX_CHANNELS; i++) {
                channel_buf[2 * i] =
                        (jack_default_audio_sample_t*)jack_port_get_buffer(jack->output_port[2 * i],
                                                                           nframes);
                channel_buf[2 * i + 1] =
                        (jack_default_audio_sample_t*)jack_port_get_buffer(jack->output_port[2 * i + 1],
                                                                           nframes);

        }
        return ENTROPICTRON_OK;
}

void
ent_jack_get_note_info(jack_midi_event_t *event,
                         struct ent_note_info *note)
{
        if (event == NULL || note == NULL)
                return;

        note->state = GKICK_KEY_STATE_DEFAULT;
        if (((*(event->buffer) & 0xf0)) == 0x90) {
                note->state = GKICK_KEY_STATE_PRESSED;
                note->channel     = *(event->buffer) & 0x0f;
                note->note_number = event->buffer[1];
                note->velocity    = event->buffer[2];
        } else if(((*(event->buffer) & 0xf0)) == 0x80) {
                note->state       = GKICK_KEY_STATE_RELEASED;
                note->channel     = *(event->buffer) & 0x0f;
                note->note_number = event->buffer[1];
                note->velocity    = event->buffer[2];
        }
        ent_log_debug("note->channel: %d", note->channel);
}

jack_port_t*
ent_jack_get_midi_in_port(struct ent_jack *jack)
{
        jack_port_t *port;

        if (jack == NULL) {
                ent_log_error("wrong arguments");
                return NULL;
        }

        ent_jack_lock(jack);
        port = jack->midi_in_port;
        ent_jack_unlock(jack);

        return port;
}

int ent_jack_srate_callback(jack_nframes_t nframes,
                              void *arg)
{
	return 0;
}

enum entropictron_error
ent_jack_enable_midi_in(struct ent_jack *jack,
                          const char *name)
{
        const char *midi_name;
        enum entropictron_error error;

        if (jack == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        if (name == NULL)
                midi_name = "midi_in";
        else
                midi_name = name;

        error = ENTROPICTRON_OK;
        ent_jack_lock(jack);
        if (jack->midi_in_port == NULL) {
                jack->midi_in_port = jack_port_register(jack->client, midi_name,
                                                JACK_DEFAULT_MIDI_TYPE,
                                                JackPortIsInput, 0);
                if (jack->midi_in_port == NULL) {
                        ent_log_error("can't register port: %s",
                                           midi_name);
                        error = ENTROPICTRON_ERROR;
                }
        } else {
                ent_log_warning("midi in is already set");
        }
        ent_jack_unlock(jack);
        return error;
}

enum entropictron_error
ent_jack_create_output_ports(struct ent_jack *jack)
{
        enum entropictron_error error;
        if (jack == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        error = ENTROPICTRON_OK;
        ent_jack_lock(jack);
        for (size_t i = 0; i < ENTROPICTRON_MAX_CHANNELS; i++) {
                char name[30];
                if (i == ENTROPICTRON_AUDITION_CHANNEL_INDEX)
                        sprintf(name, "auditionL");
                else
                        sprintf(name, "outL_%u", (unsigned int)(i + 1));
                jack->output_port[2 * i] = jack_port_register(jack->client, name,
                                                              JACK_DEFAULT_AUDIO_TYPE,
                                                              JackPortIsOutput, 0);
                if (jack->output_port[2 * i] == NULL) {
                        ent_log_error("can't register output ports");
                        error = ENTROPICTRON_ERROR;
                        break;
                }

                if (i == ENTROPICTRON_AUDITION_CHANNEL_INDEX)
                        sprintf(name, "auditionR");
                else
                        sprintf(name, "outR_%u", (unsigned int)(i + 1));
                jack->output_port[2 * i + 1] = jack_port_register(jack->client, name,
                                                                  JACK_DEFAULT_AUDIO_TYPE,
                                                                  JackPortIsOutput, 0);
                if (jack->output_port[2 * i + 1] == NULL) {
                        ent_log_error("can't register output ports");
                        error = ENTROPICTRON_ERROR;
                        break;
                }
        }
        ent_jack_unlock(jack);
        return error;
}

enum entropictron_error
ent_create_jack(struct ent_jack **jack)
{
        if (jack == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        *jack = (struct ent_jack*)calloc(1, sizeof(struct ent_jack));
        if (*jack == NULL)
                return ENTROPICTRON_ERROR;
        if (pthread_mutex_init(&(*jack)->lock, NULL) != 0) {
                ent_log_error("error on init mutex");
                ent_jack_free(jack);
                return ENTROPICTRON_ERROR;
        }

        (*jack)->client = jack_client_open(ENTROPICTRON_NAME,
                                           JackNoStartServer,
                                           NULL);
        if ((*jack)->client == NULL) {
                ent_log_error("can't create jack client");
                ent_jack_free(jack);
                return ENTROPICTRON_ERROR;
        }

        (*jack)->sample_rate = jack_get_sample_rate((*jack)->client);

        jack_set_process_callback((*jack)->client,
                                  ent_jack_process_callback,
                                  (void*)(*jack));

        if (ent_jack_create_output_ports(*jack) != ENTROPICTRON_OK) {
                ent_log_error("can't create output ports");
                ent_jack_free(jack);
                return ENTROPICTRON_ERROR;
        }

        ent_jack_enable_midi_in(*jack, "midi_in");

	return ENTROPICTRON_OK;
}

enum entropictron_error
entropictron_jack_start(struct ent_jack *jack, struct ent_mixer *mixer)
{
        jack->mixer = mixer;
        if (jack_activate(jack->client) != 0) {
                ent_log_error("cannot activate client");
                return ENTROPICTRON_ERROR;
        }

        // Connect to system playback ports.
        for (size_t i = 0; i < ENTROPICTRON_MAX_CHANNELS; i++) {
                jack_connect(jack->client,
                             jack_port_name(jack->output_port[2 * i]),
                             "system:playback_1");
                jack_connect(jack->client,
                             jack_port_name(jack->output_port[2 * i + 1]),
                             "system:playback_2");
        }
        return ENTROPICTRON_OK;
}


int
ent_jack_is_midi_in_enabled(struct ent_jack *jack)
{
        int enabled;

        if (jack == NULL) {
                ent_log_error("wrong argument");
                return 0;
        }

        ent_jack_lock(jack);
        enabled = (jack->midi_in_port != NULL) ? 1 : 0;
        ent_jack_unlock(jack);

        return enabled;
}

void
ent_jack_free(struct ent_jack **jack)
{
        if (jack != NULL && *jack != NULL) {
                if ((*jack)->client != NULL) {
                        jack_deactivate((*jack)->client);
                        for (size_t i = 0; i < 2 * ENTROPICTRON_MAX_CHANNELS; i++) {
                                if ((*jack)->output_port[i] != NULL) {
                                        jack_port_unregister((*jack)->client,
                                                             (*jack)->output_port[i]);
                                }
                        }
                        jack_client_close((*jack)->client);
                }

                free(*jack);
                *jack = NULL;
        }
}

void
ent_jack_lock(struct ent_jack *jack)
{
        if (jack != NULL)
                pthread_mutex_lock(&jack->lock);
}

void
ent_jack_unlock(struct ent_jack *jack)
{
        if (jack != NULL)
                pthread_mutex_unlock(&jack->lock);
}
