/**
 * File name: synthesizer.c
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor
 *
 * This file is part of GeonKick.
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

#include "synthesizer.h"
#include "oscillator.h"

enum entropictron_error
ent_synth_new(struct ent_synth **synth, int sample_rate)
{
        if (synth == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        *synth = (struct ent_synth*)calloc(1, sizeof(struct ent_synth));
	if (*synth == NULL) {
                ent_log_error("can't allocate memory");
		return ENTROPICTRON_ERROR_MEM_ALLOC;
	}
	memset(*synth, 0, sizeof(struct ent_synth));
	(*synth)->sample_rate = sample_rate;
        (*synth)->length = 0.3f;
	(*synth)->oscillators_number = GKICK_OSC_GROUPS_NUMBER * GKICK_OSC_GROUP_SIZE;
        (*synth)->buffer_update = 0;
        (*synth)->amplitude = 1.0f;
        (*synth)->buffer_update = false;
        (*synth)->is_active = false;
        memset((*synth)->name, '\0', sizeof((*synth)->name));
        for (size_t i = 0; i < GKICK_OSC_GROUPS_NUMBER; i++)
                (*synth)->osc_groups_amplitude[i] = 1.0f;

        if (ent_filter_new(&(*synth)->filter, (*synth)->sample_rate) != ENTROPICTRON_OK) {
                ent_log_error("can't create filter");
                ent_synth_free(synth);
		return ENTROPICTRON_ERROR;
        }
        (*synth)->filter_enabled = 0;

        if (ent_distortion_new(&(*synth)->distortion, (*synth)->sample_rate) != ENTROPICTRON_OK) {
                ent_log_error("can't create distortion");
                ent_synth_free(synth);
                return ENTROPICTRON_ERROR;
        }

        (*synth)->envelope = ent_envelope_create();
        if ((*synth)->envelope == NULL) {
                ent_log_error("can't create envelope");
                ent_synth_free(synth);
                return ENTROPICTRON_ERROR;
        } else {
                /* Add two default points. */
                struct ent_envelope_point_info info = {0.0f, 1.0f, false};
                ent_envelope_add_point((*synth)->envelope, &info);
                info.x = 1.0f;
                ent_envelope_add_point((*synth)->envelope, &info);
        }

        /* Create synthesizer kick buffer. */
        struct ent_buffer *buff;
        ent_buffer_new(&buff, (*synth)->sample_rate * ENTROPICTRON_MAX_LENGTH);
        if (buff == NULL) {
                ent_log_error("can't create synthesizer kick buffer");
                ent_synth_free(synth);
        }
        ent_buffer_set_size(buff, (size_t)((*synth)->length * (*synth)->sample_rate));
        (*synth)->buffer = buff;

        if (ent_synth_create_oscillators(*synth) != ENTROPICTRON_OK) {
                ent_log_error("can't create oscillators");
                ent_synth_free(synth);
                return ENTROPICTRON_ERROR;
        }

        return ENTROPICTRON_OK;
}

void ent_synth_free(struct ent_synth **synth)
{
        if (synth != NULL && *synth != NULL) {
                if ((*synth)->oscillators != NULL) {
                        for (size_t i = 0; i < (*synth)->oscillators_number; i++)
                                ent_osc_free(&((*synth)->oscillators[i]));
                        free((*synth)->oscillators);
                        (*synth)->oscillators = NULL;

                        if ((*synth)->buffer != NULL)
                                ent_buffer_free(&(*synth)->buffer);

                        if ((*synth)->filter)
                                ent_filter_free(&(*synth)->filter);

                        if ((*synth)->distortion)
                                ent_distortion_free(&(*synth)->distortion);

                        if ((*synth)->envelope) {
                                ent_envelope_destroy((*synth)->envelope);
                                (*synth)->envelope = NULL;
                        }
                }

                pthread_mutex_destroy(&(*synth)->lock);
                free(*synth);
                *synth = NULL;
        }
}

void ent_synth_lock(struct ent_synth *synth)
{
        pthread_mutex_lock(&synth->lock);
}

void ent_synth_unlock(struct ent_synth *synth)
{
        pthread_mutex_unlock(&synth->lock);
}

enum entropictron_error
ent_synth_create_oscillators(struct ent_synth *synth)
{
        if (synth == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        size_t size;
        size_t i;
        struct ent_oscillator *osc;

        size = sizeof(struct ent_oscillator*) * synth->oscillators_number;
        synth->oscillators = (struct ent_oscillator**)malloc(size);

        if (synth->oscillators == NULL)
                return ENTROPICTRON_ERROR_MEM_ALLOC;
        memset(synth->oscillators, 0, size);

        for (i = 0; i < synth->oscillators_number; i++) {
                osc = ent_osc_create(synth->sample_rate);
                if (osc == NULL)
                        return ENTROPICTRON_ERROR;
                synth->oscillators[i] = osc;
        }

        return ENTROPICTRON_OK;
}

struct ent_oscillator*
ent_synth_get_oscillator(struct ent_synth *synth,
                           size_t index)
{
        if (synth == NULL) {
                ent_log_error("wrong arguments");
                return NULL;
        }

        if (index < synth->oscillators_number)
                return synth->oscillators[index];

        return NULL;
}

enum entropictron_error
ent_synth_get_oscillators_number(struct ent_synth *synth,
                                   size_t *number)
{
        if (synth == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        *number = synth->oscillators_number;
        ent_synth_unlock(synth);

        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_enable_oscillator(struct ent_synth *synth,
                              size_t index,
                              int enable)
{
        if (synth == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
	struct ent_oscillator* osc = ent_synth_get_oscillator(synth, index);
	if (osc == NULL) {
		ent_log_error("can't get oscillator");
		ent_synth_unlock(synth);
		return ENTROPICTRON_ERROR;
	}

        if (enable)
                ent_osc_set_state(osc, ENTROPICTRON_OSC_STATE_ENABLED);
        else
                ent_osc_set_state(osc, ENTROPICTRON_OSC_STATE_DISABLED);

        if (synth->osc_groups[index / GKICK_OSC_GROUP_SIZE])
                synth->buffer_update = true;

	ent_synth_unlock(synth);

        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_osc_is_enabled(struct ent_synth *synth,
                           size_t index,
                           int *enabled)
{
        struct ent_oscillator* osc;

        if (synth == NULL || enabled == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
	osc = ent_synth_get_oscillator(synth, index);
	if (osc == NULL) {
		ent_log_error("can't get oscillator");
		ent_synth_unlock(synth);
		return ENTROPICTRON_ERROR;
	}
        *enabled = ent_osc_enabled(osc);
	ent_synth_unlock(synth);

        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_osc_set_fm(struct ent_synth *synth,
                       size_t index,
                       bool is_fm)
{
        ent_synth_lock(synth);
	struct ent_oscillator* osc = ent_synth_get_oscillator(synth, index);
	if (osc == NULL) {
		ent_log_error("can't get oscillator");
		ent_synth_unlock(synth);
		return ENTROPICTRON_ERROR;
	}

        osc->is_fm = is_fm;
        if (osc->state == ENTROPICTRON_OSC_STATE_ENABLED)
                synth->buffer_update = true;

	ent_synth_unlock(synth);

        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_osc_is_fm(struct ent_synth *synth,
                      size_t index,
                      bool *is_fm)
{
        ent_synth_lock(synth);
	struct ent_oscillator* osc = ent_synth_get_oscillator(synth, index);
	if (osc == NULL) {
		ent_log_error("can't get oscillator");
		ent_synth_unlock(synth);
		return ENTROPICTRON_ERROR;
	}

        *is_fm = osc->is_fm;
	ent_synth_unlock(synth);

        return ENTROPICTRON_OK;
}

struct ent_envelope*
ent_synth_osc_get_env(struct ent_synth *synth,
                        size_t osc_index,
                        size_t env_index)
{
        if (synth == NULL) {
                ent_log_error("wrong arguments");
                return NULL;
        }

        struct ent_oscillator *osc;
        osc = ent_synth_get_oscillator(synth, osc_index);
        if (osc == NULL) {
                 ent_log_error("can't get oscillator %d", osc_index);
                 return NULL;
        }

        return ent_osc_get_envelope(osc, env_index);
}

enum entropictron_error
ent_synth_osc_envelope_points(struct ent_synth *synth,
                                int osc_index,
                                int env_index,
                                struct ent_envelope_point_info **buf,
                                size_t *npoints)
{
        if (synth == NULL || buf == NULL || npoints == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        *npoints = 0;
        *buf = NULL;

        ent_synth_lock(synth);
        struct ent_oscillator *osc = ent_synth_get_oscillator(synth, osc_index);
        if (osc == NULL) {
                ent_log_error("can't get oscillator %d", osc_index);
                ent_synth_unlock(synth);
                return ENTROPICTRON_ERROR;
        }
        ent_osc_get_envelope_points(osc, env_index, buf, npoints);
        ent_synth_unlock(synth);

        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_osc_envelope_set_points(struct ent_synth *synth,
                                    int osc_index,
                                    int env_index,
                                    const struct ent_envelope_point_info *buf,
                                    size_t npoints)
{
        if (synth == NULL || buf == NULL || npoints == 0) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        struct ent_oscillator *osc = ent_synth_get_oscillator(synth, osc_index);
        if (osc == NULL) {
                ent_log_error("can't get oscillator %d", osc_index);
                ent_synth_unlock(synth);
                return ENTROPICTRON_ERROR;
        }
        ent_osc_set_envelope_points(osc, env_index, buf, npoints);
        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == ENTROPICTRON_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
        }
        ent_synth_unlock(synth);

        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_osc_env_add_point(struct ent_synth *synth,
                              int osc_index,
                              int env_index,
                              struct ent_envelope_point_info *point_info)
{
        if (synth == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        struct ent_oscillator *osc = ent_synth_get_oscillator(synth, osc_index);
        if (osc == NULL) {
                 ent_log_error("can't get oscillator %d", osc_index);
                 ent_synth_unlock(synth);
                 return ENTROPICTRON_ERROR;
        }
        struct ent_envelope *env = ent_osc_get_envelope(osc, env_index);
        if (env == NULL) {
                ent_synth_unlock(synth);
                ent_log_error("can't get envelope");
                return ENTROPICTRON_ERROR;
        }
        if (ent_envelope_add_point(env, point_info) == NULL) {
                ent_log_error("can't add envelope point");
                ent_synth_unlock(synth);
                return ENTROPICTRON_ERROR;
        }

        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == ENTROPICTRON_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
        }

        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_osc_env_remove_point(struct ent_synth *synth,
                                 size_t osc_index,
                                 size_t env_index,
                                 size_t index)
{
        if (synth == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        struct ent_oscillator *osc = ent_synth_get_oscillator(synth, osc_index);
        if (osc == NULL) {
                 ent_log_error("can't get oscillator %d", osc_index);
                 ent_synth_unlock(synth);
                 return ENTROPICTRON_ERROR;
        }
        struct ent_envelope *env = ent_osc_get_envelope(osc, env_index);
        if (env == NULL) {
                ent_synth_unlock(synth);
                ent_log_error("can't get envelope");
                return ENTROPICTRON_ERROR;
        }
        ent_envelope_remove_point(env, index);
        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == ENTROPICTRON_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
        }

        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_osc_env_update_point(struct ent_synth *synth,
                                 int osc_index,
                                 int env_index,
                                 int index,
                                 struct ent_envelope_point_info *point_info)
{
        if (synth == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        struct ent_oscillator *osc = ent_synth_get_oscillator(synth, osc_index);
        if (osc == NULL) {
                 ent_log_error("can't get oscillator %d", osc_index);
                 ent_synth_unlock(synth);
                 return ENTROPICTRON_ERROR;
        }

        struct ent_envelope *env = ent_osc_get_envelope(osc, env_index);
        if (env == NULL) {
                ent_synth_unlock(synth);
                ent_log_error("can't get envelope");
                return ENTROPICTRON_ERROR;
        }

        ent_envelope_update_point(env, index, point_info);
        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == ENTROPICTRON_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
        }
        ent_synth_unlock(synth);

        return ENTROPICTRON_OK;
}

enum entropictron_error
synth_osc_env_set_apply_type(struct ent_synth *synth,
			     size_t osc_index,
			     size_t env_index,
			     enum ent_envelope_apply_type apply_type)
{
	if (synth == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
	        struct ent_oscillator *osc = ent_synth_get_oscillator(synth, osc_index);
        if (osc == NULL) {
                 ent_log_error("can't get oscillator %d", osc_index);
                 ent_synth_unlock(synth);
                 return ENTROPICTRON_ERROR;
        }

        struct ent_envelope *env = ent_osc_get_envelope(osc, env_index);
        if (env == NULL) {
                ent_synth_unlock(synth);
                ent_log_error("can't get envelope");
                return ENTROPICTRON_ERROR;
        }
        ent_envelope_set_apply_type(env, apply_type);
	osc = ent_synth_get_oscillator(synth, osc_index);
        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == ENTROPICTRON_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
        }
        ent_synth_unlock(synth);

        return ENTROPICTRON_OK;
}

enum entropictron_error
synth_osc_env_get_apply_type(struct ent_synth *synth,
			     size_t osc_index,
			     size_t env_index,
			     enum ent_envelope_apply_type *apply_type)
{
	if (synth == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
	struct ent_envelope *env = ent_synth_osc_get_env(synth, osc_index, env_index);
        if (env == NULL) {
                ent_synth_unlock(synth);
                ent_log_error("can't get envelope %d", env_index);
                return ENTROPICTRON_ERROR;
        }
	*apply_type = ent_envelope_get_apply_type(env);
        ent_synth_unlock(synth);

        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_set_osc_function(struct ent_synth *synth,
                             size_t osc_index,
                             enum entropictron_osc_func_type type)
{
        struct ent_oscillator *osc;
        if (synth == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        osc = ent_synth_get_oscillator(synth, osc_index);
        if (osc == NULL) {
                ent_log_error("can't get oscilaltor");
                ent_synth_unlock(synth);
                return ENTROPICTRON_ERROR;
        } else {
                osc->func = type;
        }

        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
                    && osc->state == ENTROPICTRON_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
        }

        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_get_osc_function(struct ent_synth *synth,
                             size_t osc_index,
                             enum entropictron_osc_func_type *type)
{
        struct ent_oscillator *osc;

        if (synth == NULL || type == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        osc = ent_synth_get_oscillator(synth, osc_index);
        if (osc == NULL) {
                ent_log_error("can't get oscilaltor");
                ent_synth_unlock(synth);
                return ENTROPICTRON_ERROR;
        } else {
                *type = osc->func;
        }

        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_set_osc_phase(struct ent_synth *synth,
                          size_t osc_index,
                          ent_real phase)
{
        struct ent_oscillator *osc;
        if (synth == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        osc = ent_synth_get_oscillator(synth, osc_index);
        if (osc == NULL) {
                ent_log_error("can't get oscilaltor");
                ent_synth_unlock(synth);
                return ENTROPICTRON_ERROR;
        } else {
                osc->initial_phase = phase;
        }

        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == ENTROPICTRON_OSC_STATE_ENABLED)
                synth->buffer_update = true;

	ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_get_osc_phase(struct ent_synth *synth,
                          size_t osc_index,
                          ent_real *phase)
{
        struct ent_oscillator *osc;
        if (synth == NULL || phase == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        osc = ent_synth_get_oscillator(synth, osc_index);
        if (osc == NULL) {
                ent_log_error("can't get oscilaltor");
                ent_synth_unlock(synth);
                return ENTROPICTRON_ERROR;
        } else {
                *phase = osc->initial_phase;
        }

        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_set_osc_seed(struct ent_synth *synth,
                         size_t osc_index,
                         unsigned int seed)
{
        struct ent_oscillator *osc;
        if (synth == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        osc = ent_synth_get_oscillator(synth, osc_index);
        if (osc == NULL) {
                ent_log_error("can't get oscilaltor");
                ent_synth_unlock(synth);
                return ENTROPICTRON_ERROR;
        } else {
                osc->seed = seed;
        }

        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == ENTROPICTRON_OSC_STATE_ENABLED)
                synth->buffer_update = true;

	ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_get_osc_seed(struct ent_synth *synth,
                         size_t osc_index,
                         unsigned *seed)
{
        struct ent_oscillator *osc;
        if (synth == NULL || seed == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        osc = ent_synth_get_oscillator(synth, osc_index);
        if (osc == NULL) {
                ent_log_error("can't get oscilaltor");
                ent_synth_unlock(synth);
                return ENTROPICTRON_ERROR;
        } else {
                *seed = osc->seed;
        }

        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}


enum entropictron_error
ent_synth_get_length(struct ent_synth *synth,
                       ent_real *len)
{
        if (synth == NULL || len == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        *len = synth->length;
        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_set_length(struct ent_synth *synth,
                       ent_real len)
{
        if (synth == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        synth->length = len;
        ent_buffer_set_size(synth->buffer, synth->length * synth->sample_rate);
        synth->buffer_update = true;
        ent_synth_unlock(synth);

        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_kick_set_amplitude(struct ent_synth *synth,
                               ent_real amplitude)
{
        if (synth == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        synth->amplitude = amplitude;
        synth->buffer_update = true;
        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_kick_get_amplitude(struct ent_synth *synth,
                               ent_real *amplitude)
{
        if (synth == NULL || amplitude == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        *amplitude = synth->amplitude;
        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

enum entropictron_error
entropictron_synth_kick_filter_enable(struct ent_synth *synth,
                                  int enable)
{
        if (synth == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        synth->filter_enabled = enable;
        synth->buffer_update = true;
        ent_synth_unlock(synth);

        return ENTROPICTRON_OK;
}

enum entropictron_error
entropictron_synth_kick_filter_is_enabled(struct ent_synth *synth,
                                      int *enabled)
{
        if (synth == NULL || enabled == NULL) {
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        *enabled = synth->filter_enabled;
        ent_synth_unlock(synth);

        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_kick_set_filter_frequency(struct ent_synth *synth,
                                      ent_real frequency)
{
        enum entropictron_error res;
        if (synth == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        res = ent_filter_set_cutoff_freq(synth->filter, frequency);
        if (synth->filter_enabled)
                synth->buffer_update = true;
        ent_synth_unlock(synth);
        return res;
}

enum entropictron_error
ent_synth_kick_get_filter_frequency(struct ent_synth *synth,
                                      ent_real *frequency)
{
        if (synth == NULL || frequency == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        return ent_filter_get_cutoff_freq(synth->filter, frequency);
}

enum entropictron_error
ent_synth_kick_set_filter_factor(struct ent_synth *synth,
                                   ent_real factor)
{
        enum entropictron_error res;
        if (synth == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        res = ent_filter_set_factor(synth->filter, factor);
        if (synth->filter_enabled)
                synth->buffer_update = true;
        ent_synth_unlock(synth);
        return res;
}

enum entropictron_error
ent_synth_kick_get_filter_factor(struct ent_synth *synth,
                                   ent_real *factor)
{
        if (synth == NULL || factor == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        return ent_filter_get_factor(synth->filter, factor);
}

enum entropictron_error
ent_synth_set_kick_filter_type(struct ent_synth *synth,
                                 enum ent_filter_type type)
{
        enum entropictron_error res;
        if (synth == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        res = ent_filter_set_type(synth->filter, type);
        if (synth->filter_enabled)
                synth->buffer_update = true;
        ent_synth_unlock(synth);
        return res;
}

enum entropictron_error
ent_synth_get_kick_filter_type(struct ent_synth *synth,
                                 enum ent_filter_type *type)
{
        if (synth == NULL || type == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        return ent_filter_get_type(synth->filter, type);
}

struct ent_envelope*
synth_get_kick_envelope(struct ent_synth *synth,
			enum entropictron_envelope_type env_type)
{
	switch(env_type) {
	case ENTROPICTRON_AMPLITUDE_ENVELOPE:
                return synth->envelope;
	case ENTROPICTRON_FILTER_CUTOFF_ENVELOPE:
                return synth->filter->cutoff_env;
	case ENTROPICTRON_FILTER_Q_ENVELOPE:
                return synth->filter->q_env;
	case ENTROPICTRON_DISTORTION_DRIVE_ENVELOPE:
		return synth->distortion->drive_env;
	case ENTROPICTRON_DISTORTION_VOLUME_ENVELOPE:
                return synth->distortion->volume_env;
	default:
		return NULL;
	}
}

enum entropictron_error
ent_synth_kick_envelope_get_points(struct ent_synth *synth,
                                     enum entropictron_envelope_type env_type,
                                     struct ent_envelope_point_info **buf,
                                     size_t *npoints)
{
        if (synth == NULL || buf == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        *npoints = 0;
        *buf = NULL;
        ent_synth_lock(synth);
	struct ent_envelope *env = synth_get_kick_envelope(synth, env_type);
	if (env != NULL)
		ent_envelope_get_points(env, buf, npoints);
        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_kick_envelope_set_points(struct ent_synth *synth,
                                     enum entropictron_envelope_type env_type,
                                     const struct ent_envelope_point_info *buf,
                                     size_t npoints)
{
        if (synth == NULL || buf == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
	struct ent_envelope *env = synth_get_kick_envelope(synth, env_type);
	if (env != NULL)
		ent_envelope_set_points(env, buf, npoints);

	if (env_type == ENTROPICTRON_AMPLITUDE_ENVELOPE
            || ((env_type == ENTROPICTRON_FILTER_Q_ENVELOPE
		 || env_type == ENTROPICTRON_FILTER_CUTOFF_ENVELOPE)
                && synth->filter_enabled)
	    || ((env_type == ENTROPICTRON_DISTORTION_DRIVE_ENVELOPE
                 || env_type == ENTROPICTRON_DISTORTION_VOLUME_ENVELOPE)
                && synth->distortion->enabled)) {
                synth->buffer_update = true;
        }
        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_kick_add_env_point(struct ent_synth *synth,
                               enum entropictron_envelope_type env_type,
                               const struct ent_envelope_point_info *point_info)
{
        if (synth == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
	struct ent_envelope *env = synth_get_kick_envelope(synth, env_type);
	if (env != NULL)
		ent_envelope_add_point(env, point_info);

        if (env_type == ENTROPICTRON_AMPLITUDE_ENVELOPE
            || ((env_type == ENTROPICTRON_FILTER_CUTOFF_ENVELOPE
		 || env_type == ENTROPICTRON_FILTER_Q_ENVELOPE)
                && synth->filter_enabled)
	    || ((env_type == ENTROPICTRON_DISTORTION_DRIVE_ENVELOPE
                 || env_type == ENTROPICTRON_DISTORTION_VOLUME_ENVELOPE)
                && synth->distortion->enabled))
                synth->buffer_update = true;
        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_kick_remove_env_point(struct ent_synth *synth,
                                  enum entropictron_envelope_type env_type,
                                  size_t index)
{
        if (synth == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
	struct ent_envelope *env = synth_get_kick_envelope(synth, env_type);
	if (env != NULL)
		ent_envelope_remove_point(env, index);

        if (env_type == ENTROPICTRON_AMPLITUDE_ENVELOPE
            || ((env_type == ENTROPICTRON_FILTER_CUTOFF_ENVELOPE
		|| env_type == ENTROPICTRON_FILTER_Q_ENVELOPE)
                && synth->filter_enabled)
	    || ((env_type == ENTROPICTRON_DISTORTION_DRIVE_ENVELOPE
                 || env_type == ENTROPICTRON_DISTORTION_VOLUME_ENVELOPE)
                && synth->distortion->enabled)) {
                synth->buffer_update = true;
        }
        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_kick_update_env_point(struct ent_synth *synth,
                                  enum entropictron_envelope_type env_type,
                                  size_t index,
                                  const struct ent_envelope_point_info *point_info)
{
        if (synth == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
	struct ent_envelope *env = synth_get_kick_envelope(synth, env_type);
	if (env != NULL)
		ent_envelope_update_point(env, index, point_info);

        if (env_type == ENTROPICTRON_AMPLITUDE_ENVELOPE
            || ((env_type == ENTROPICTRON_FILTER_CUTOFF_ENVELOPE
		 || env_type == ENTROPICTRON_FILTER_Q_ENVELOPE)
                && synth->filter_enabled)
	    || ((env_type == ENTROPICTRON_DISTORTION_DRIVE_ENVELOPE
                 || env_type == ENTROPICTRON_DISTORTION_VOLUME_ENVELOPE)
                && synth->distortion->enabled)) {
                synth->buffer_update = true;
	}
        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

enum entropictron_error
synth_kick_env_set_apply_type(struct ent_synth *synth,
			      enum entropictron_envelope_type env_type,
			      enum ent_envelope_apply_type apply_type)
{
        ent_synth_lock(synth);
        if (env_type == ENTROPICTRON_FILTER_CUTOFF_ENVELOPE) {
		ent_envelope_set_apply_type(synth->filter->cutoff_env, apply_type);
	}

        if (env_type == ENTROPICTRON_AMPLITUDE_ENVELOPE
            || (env_type == ENTROPICTRON_FILTER_CUTOFF_ENVELOPE
                && synth->filter_enabled)
	    || ((env_type == ENTROPICTRON_DISTORTION_DRIVE_ENVELOPE
                 || env_type == ENTROPICTRON_DISTORTION_VOLUME_ENVELOPE)
                && synth->distortion->enabled)) {
                synth->buffer_update = true;
        }
        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

enum entropictron_error
synth_kick_env_get_apply_type(struct ent_synth *synth,
			      enum entropictron_envelope_type env_type,
			      enum ent_envelope_apply_type *apply_type)
{
	ent_synth_lock(synth);
        switch (env_type) {
        case ENTROPICTRON_FILTER_CUTOFF_ENVELOPE:
		*apply_type = ent_envelope_get_apply_type(synth->filter->cutoff_env);
                break;
        default:
                *apply_type = ENTROPICTRON_ENVELOPE_APPLY_LINEAR;
                break;
        }
        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_set_osc_frequency(struct ent_synth *synth,
                             size_t osc_index,
                             ent_real v)
{
        struct ent_oscillator* osc;

	if (synth == NULL) {
                ent_log_error("wrong arguments");
		return ENTROPICTRON_ERROR;
	}

	ent_synth_lock(synth);
	osc = ent_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		ent_log_error("can't get oscillator");
		ent_synth_unlock(synth);
		return ENTROPICTRON_ERROR;
	}
	osc->frequency = v;
        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == ENTROPICTRON_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
        }

	ent_synth_unlock(synth);
	return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_get_osc_frequency(struct ent_synth *synth,
                             size_t osc_index,
                             ent_real *v)
{
	if (synth == NULL || v == NULL) {
                ent_log_error("wrong arguments");
		return ENTROPICTRON_ERROR;
	}

	ent_synth_lock(synth);
	struct ent_oscillator* osc = ent_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		ent_log_error("can't get oscillator");
		ent_synth_unlock(synth);
		return ENTROPICTRON_ERROR;
	}
	*v = osc->frequency;
	ent_synth_unlock(synth);
	return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_set_osc_pitch_shift(struct ent_synth *synth,
                                size_t osc_index,
                                ent_real semitones)
{
        struct ent_oscillator* osc;

	if (synth == NULL) {
                ent_log_error("wrong arguments");
		return ENTROPICTRON_ERROR;
	}

	ent_synth_lock(synth);
	osc = ent_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		ent_log_error("can't get oscillator");
		ent_synth_unlock(synth);
		return ENTROPICTRON_ERROR;
	}
	osc->pitch_shift = semitones;
        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == ENTROPICTRON_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
        }

	ent_synth_unlock(synth);
	return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_get_osc_pitch_shift(struct ent_synth *synth,
                                size_t osc_index,
                                ent_real *semitones)
{
	if (synth == NULL || semitones == NULL) {
                ent_log_error("wrong arguments");
		return ENTROPICTRON_ERROR;
	}

	ent_synth_lock(synth);
	struct ent_oscillator* osc = ent_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		ent_log_error("can't get oscillator");
		ent_synth_unlock(synth);
		return ENTROPICTRON_ERROR;
	}
	*semitones = osc->pitch_shift;
	ent_synth_unlock(synth);
	return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_set_osc_noise_density(struct ent_synth *synth,
                                size_t osc_index,
                                ent_real density)
{
        struct ent_oscillator* osc;

	if (synth == NULL) {
                ent_log_error("wrong arguments");
		return ENTROPICTRON_ERROR;
	}

	ent_synth_lock(synth);
	osc = ent_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		ent_log_error("can't get oscillator");
		ent_synth_unlock(synth);
		return ENTROPICTRON_ERROR;
	}
	osc->noise_density = density;
        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == ENTROPICTRON_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
        }

	ent_synth_unlock(synth);
	return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_get_osc_noise_density(struct ent_synth *synth,
                                size_t osc_index,
                                ent_real *density)
{
	if (synth == NULL || density == NULL) {
                ent_log_error("wrong arguments");
		return ENTROPICTRON_ERROR;
	}

	ent_synth_lock(synth);
	struct ent_oscillator* osc = ent_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		ent_log_error("can't get oscillator");
		ent_synth_unlock(synth);
		return ENTROPICTRON_ERROR;
	}
	*density = osc->noise_density;
	ent_synth_unlock(synth);
	return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_set_osc_amplitude(struct ent_synth *synth,
                              size_t osc_index,
                              ent_real v)
{
	if (synth == NULL) {
                ent_log_error("wrong arguments");
		return ENTROPICTRON_ERROR;
	}

	ent_synth_lock(synth);
        struct ent_oscillator* osc = ent_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		ent_log_error("can't get oscillator");
		ent_synth_unlock(synth);
		return ENTROPICTRON_ERROR;
	}
	osc->amplitude = v;

        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == ENTROPICTRON_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
        }

	ent_synth_unlock(synth);
	return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_get_osc_amplitude(struct ent_synth *synth,
                              size_t osc_index,
                              ent_real *v)
{
        struct ent_oscillator* osc;

	if (synth == NULL || v == NULL) {
                ent_log_error("wrong arguments");
		return ENTROPICTRON_ERROR;
	}

	ent_synth_lock(synth);
	osc = ent_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		ent_log_error("can't get oscillator");
		ent_synth_unlock(synth);
		return ENTROPICTRON_ERROR;
	}
	*v = osc->amplitude;
	ent_synth_unlock(synth);

	return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_get_buffer_size(struct ent_synth *synth,
                            size_t *size)
{
        if (synth == NULL || size == NULL) {
                ent_log_error("wrong arguments");
		return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        *size = ent_buffer_size(synth->buffer);
        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_get_buffer(struct ent_synth *synth,
                       ent_real *buffer,
                       size_t size)
{
	if (synth == NULL || buffer == NULL) {
                ent_log_error("wrong arguments");
		return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        size = min(ent_buffer_size(synth->buffer), size);
        memcpy(buffer, synth->buffer->buff, size * sizeof(ent_real));
        ent_synth_unlock(synth);

        return ENTROPICTRON_ERROR;
}

void
ent_synth_set_output(struct ent_synth *synth,
                       struct ent_audio_output *output)
{
        if (synth == NULL || output == NULL) {
                ent_log_error("wrong argument");
                return;
        }
        synth->output = output;
}

enum entropictron_error
ent_synth_process(struct ent_synth *synth)
{
	if (synth == NULL)
		return ENTROPICTRON_ERROR;

	ent_synth_lock(synth);
	synth->buffer_update = false;
        ent_buffer_set_size(synth->buffer, synth->length * synth->sample_rate);
	ent_real dt = synth->length / ent_buffer_size(synth->buffer);
	ent_synth_reset_oscillators(synth);
	ent_filter_init(synth->filter);
	ent_synth_unlock(synth);

	/* Synthesize the percussion into the synthesizer buffer. */
	size_t i = 0;
        size_t tries = 0;
	while (1) {
                /**
                 * Try lock in order not too block the access
                 * of the synthesizer parameters for too long time.
                 */
                if (pthread_mutex_trylock(&synth->lock) != 0) {
                        entropictron_usleep(50);
                        /**
                         * Check how many tries for locking to avoind infinite loop.
                         * It should be maximum around 30ms.
                         */
                        if (++tries > 600)
                                break;
                        else
                                continue;
                }

		if (ent_buffer_is_end((struct ent_buffer*)synth->buffer)) {
			ent_synth_unlock(synth);
			break;
		} else {
			ent_real val = ent_synth_get_value(synth, (ent_real)(i * dt));
			if (isnan(val))
				val = 0.0f;
			else if (val > 1.0f)
				val = 1.0f;
			else if (val < -1.0f)
				val = -1.0f;
			ent_buffer_push_back(synth->buffer, val);
			i++;
		}
                ent_synth_unlock(synth);
	}

	ent_synth_lock(synth);
        if (synth->buffer_callback != NULL && synth->callback_args != NULL) {
                ent_log_debug("synth->buffer_callback");
                synth->buffer_callback(synth->callback_args,
                                       synth->buffer->buff,
                                       ent_buffer_size(synth->buffer),
                                       synth->id);
        }

        /**
         * Don't update the output audio buffer if
         * the synthesizer parameters
         * were updated during the synthesis.
         */
        if (!synth->buffer_update) {
                ent_audio_output_lock(synth->output);
                struct ent_buffer *buff = synth->output->updated_buffer;
                synth->output->updated_buffer = synth->buffer;
                synth->buffer = buff;
                ent_audio_output_unlock(synth->output);
        }
	ent_synth_unlock(synth);

	return ENTROPICTRON_OK;
}

ent_real
ent_synth_get_value(struct ent_synth *synth,
                      ent_real t)
{
        ent_real val = 0.0f;
        ent_real env_x = 0.0f;
        ent_real fm_val = 0.0f;
        size_t n = synth->oscillators_number;
        for (size_t i = 0; i < n; i++) {
                if (synth->osc_groups[i / GKICK_OSC_GROUP_SIZE]
                    && ent_osc_enabled(synth->oscillators[i])) {
                        if (synth->oscillators[i]->is_fm
                            && i % 3 == 0 && i + 1 < n) {
                                fm_val = ent_osc_value(synth->oscillators[i],
                                                         t,
                                                         synth->length);
                                synth->oscillators[i + 1]->fm_input = fm_val;
                        } else {
                                ent_real group_ampl
                                        = synth->osc_groups_amplitude[i / GKICK_OSC_GROUP_SIZE];
                                val += group_ampl * ent_osc_value(synth->oscillators[i],
                                                                    t,
                                                                    synth->length);
                        }
                        ent_osc_increment_phase(synth->oscillators[i], t, synth->length);
                }
        }

        env_x = t / synth->length;
        val *= synth->amplitude * ent_envelope_get_value(synth->envelope, env_x);
        if (synth->filter_enabled)
                ent_filter_val(synth->filter, val, &val, env_x);

        bool enabled = false;
        ent_distortion_is_enabled(synth->distortion, &enabled);
        if (enabled)
                ent_distortion_val(synth->distortion, val, &val, env_x);

        return val;
}

void
ent_synth_reset_oscillators(struct ent_synth *synth)
{
        if (synth == NULL)
                return;

        for (size_t i = 0; i < synth->oscillators_number; i++) {
                struct ent_oscillator *osc = synth->oscillators[i];
                osc->phase = osc->initial_phase;
                osc->fm_input = 0.0f;
                osc->seedp = osc->seed;
                ent_filter_init(osc->filter);
                if (osc->sample != NULL)
                        ent_buffer_reset(osc->sample);
        }
}

int
ent_synth_is_update_buffer(struct ent_synth *synth)
{
        return synth->buffer_update;
}

enum entropictron_error
ent_synth_set_osc_filter_type(struct ent_synth *synth,
                             size_t osc_index,
                             enum ent_filter_type type)
{
        struct ent_oscillator *osc;
        enum entropictron_error res;

        if (synth == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        osc = ent_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		ent_log_error("can't get oscillator");
		ent_synth_unlock(synth);
		return ENTROPICTRON_ERROR;
	}

        res = ent_filter_set_type(osc->filter, type);
        if (osc->filter_enabled
            && synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == ENTROPICTRON_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
        }

        ent_synth_unlock(synth);

        return res;
}

enum entropictron_error
ent_synth_get_osc_filter_type(struct ent_synth *synth,
                                size_t osc_index,
                                enum ent_filter_type *type)
{
        if (synth == NULL || type == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        struct ent_oscillator *osc;
        osc = ent_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		ent_log_error("can't get oscillator");
		ent_synth_unlock(synth);
		return ENTROPICTRON_ERROR;
	}

        enum entropictron_error res;
        res = ent_filter_get_type(osc->filter, type);
        ent_synth_unlock(synth);

        return res;
}

enum entropictron_error
ent_synth_set_osc_filter_cutoff(struct ent_synth *synth,
                                  size_t osc_index,
                                  ent_real cutoff)
{
        if (synth == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        struct ent_oscillator *osc;
        osc = ent_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		ent_log_error("can't get oscillator");
		ent_synth_unlock(synth);
		return ENTROPICTRON_ERROR;
	}
        enum entropictron_error res;
        res = ent_filter_set_cutoff_freq(osc->filter, cutoff);
        if (osc->filter_enabled
            && synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == ENTROPICTRON_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
        }

        ent_synth_unlock(synth);
        return res;
}

enum entropictron_error
ent_synth_get_osc_filter_cutoff(struct ent_synth *synth,
                                  size_t osc_index,
                                  ent_real *cutoff)
{
        if (synth == NULL || cutoff == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        struct ent_oscillator *osc;
        osc = ent_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		ent_log_error("can't get oscillator");
		ent_synth_unlock(synth);
		return ENTROPICTRON_ERROR;
	}

        enum entropictron_error res;
        res = ent_filter_get_cutoff_freq(osc->filter, cutoff);
        ent_synth_unlock(synth);
        return res;
}

enum entropictron_error
ent_synth_set_osc_filter_factor(struct ent_synth *synth,
                               size_t osc_index,
                               ent_real factor)
{
        if (synth == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        struct ent_oscillator *osc;
        osc = ent_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		ent_log_error("can't get oscillator");
		ent_synth_unlock(synth);
		return ENTROPICTRON_ERROR;
	}

        enum entropictron_error res;
        res = ent_filter_set_factor(osc->filter, factor);
        if (osc->filter_enabled
            && synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == ENTROPICTRON_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
        }
        ent_synth_unlock(synth);
        return res;
}

enum entropictron_error
ent_synth_get_osc_filter_factor(struct ent_synth *synth,
                               size_t osc_index,
                               ent_real *factor)
{
        if (synth == NULL || factor == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        struct ent_oscillator *osc;
        osc = ent_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		ent_log_error("can't get oscillator");
		ent_synth_unlock(synth);
		return ENTROPICTRON_ERROR;
	}

        enum entropictron_error res;
        res =  ent_filter_get_factor(osc->filter, factor);
        ent_synth_unlock(synth);
        return res;
}


enum entropictron_error
ent_synth_osc_enable_filter(struct ent_synth *synth,
                              size_t osc_index,
                              int enable)
{
        if (synth == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        struct ent_oscillator *osc;
        osc = ent_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		ent_log_error("can't get oscillator");
		ent_synth_unlock(synth);
		return ENTROPICTRON_ERROR;
	}

        osc->filter_enabled = enable;
        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == ENTROPICTRON_OSC_STATE_ENABLED) {
                synth->buffer_update = true;
        }

        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_osc_is_enabled_filter(struct ent_synth *synth,
                                  size_t osc_index,
                                  int *enabled)
{
        if (synth == NULL || enabled == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_synth_lock(synth);
        struct ent_oscillator *osc;
        osc = ent_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		ent_log_error("can't get oscillator");
		ent_synth_unlock(synth);
		return ENTROPICTRON_ERROR;
	}
        *enabled = osc->filter_enabled;
        ent_synth_unlock(synth);

        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_distortion_enable(struct ent_synth *synth,
                              bool enable)
{
	synth->buffer_update = true;
        return ent_distortion_enable(synth->distortion,
                                       enable);
}

enum entropictron_error
ent_synth_distortion_is_enabled(struct ent_synth *synth,
                                  bool *enabled)
{
        return ent_distortion_is_enabled(synth->distortion,
                                           enabled);
}

enum entropictron_error
ent_synth_distortion_set_type(struct ent_synth *synth,
                                enum ent_distortion_type type)
{
        enum entropictron_error res;
        ent_distortion_set_type(synth->distortion, type);
	bool enabled = false;
        res = ent_distortion_is_enabled(synth->distortion,
                                               &enabled);
        if (res == ENTROPICTRON_OK && enabled)
                synth->buffer_update = true;
        return res;
}

enum entropictron_error
ent_synth_distortion_get_type(struct ent_synth *synth,
                                enum ent_distortion_type *type)
{
        return ent_distortion_get_type(synth->distortion,
                                         type);
}

enum entropictron_error
ent_synth_distortion_set_in_limiter(struct ent_synth *synth,
                                      ent_real value)
{
        enum entropictron_error res;
        ent_distortion_set_in_limiter(synth->distortion,
                                        value);
	bool enabled = false;
        res = ent_distortion_is_enabled(synth->distortion,
                                          &enabled);
        if (res == ENTROPICTRON_OK && enabled)
                synth->buffer_update = true;
        return res;
}

enum entropictron_error
ent_synth_distortion_get_in_limiter(struct ent_synth *synth,
                                      ent_real *value)
{
	return ent_distortion_get_in_limiter(synth->distortion,
                                               value);
}

enum entropictron_error
ent_synth_distortion_set_out_limiter(struct ent_synth *synth, ent_real value)
{
        enum entropictron_error res;
        bool enabled = false;
        res = ent_distortion_set_out_limiter(synth->distortion, value);
        ent_distortion_is_enabled(synth->distortion, &enabled);
        if (res == ENTROPICTRON_OK && enabled)
                synth->buffer_update = true;
        return res;
}

enum entropictron_error
ent_synth_distortion_get_out_limiter(struct ent_synth *synth,
                                       ent_real *value)
{
        return ent_distortion_get_out_limiter(synth->distortion,
                                                value);
}

enum entropictron_error
ent_synth_distortion_set_drive(struct ent_synth *synth,
                                 ent_real drive)
{
        enum entropictron_error res;
        bool enabled = false;
        res = ent_distortion_set_drive(synth->distortion, drive);
        ent_distortion_is_enabled(synth->distortion, &enabled);
        if (res == ENTROPICTRON_OK && enabled)
                synth->buffer_update = true;
        return res;
}

enum entropictron_error
ent_synth_distortion_get_drive(struct ent_synth *synth,
                                 ent_real *drive)
{
        return ent_distortion_get_drive(synth->distortion, drive);
}

enum entropictron_error
ent_synth_enable_group(struct ent_synth *synth,
                         size_t index,
                         bool enable)
{
        ent_synth_lock(synth);
        synth->osc_groups[index] = enable;
        synth->buffer_update = true;
        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_synth_group_enabled(struct ent_synth *synth,
                          size_t index,
                          bool *enabled)
{
        ent_synth_lock(synth);
        *enabled = synth->osc_groups[index];
        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

enum entropictron_error
entropictron_synth_group_set_amplitude(struct ent_synth *synth,
                                   size_t index,
                                   ent_real amplitude)
{
        ent_synth_lock(synth);
        synth->osc_groups_amplitude[index] = amplitude;
        synth->buffer_update = true;
        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

enum entropictron_error
entropictron_synth_group_get_amplitude(struct ent_synth *synth,
                                   size_t index,
                                   ent_real *amplitude)
{
        ent_synth_lock(synth);
        *amplitude = synth->osc_groups_amplitude[index];
        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

enum entropictron_error
entropictron_synth_set_osc_sample(struct ent_synth *synth,
                              size_t osc_index,
                              const ent_real *data,
                              size_t size)
{
        ent_synth_lock(synth);
        struct ent_oscillator *osc;
        osc = ent_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		ent_log_error("can't get oscillator");
		ent_synth_unlock(synth);
		return ENTROPICTRON_ERROR;
	}

        if (osc->sample == NULL)
                ent_buffer_new(&osc->sample, osc->sample_rate * ENTROPICTRON_MAX_LENGTH);
        ent_buffer_set_data(osc->sample, data, size);
        ent_buffer_reset(osc->sample);
        if (synth->osc_groups[osc_index / GKICK_OSC_GROUP_SIZE]
            && osc->state == ENTROPICTRON_OSC_STATE_ENABLED)
                synth->buffer_update = true;
        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

enum entropictron_error
entropictron_synth_get_osc_sample(struct ent_synth *synth,
                              size_t osc_index,
                              ent_real **data,
                              size_t *size)
{
        *data = NULL;
        *size = 0;
        ent_synth_lock(synth);
        struct ent_oscillator *osc;
        osc = ent_synth_get_oscillator(synth, osc_index);
	if (osc == NULL) {
		ent_log_error("can't get oscillator");
		ent_synth_unlock(synth);
		return ENTROPICTRON_ERROR;
	}

        if (osc->sample == NULL) {
                ent_synth_unlock(synth);
                return ENTROPICTRON_OK;
        }

        *size = ent_buffer_size(osc->sample);
        if (*size > 0) {
                *data = (ent_real*)malloc(sizeof(ent_real) * (*size));
                if (*data == NULL) {
                        ent_log_error("can't allocate memory");
                        *size = 0;
                        ent_synth_unlock(synth);
                        return ENTROPICTRON_ERROR;
                }
                memcpy(*data, osc->sample->buff, sizeof(ent_real) * (*size));
        }
        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}
