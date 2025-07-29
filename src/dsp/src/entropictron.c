/**
 * File name: entropictron.c
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

#include "entropictron_internal.h"
#include "synthesizer.h"
#include "ent_audio.h"
#include "audio_output.h"
#include "envelope.h"
#include "mixer.h"
#include "oscillator.h"

#include <unistd.h>
#include <stdlib.h>

enum entropictron_error
entropictron_create(struct entropictron **kick, int sample_rate)
{
	if (kick == NULL)
		return ENTROPICTRON_ERROR;

	*kick = (struct entropictron*)calloc(1, sizeof(struct entropictron));
	if (*kick == NULL)
		return ENTROPICTRON_ERROR_MEM_ALLOC;
	strcpy((*kick)->name, "Geonkick");
	(*kick)->sample_rate = sample_rate;
        (*kick)->synthesis_on = false;
        (*kick)->per_index = 0;

	if (pthread_mutex_init(&(*kick)->lock, NULL) != 0) {
                ent_log_error("error on init mutex");
                entropictron_free(kick);
                return ENTROPICTRON_ERROR;
	}

	if (ent_audio_create(&(*kick)->audio, sample_rate) != ENTROPICTRON_OK) {
                ent_log_warning("can't create audio");
		entropictron_free(kick);
		return ENTROPICTRON_ERROR;
	}

        (*kick)->sample_rate = (*kick)->audio->sample_rate;

        for (size_t i = 0; i < ENTROPICTRON_MAX_INSTRUMENTS; i++) {
                if (ent_synth_new(&(*kick)->synths[i], (*kick)->sample_rate) != ENTROPICTRON_OK) {
                        ent_log_error("can't create synthesizer %u", i);
                        entropictron_free(kick);
                        return ENTROPICTRON_ERROR;
                }
                (*kick)->synths[i]->id = i;
        }

        for (size_t i = 0; i < ENTROPICTRON_MAX_INSTRUMENTS; i++) {
                ent_synth_set_output((*kick)->synths[i], (*kick)->audio->audio_outputs[i]);
                entropictron_set_percussion_channel(*kick, i, i % ENTROPICTRON_MAX_CHANNELS);
        }

        if (!entropictron_worker_created()) {
                if (entropictron_worker_create() != ENTROPICTRON_OK) {
                        ent_log_error("can't init worker");
                        entropictron_free(kick);
                        return ENTROPICTRON_ERROR;
                }

                if (entropictron_worker_start() != ENTROPICTRON_OK) {
                        ent_log_error("can't start worker");
                        entropictron_free(kick);
                        return ENTROPICTRON_ERROR;
                }
        }
        entropictron_worker_add_instance(*kick);

        if (ent_start_audio((*kick)->audio) != ENTROPICTRON_OK) {
                ent_log_error("can't start audio module");
                entropictron_free(kick);
        }

	return ENTROPICTRON_OK;
}

void entropictron_free(struct entropictron **kick)
{
        if (kick != NULL && *kick != NULL) {
                entropictron_worker_remove_instance(*kick);
                if (entropictron_worker_reference_count() == 0)
                        entropictron_worker_destroy();
                ent_log_debug("ref count: %d", entropictron_worker_reference_count());
                for (size_t i = 0; i < ENTROPICTRON_MAX_INSTRUMENTS; i++)
                        ent_synth_free(&((*kick)->synths[i]));
                ent_audio_free(&((*kick)->audio));
		pthread_mutex_destroy(&(*kick)->lock);
                free(*kick);
        }
}

void entropictron_lock(struct entropictron *kick)
{
        pthread_mutex_lock(&kick->lock);
}

void entropictron_unlock(struct entropictron *kick)
{
        pthread_mutex_unlock(&kick->lock);
}

enum entropictron_error
entropictron_enable_oscillator(struct entropictron* kick, size_t index)
{
        if (kick == NULL)
                return ENTROPICTRON_ERROR;
        enum entropictron_error res;
        res = ent_synth_enable_oscillator(kick->synths[kick->per_index], index, 1);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_disable_oscillator(struct entropictron* kick, size_t index)
{
        if (kick == NULL)
                return ENTROPICTRON_ERROR;
        enum entropictron_error res;
        res = ent_synth_enable_oscillator(kick->synths[kick->per_index], index, 0);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_is_oscillator_enabled(struct entropictron* kick, size_t index, int *enabled)
{
        if (kick == NULL || enabled == NULL)
                return ENTROPICTRON_ERROR;
        return ent_synth_osc_is_enabled(kick->synths[kick->per_index], index, enabled);
}

enum entropictron_error
entropictron_get_oscillators_number(struct entropictron *kick, size_t *number)
{
        if (kick == NULL || number == NULL)
                return ENTROPICTRON_ERROR;
        return ent_synth_get_oscillators_number(kick->synths[kick->per_index], number);
}

enum entropictron_error
entropictron_osc_envelope_add_point(struct entropictron *kick,
				size_t osc_index,
				size_t env_index,
                                struct ent_envelope_point_info *point_info)
{
        if (kick == NULL || point_info == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        enum entropictron_error res;
        res = ent_synth_osc_env_add_point(kick->synths[kick->per_index],
                                            osc_index,
                                            env_index, point_info);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_set_sample_rate(struct entropictron *kick, ent_real rate)
{
        return ENTROPICTRON_OK;
}

enum entropictron_error
entropictron_osc_envelope_get_points(struct entropictron *kick,
				 size_t osc_index,
				 size_t env_index,
				 struct ent_envelope_point_info **buf,
				 size_t *npoints)
{
        if (kick == NULL || buf == NULL || npoints == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        return ent_synth_osc_envelope_points(kick->synths[kick->per_index],
                                               osc_index,
                                               env_index,
                                               buf,
                                               npoints);
}

enum entropictron_error
entropictron_osc_envelope_set_points(struct entropictron *kick,
                                 size_t osc_index,
                                 size_t env_index,
                                 const struct ent_envelope_point_info *buff,
                                 size_t npoints)
{
        if (kick == NULL || buff == NULL || npoints == 0) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        return ent_synth_osc_envelope_set_points(kick->synths[kick->per_index],
                                                   osc_index,
                                                   env_index,
                                                   buff,
                                                   npoints);
}


enum entropictron_error
entropictron_osc_envelope_remove_point(struct entropictron *kick,
				   size_t osc_index,
				   size_t env_index,
				   size_t index)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        enum entropictron_error res;
        res = ent_synth_osc_env_remove_point(kick->synths[kick->per_index],
                                               osc_index,
                                               env_index,
                                               index);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_osc_envelope_update_point(struct entropictron *kick,
				   size_t osc_index,
				   size_t env_index,
  				   size_t index,
                                   struct ent_envelope_point_info *point_info)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        enum entropictron_error res;
        res = ent_synth_osc_env_update_point(kick->synths[kick->per_index],
                                               osc_index,
                                               env_index,
                                               index,
                                               point_info);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);

        return res;
}

enum entropictron_error
entropictron_osc_envelope_set_apply_type(struct entropictron *kick,
				     size_t osc_index,
				     size_t env_index,
				     enum ent_envelope_apply_type apply_type)
{
	if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        enum entropictron_error res;
        res = synth_osc_env_set_apply_type(kick->synths[kick->per_index],
					   osc_index,
					   env_index,
					   apply_type);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_osc_envelope_get_apply_type(struct entropictron *kick,
				     size_t osc_index,
				     size_t env_index,
				     enum ent_envelope_apply_type *apply_type)
{
        if (kick == NULL || apply_type == NULL)
                return ENTROPICTRON_ERROR;
        return synth_osc_env_get_apply_type(kick->synths[kick->per_index],
					    osc_index,
					    env_index,
					    apply_type);
}

enum entropictron_error
entropictron_osc_set_fm(struct entropictron *kick, size_t index, bool is_fm)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        enum entropictron_error res;
        res = ent_synth_osc_set_fm(kick->synths[kick->per_index], index, is_fm);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_osc_is_fm(struct entropictron *kick, size_t index, bool *is_fm)
{
        if (kick == NULL || is_fm == NULL)
                return ENTROPICTRON_ERROR;
        return ent_synth_osc_is_fm(kick->synths[kick->per_index], index, is_fm);
}

enum entropictron_error
entropictron_osc_set_fm_k(struct entropictron *kick, size_t index, ent_real k)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        struct ent_synth *synth = kick->synths[kick->per_index];
        ent_synth_lock(synth);
        struct ent_oscillator* osc;
        osc = ent_synth_get_oscillator(synth, index);
	if (osc == NULL) {
		ent_log_error("can't get oscillator");
		ent_synth_unlock(synth);
		return ENTROPICTRON_ERROR;
	}

        osc->fm_k = k;
	ent_synth_unlock(synth);

        return ENTROPICTRON_OK;
}

enum entropictron_error
entropictron_set_osc_function(struct entropictron *kick,
			  size_t osc_index,
			  enum entropictron_osc_func_type type)
{
        if (kick == NULL)
                return ENTROPICTRON_ERROR;
        enum entropictron_error res;
        res = ent_synth_set_osc_function(kick->synths[kick->per_index], osc_index, type);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_get_osc_function(struct entropictron *kick,
			  size_t osc_index,
			  enum entropictron_osc_func_type *type)
{
        if (kick == NULL || type == NULL)
                return ENTROPICTRON_ERROR;
        return ent_synth_get_osc_function(kick->synths[kick->per_index], osc_index, type);
}

enum entropictron_error
entropictron_set_osc_phase(struct entropictron *kick,
                       size_t osc_index,
                       ent_real phase)
{
        if (kick == NULL)
                return ENTROPICTRON_ERROR;

        enum entropictron_error res;
        res = ent_synth_set_osc_phase(kick->synths[kick->per_index], osc_index, phase);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;

}

enum entropictron_error
entropictron_get_osc_phase(struct entropictron *kick,
                       size_t osc_index,
                       ent_real *phase)
{
        if (kick == NULL || phase == NULL)
                return ENTROPICTRON_ERROR;

        return ent_synth_get_osc_phase(kick->synths[kick->per_index],
                                         osc_index,
                                         phase);
}

enum entropictron_error
entropictron_set_osc_seed(struct entropictron *kick,
                      size_t osc_index,
                      unsigned int seed)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }


        enum entropictron_error res;
        res = ent_synth_set_osc_seed(kick->synths[kick->per_index],
                                       osc_index,
                                       seed);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_get_osc_seed(struct entropictron *kick,
                      size_t osc_index,
                      unsigned int *seed)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        return ent_synth_get_osc_seed(kick->synths[kick->per_index],
                                        osc_index,
                                        seed);

}

enum entropictron_error
entropictron_set_length(struct entropictron *kick, ent_real len)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        enum entropictron_error res;
        res = ent_synth_set_length(kick->synths[kick->per_index], len);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_get_length(struct entropictron *kick,
                    ent_real *len)
{
        if (kick == NULL || len == NULL)
                return ENTROPICTRON_ERROR;
        return ent_synth_get_length(kick->synths[kick->per_index], len);
}

enum entropictron_error
entropictron_get_max_length(struct entropictron *kick,
                        ent_real *len)
{
        ENTROPICTRON_UNUSED(kick);
        if (kick == NULL || len == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        *len = ENTROPICTRON_MAX_LENGTH;
        return ENTROPICTRON_OK;
}

enum entropictron_error
entropictron_kick_set_amplitude(struct entropictron *kick,
                            ent_real amplitude)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        enum entropictron_error res;
        res = ent_synth_kick_set_amplitude(kick->synths[kick->per_index], amplitude);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_kick_get_amplitude(struct entropictron *kick,
                            ent_real *amplitude)
{
        if (kick == NULL || amplitude == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_synth_kick_get_amplitude(kick->synths[kick->per_index], amplitude);
}

enum entropictron_error
entropictron_kick_set_filter_frequency(struct entropictron *kick,
                                   ent_real frequency)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        enum entropictron_error res;
        res = ent_synth_kick_set_filter_frequency(kick->synths[kick->per_index], frequency);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_kick_filter_enable(struct entropictron *kick,
                            int enable)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        enum entropictron_error res;
        res = entropictron_synth_kick_filter_enable(kick->synths[kick->per_index], enable);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_kick_filter_is_enabled(struct entropictron *kick,
                                int *enabled)
{
        if (kick == NULL || enabled == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return entropictron_synth_kick_filter_is_enabled(kick->synths[kick->per_index],
                                                     enabled);
}

enum entropictron_error
entropictron_kick_get_filter_frequency(struct entropictron *kick,
                                   ent_real *frequency)
{
        if (kick == NULL || frequency == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_synth_kick_get_filter_frequency(kick->synths[kick->per_index],
                                                     frequency);
}

enum entropictron_error
entropictron_kick_set_filter_factor(struct entropictron *kick,
                                ent_real factor)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        enum entropictron_error res;
        res = ent_synth_kick_set_filter_factor(kick->synths[kick->per_index], factor);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_kick_get_filter_factor(struct entropictron *kick,
                                ent_real *factor)
{
        if (kick == NULL || factor == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_synth_kick_get_filter_factor(kick->synths[kick->per_index], factor);
}

enum entropictron_error
entropictron_set_kick_filter_type(struct entropictron *kick,
                              enum ent_filter_type type)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        enum entropictron_error res;
        res = ent_synth_set_kick_filter_type(kick->synths[kick->per_index], type);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_get_kick_filter_type(struct entropictron *kick,
                              enum ent_filter_type *type)
{
        if (kick == NULL || type == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_synth_get_kick_filter_type(kick->synths[kick->per_index], type);
}

enum entropictron_error
entropictron_kick_envelope_get_points(struct entropictron *kick,
                                  enum entropictron_envelope_type env_type,
                                  struct ent_envelope_point_info **buf,
                                  size_t *npoints)
{
        if (kick == NULL || buf == NULL || npoints == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_synth_kick_envelope_get_points(kick->synths[kick->per_index],
                                                    env_type,
                                                    buf,
                                                    npoints);
}

enum entropictron_error
entropictron_kick_envelope_set_points(struct entropictron *kick,
                                  enum entropictron_envelope_type env_type,
                                  const struct ent_envelope_point_info *buff,
                                  size_t npoints)
{
        if (kick == NULL || buff == NULL || npoints == 0)
                return ENTROPICTRON_ERROR;
        enum entropictron_error res;
        res = ent_synth_kick_envelope_set_points(kick->synths[kick->per_index],
                                                   env_type,
                                                   buff,
                                                   npoints);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;

}

enum entropictron_error
entropictron_kick_add_env_point(struct entropictron *kick,
                            enum entropictron_envelope_type env_type,
                            const struct ent_envelope_point_info *point_info)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        enum entropictron_error res;
        res = ent_synth_kick_add_env_point(kick->synths[kick->per_index],
                                             env_type,
                                             point_info);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_kick_remove_env_point(struct entropictron *kick,
                               enum entropictron_envelope_type env_type,
                               size_t index)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        enum entropictron_error res;
        res = ent_synth_kick_remove_env_point(kick->synths[kick->per_index],
                                                env_type,
                                                index);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_kick_update_env_point(struct entropictron *kick,
                               enum entropictron_envelope_type env_type,
                               size_t index,
                               const struct ent_envelope_point_info *point_info)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        enum entropictron_error res;
        res = ent_synth_kick_update_env_point(kick->synths[kick->per_index],
                                                env_type,
                                                index, point_info);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_kick_env_set_apply_type(struct entropictron *kick,
				 enum entropictron_envelope_type env_type,
				 enum ent_envelope_apply_type apply_type)
{
	if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        enum entropictron_error res;
        res = synth_kick_env_set_apply_type(kick->synths[kick->per_index],
					    env_type,
                                            apply_type);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_kick_env_get_apply_type(struct entropictron *kick,
				 enum entropictron_envelope_type env_type,
				 enum ent_envelope_apply_type *apply_type)
{
	if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        return synth_kick_env_get_apply_type(kick->synths[kick->per_index],
					     env_type,
					     apply_type);
}

enum entropictron_error
entropictron_set_osc_frequency(struct entropictron *kick,
                           size_t osc_index,
                           ent_real v)
{
	if (kick == NULL) {
		ent_log_error("wrong arguments");
		return ENTROPICTRON_ERROR;
	}

        enum entropictron_error res;
        res = ent_synth_set_osc_frequency(kick->synths[kick->per_index],
                                            osc_index,
                                            v);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_set_osc_pitch_shift(struct entropictron *kick,
                             size_t osc_index,
                             ent_real semitones)
{
	if (kick == NULL) {
		ent_log_error("wrong arguments");
		return ENTROPICTRON_ERROR;
	}

        enum entropictron_error res;
        res = ent_synth_set_osc_pitch_shift(kick->synths[kick->per_index],
                                              osc_index,
                                              semitones);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_set_osc_noise_density(struct entropictron *kick,
                               size_t osc_index,
                               ent_real density)
{
	if (kick == NULL) {
		ent_log_error("wrong arguments");
		return ENTROPICTRON_ERROR;
	}

        enum entropictron_error res;
        res = ent_synth_set_osc_noise_density(kick->synths[kick->per_index],
                                                osc_index,
                                                density);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_set_osc_amplitude(struct entropictron *kick,
                           size_t osc_index,
                           ent_real v)
{
	if (kick == NULL) {
		ent_log_error("wrong arguments");
		return ENTROPICTRON_ERROR;
	}

        enum entropictron_error res;
        res = ent_synth_set_osc_amplitude(kick->synths[kick->per_index],
                                            osc_index,
                                            v);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_get_osc_amplitude(struct entropictron *kick,
                           size_t osc_index,
                           ent_real *v)
{
	if (kick == NULL || v == NULL)
		return ENTROPICTRON_ERROR;
        return ent_synth_get_osc_amplitude(kick->synths[kick->per_index],
                                             osc_index,
                                             v);
}

enum entropictron_error
entropictron_get_osc_frequency(struct entropictron *kick,
                           size_t osc_index,
                           ent_real *v)
{
	if (kick == NULL || v == NULL)
		return ENTROPICTRON_ERROR;
	return ent_synth_get_osc_frequency(kick->synths[kick->per_index],
                                             osc_index,
                                             v);
}

enum entropictron_error
entropictron_get_osc_pitch_shift(struct entropictron *kick,
                             size_t osc_index,
                             ent_real *semitones)
{
	if (kick == NULL || semitones == NULL)
		return ENTROPICTRON_ERROR;
	return ent_synth_get_osc_pitch_shift(kick->synths[kick->per_index],
                                               osc_index,
                                               semitones);
}

enum entropictron_error
entropictron_get_osc_noise_density(struct entropictron *kick,
                               size_t osc_index,
                               ent_real *density)
{
	if (kick == NULL || density == NULL)
		return ENTROPICTRON_ERROR;
	return ent_synth_get_osc_noise_density(kick->synths[kick->per_index],
                                                 osc_index,
                                                 density);
}

enum entropictron_error
entropictron_play(struct entropictron *kick, size_t id)
{
        if (kick == NULL || id >= ENTROPICTRON_MAX_INSTRUMENTS) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_audio_play(kick->audio, id);
}

enum entropictron_error
entropictron_key_pressed(struct entropictron *kick,
                     bool pressed,
                     int note,
                     int velocity)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_audio_key_pressed(kick->audio,
                                       pressed,
                                       note,
                                       velocity);
}

enum entropictron_error
entropictron_get_kick_buffer_size(struct entropictron *kick, size_t *size)
{
        enum entropictron_error res;
        if (kick  == NULL || size == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        res = ent_synth_get_buffer_size(kick->synths[kick->per_index],
                                          size);
        return res;
}

enum entropictron_error
entropictron_get_kick_buffer(struct entropictron *kick,
                         ent_real *buffer,
                         size_t size)
{
        enum entropictron_error res;
        if (kick  == NULL || buffer == NULL || size < 1) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        res = ent_synth_get_buffer(kick->synths[kick->per_index],
                                     buffer,
                                     size);
        return res;
}

enum entropictron_error
entropictron_set_kick_buffer_callback(struct entropictron *kick,
                                  void (*callback)(void*,
                                                   ent_real* buff,
                                                   size_t size,
                                                   size_t id),
                                  void *arg)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

	entropictron_lock(kick);
        for (size_t i = 0; i < ENTROPICTRON_MAX_INSTRUMENTS; i++) {
                kick->synths[i]->buffer_callback = callback;
                kick->synths[i]->callback_args = arg;
        }
	entropictron_unlock(kick);
	return ENTROPICTRON_OK;
}

enum entropictron_error
entropictron_set_kick_limiter_callback(struct entropictron *kick,
                                   void (*callback)(void*,
                                                    size_t index,
                                                    ent_real val),
                                   void *arg)
{
        if (kick  == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_audio_set_limiter_callback(kick->audio,
                                                callback,
                                                arg);
}

enum entropictron_error
entropictron_set_limiter_value(struct entropictron *kick,
                           ent_real limit)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_audio_set_limiter_val(kick->audio,
                                           kick->per_index,
                                           limit);
}

enum entropictron_error
entropictron_get_limiter_value(struct entropictron *kick,
                           ent_real *limit)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_audio_get_limiter_val(kick->audio,
                                           kick->per_index,
                                           limit);
}

enum entropictron_error
entropictron_set_osc_filter_type(struct entropictron *kick,
                             size_t osc_index,
                             enum ent_filter_type type)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        enum entropictron_error res;
        res = ent_synth_set_osc_filter_type(kick->synths[kick->per_index],
                                              osc_index,
                                              type);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_get_osc_filter_type(struct entropictron *kick,
                             size_t osc_index,
                             enum ent_filter_type *type)
{
        if (kick == NULL || type == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_synth_get_osc_filter_type(kick->synths[kick->per_index],
                                               osc_index,
                                               type);
}

enum entropictron_error
entropictron_set_osc_filter_cutoff_freq(struct entropictron *kick,
                                    size_t osc_index,
                                    ent_real cutoff)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        enum entropictron_error res;
        res = ent_synth_set_osc_filter_cutoff(kick->synths[kick->per_index],
                                                osc_index,
                                                cutoff);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_get_osc_filter_cutoff_freq(struct entropictron *kick,
                                    size_t osc_index,
                                    ent_real *cutoff)
{
        if (kick == NULL || cutoff == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_synth_get_osc_filter_cutoff(kick->synths[kick->per_index],
                                                 osc_index,
                                                 cutoff);
}

enum entropictron_error
entropictron_set_osc_filter_factor(struct entropictron *kick,
                               size_t osc_index,
                               ent_real factor)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        enum entropictron_error res;
        res = ent_synth_set_osc_filter_factor(kick->synths[kick->per_index],
                                                osc_index,
                                                factor);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_get_osc_filter_factor(struct entropictron *kick,
                               size_t osc_index,
                               ent_real *factor)
{
        if (kick == NULL || factor == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_synth_get_osc_filter_factor(kick->synths[kick->per_index],
                                                 osc_index,
                                                 factor);
}


enum entropictron_error
entropictron_enbale_osc_filter(struct entropictron *kick,
                           size_t osc_index,
                           int enable)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        enum entropictron_error res;
        res = ent_synth_osc_enable_filter(kick->synths[kick->per_index],
                                            osc_index,
                                            enable);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_osc_filter_is_enabled(struct entropictron *kick,
                               size_t osc_index,
                               int *enable)
{
        if (kick == NULL || enable == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_synth_osc_is_enabled_filter(kick->synths[kick->per_index],
                                                 osc_index,
                                                 enable);
}

enum entropictron_error
entropictron_get_sample_rate(struct entropictron *kick,
                         int *sample_rate)
{
        ENTROPICTRON_UNUSED(kick);

        if (sample_rate == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        *sample_rate = kick->sample_rate;
        return ENTROPICTRON_OK;
}

enum entropictron_error
entropictron_enable_synthesis(struct entropictron *kick,
                          bool enable)
{
	if (kick == NULL) {
		ent_log_error("wrong arguments");
		return ENTROPICTRON_ERROR;
	}

	kick->synthesis_on = enable;
        if (kick->synthesis_on) {
                for (size_t i = 0; i < ENTROPICTRON_MAX_INSTRUMENTS; i++) {
                        if (kick->synths[i]->is_active)
                                kick->synths[i]->buffer_update = true;
                }
                entropictron_wakeup(kick);
        }
        return ENTROPICTRON_OK;
}

enum entropictron_error
entropictron_audio_process(struct entropictron *kick,
                       float **out,
                       size_t channel,
                       size_t size)
{
        return ent_audio_process(kick->audio,
                                   out,
                                   channel,
                                   size);
}

enum entropictron_error
entropictron_distortion_enable(struct entropictron *kick, bool enable)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        enum entropictron_error res;
        res = ent_synth_distortion_enable(kick->synths[kick->per_index],
                                            enable);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_distortion_is_enabled(struct entropictron *kick,
                               bool *enabled)
{
        if (kick == NULL || enabled == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_synth_distortion_is_enabled(kick->synths[kick->per_index],
                                                 enabled);
}

enum entropictron_error
entropictron_distortion_set_type(struct entropictron *kick,
                             enum ent_distortion_type type)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        enum entropictron_error res;
        res = ent_synth_distortion_set_type(kick->synths[kick->per_index],
                                              type);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_distortion_get_type(struct entropictron *kick,
                             enum ent_distortion_type *type)
{
        if (kick == NULL || type == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_synth_distortion_get_type(kick->synths[kick->per_index],
                                               type);
}

enum entropictron_error
entropictron_distortion_set_in_limiter(struct entropictron *kick,
                                   ent_real limit)
{
	if (kick == NULL) {
		ent_log_error("wrong arguments");
		return ENTROPICTRON_ERROR;
	}

	enum entropictron_error res;
	res = ent_synth_distortion_set_in_limiter(kick->synths[kick->per_index],
                                                    limit);
	if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
	return res;
}

enum entropictron_error
entropictron_distortion_get_in_limiter(struct entropictron *kick,
                                   ent_real *limit)
{
	if (kick == NULL || limit == NULL) {
		ent_log_error("wrong arguments");
		return ENTROPICTRON_ERROR;
	}
	return ent_synth_distortion_get_in_limiter(kick->synths[kick->per_index],
                                                     limit);
}

enum entropictron_error
entropictron_distortion_set_out_limiter(struct entropictron *kick,
                                    ent_real value)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        enum entropictron_error res;
        res = ent_synth_distortion_set_out_limiter(kick->synths[kick->per_index],
                                                     value);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_distortion_get_out_limiter(struct entropictron *kick, ent_real *value)
{
        if (kick == NULL || value == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_synth_distortion_get_out_limiter(kick->synths[kick->per_index],
                                                      value);
}

enum entropictron_error
entropictron_distortion_set_drive(struct entropictron *kick,
                              ent_real drive)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        enum entropictron_error res;
        res = ent_synth_distortion_set_drive(kick->synths[kick->per_index],
                                               drive);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_distortion_get_drive(struct entropictron *kick,
                              ent_real *drive)
{
        if (kick == NULL || drive == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_synth_distortion_get_drive(kick->synths[kick->per_index],
                                                drive);
}

enum entropictron_error
entropictron_osc_distortion_enable(struct entropictron *kick,
                               size_t index,
                               bool enable)
{
        struct ent_oscillator* osc;
        osc = ent_synth_get_oscillator(kick->synths[kick->per_index], index);
        if (!osc)
                return ENTROPICTRON_ERROR;
        enum entropictron_error res;
        res = ent_distortion_enable(osc->distortion, enable);
        if (res == ENTROPICTRON_OK) {
                kick->synths[kick->per_index]->buffer_update = true;
                entropictron_wakeup(kick);
        }
        return res;
}

enum entropictron_error
entropictron_osc_distortion_is_enabled(struct entropictron *kick,
                                   size_t index,
                                   bool *enabled)
{
        struct ent_oscillator* osc;
        osc = ent_synth_get_oscillator(kick->synths[kick->per_index], index);
        if (!osc)
                return ENTROPICTRON_ERROR;
        return ent_distortion_is_enabled(osc->distortion, enabled);
}

enum entropictron_error
entropictron_osc_distortion_get_type(struct entropictron *kick,
                                 size_t index,
                                 enum ent_distortion_type *type)
{
        struct ent_oscillator* osc;
        osc = ent_synth_get_oscillator(kick->synths[kick->per_index], index);
        if (!osc)
                return ENTROPICTRON_ERROR;
        return ent_distortion_get_type(osc->distortion, type);
}

enum entropictron_error
entropictron_osc_distortion_set_type(struct entropictron *kick,
                                 size_t index,
                                 enum ent_distortion_type type)
{
        struct ent_oscillator* osc;
        struct ent_synth *synth = kick->synths[kick->per_index];
        osc = ent_synth_get_oscillator(synth, index);
        if (!osc)
                return ENTROPICTRON_ERROR;
        enum entropictron_error res;
        res = ent_distortion_set_type(osc->distortion, type);
        if (res != ENTROPICTRON_OK)
                return res;
        bool enabled = false;
        ent_distortion_is_enabled(osc->distortion, &enabled);
        if (enabled) {
                synth->buffer_update = true;
                entropictron_wakeup(kick);
        }
        return res;
}

enum entropictron_error
entropictron_osc_distortion_set_in_limiter(struct entropictron *kick,
                                       size_t index,
                                       ent_real value)
{
        struct ent_oscillator* osc;
        struct ent_synth *synth = kick->synths[kick->per_index];
        osc = ent_synth_get_oscillator(synth, index);
        if (!osc)
                return ENTROPICTRON_ERROR;
        enum entropictron_error res;
        res = ent_distortion_set_in_limiter(osc->distortion, value);
        if (res != ENTROPICTRON_OK)
                return res;
        bool enabled = false;
        ent_distortion_is_enabled(osc->distortion, &enabled);
        if (enabled) {
                synth->buffer_update = true;
                entropictron_wakeup(kick);
        }
        return res;
}

enum entropictron_error
entropictron_osc_distortion_get_in_limiter(struct entropictron *kick,
                                       size_t index,
                                       ent_real *value)
{
        struct ent_oscillator* osc;
        osc = ent_synth_get_oscillator(kick->synths[kick->per_index], index);
        if (!osc)
                return ENTROPICTRON_ERROR;
        return ent_distortion_get_in_limiter(osc->distortion, value);
}

enum entropictron_error
entropictron_osc_distortion_set_out_limiter(struct entropictron *kick,
                                        size_t index,
                                        ent_real value)
{
        struct ent_oscillator* osc;
        struct ent_synth *synth = kick->synths[kick->per_index];
        osc = ent_synth_get_oscillator(synth, index);
        if (!osc)
                return ENTROPICTRON_ERROR;
        enum entropictron_error res;
        res = ent_distortion_set_out_limiter(osc->distortion, value);
        if (res != ENTROPICTRON_OK)
                return res;
        bool enabled = false;
        ent_distortion_is_enabled(osc->distortion, &enabled);
        if (enabled) {
                synth->buffer_update = true;
                entropictron_wakeup(kick);
        }
        return res;
}

enum entropictron_error
entropictron_osc_distortion_get_out_limiter(struct entropictron *kick,
                                        size_t index,
                                        ent_real *value)
{
        struct ent_oscillator* osc;
        osc = ent_synth_get_oscillator(kick->synths[kick->per_index], index);
        if (!osc)
                return ENTROPICTRON_ERROR;
        return ent_distortion_get_out_limiter(osc->distortion, value);
}

enum entropictron_error
entropictron_osc_distortion_set_drive(struct entropictron *kick,
                                  size_t index,
                                  ent_real drive)
{
        struct ent_oscillator* osc;
        struct ent_synth *synth = kick->synths[kick->per_index];
        osc = ent_synth_get_oscillator(synth, index);
        if (!osc)
                return ENTROPICTRON_ERROR;
        enum entropictron_error res;
        res = ent_distortion_set_drive(osc->distortion, drive);
        if (res != ENTROPICTRON_OK)
                return res;
        bool enabled = false;
        ent_distortion_is_enabled(osc->distortion, &enabled);
        if (enabled) {
                synth->buffer_update = true;
                entropictron_wakeup(kick);
        }
        return res;
}

enum entropictron_error
entropictron_osc_distortion_get_drive(struct entropictron *kick,
                                  size_t index,
                                  ent_real *drive)
{
        struct ent_oscillator* osc;
        osc = ent_synth_get_oscillator(kick->synths[kick->per_index], index);
        if (!osc)
                return ENTROPICTRON_ERROR;
        return ent_distortion_get_drive(osc->distortion, drive);
}

int entropictron_is_module_enabed(struct entropictron *kick,
                              enum ENTROPICTRON_MODULE module)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return 0;
        }

        switch (module)
        {
        case ENTROPICTRON_MODULE_JACK:
                if (kick->audio != NULL && kick->audio->jack != NULL)
                        return 1;
                else
                        return 0;
        default:
                return 0;
        }
}

enum entropictron_error
entropictron_enable_group(struct entropictron *kick,
                      size_t index,
                      bool enable)
{
        if (kick == NULL || index >= GKICK_OSC_GROUPS_NUMBER) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        enum entropictron_error res;
        res  = ent_synth_enable_group(kick->synths[kick->per_index],
                                        index,
                                        enable);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_group_enabled(struct entropictron *kick,
                       size_t index,
                       bool *enabled)
{
        if (kick == NULL || enabled == NULL
            || index >= GKICK_OSC_GROUPS_NUMBER) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        return ent_synth_group_enabled(kick->synths[kick->per_index],
                                         index,
                                         enabled);
}

enum entropictron_error
entropictron_group_set_amplitude(struct entropictron *kick,
                             size_t index,
                             ent_real amplitude)
{
        if (kick == NULL || index >= GKICK_OSC_GROUPS_NUMBER) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        enum entropictron_error res;
        res  = entropictron_synth_group_set_amplitude(kick->synths[kick->per_index],
                                                  index,
                                                  amplitude);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_group_get_amplitude(struct entropictron *kick,
                             size_t index,
                             ent_real *amplitude)
{
        if (kick == NULL
            || index >= GKICK_OSC_GROUPS_NUMBER
            || amplitude == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        return entropictron_synth_group_get_amplitude(kick->synths[kick->per_index],
                                                  index,
                                                  amplitude);
}

enum entropictron_error
entropictron_tune_audio_output(struct entropictron *kick,
                           size_t index,
                           bool tune)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        return ent_mixer_tune_output(kick->audio->mixer,
                                       index,
                                       tune);
}

enum entropictron_error
entropictron_is_audio_output_tuned(struct entropictron *kick,
                               size_t index,
                               bool *tune)
{
        if (kick == NULL || tune == NULL
            || index >= ENTROPICTRON_MAX_INSTRUMENTS) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

	return ent_mixer_is_output_tuned(kick->audio->mixer,
                                           index,
                                           tune);
}

enum entropictron_error
entropictron_set_osc_sample(struct entropictron *kick,
                        size_t osc_index,
                        const ent_real *data,
                        size_t size)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        enum entropictron_error res;
        res  = entropictron_synth_set_osc_sample(kick->synths[kick->per_index],
                                             osc_index,
                                             data, size);
        if (res == ENTROPICTRON_OK && kick->synths[kick->per_index]->buffer_update)
                entropictron_wakeup(kick);
        return res;
}

enum entropictron_error
entropictron_get_osc_sample(struct entropictron *kick,
                        size_t osc_index,
                        ent_real **data,
                        size_t *size)
{
        if (kick == NULL || data == NULL || size == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        return entropictron_synth_get_osc_sample(kick->synths[kick->per_index],
                                             osc_index,
                                             data,
                                             size);
}

enum entropictron_error
entropictron_set_current_percussion(struct entropictron *kick,
                                size_t index)
{
        if (kick == NULL || index >= ENTROPICTRON_MAX_INSTRUMENTS) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        kick->per_index = index;
        kick->audio->mixer->limiter_callback_index = index;
        return ENTROPICTRON_OK;
}

enum entropictron_error
entropictron_get_current_percussion(struct entropictron *kick,
                                size_t *index)
{
        if (kick == NULL || index == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

	*index = kick->per_index;
	return ENTROPICTRON_OK;
}

void entropictron_process(struct entropictron *kick)
{
        for (size_t i = 0; i < ENTROPICTRON_MAX_INSTRUMENTS; i++) {
                struct ent_synth *synth = kick->synths[i];
                if (synth != NULL && synth->is_active && synth->buffer_update)
                        ent_synth_process(synth);
        }
}

/**
 * Finds the first unused/free percussion index.
 */
enum entropictron_error
entropictron_unused_percussion(struct entropictron *kick,
                           int *index)
{
        if (kick == NULL || index == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

	*index = -1;
        for (size_t i = 0; i < ENTROPICTRON_MAX_INSTRUMENTS; i++) {
                if (!kick->synths[i]->is_active) {
                        *index = i;
                        return ENTROPICTRON_OK;
                }
        }
        return ENTROPICTRON_ERROR;
}

enum entropictron_error
entropictron_enable_percussion(struct entropictron *kick,
                           size_t index,
                           bool enable)
{
        if (kick == NULL || index >= ENTROPICTRON_MAX_INSTRUMENTS) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        kick->synths[index]->is_active  = enable;
        kick->audio->audio_outputs[index]->enabled = enable;
	return ENTROPICTRON_OK;
}

enum entropictron_error
entropictron_is_percussion_enabled(struct entropictron *kick,
                               size_t index,
                               bool *enable)
{
        if (kick == NULL || enable == NULL
            || index >= ENTROPICTRON_MAX_INSTRUMENTS) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        *enable = kick->synths[index]->is_active;
        return ENTROPICTRON_OK;
}

enum entropictron_error
entropictron_set_playing_key(struct entropictron *kick,
                         size_t id,
                         signed char key)
{
        if (kick == NULL || id >= ENTROPICTRON_MAX_INSTRUMENTS) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_audio_output_set_playing_key(kick->synths[id]->output, key);
}

enum entropictron_error
entropictron_get_playing_key(struct entropictron *kick, size_t id, signed char *key)
{
        if (kick == NULL || key == NULL
            || id >= ENTROPICTRON_MAX_INSTRUMENTS) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_audio_output_get_playing_key(kick->synths[id]->output,
                                                  key);
}

enum entropictron_error
entropictron_set_midi_channel(struct entropictron *kick,
                          size_t id,
                          signed char channel)
{
        if (kick == NULL || id >= ENTROPICTRON_MAX_INSTRUMENTS) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        ent_log_debug("id: %d, ch: %d", id, channel);
        return ent_audio_output_set_midi_channel(kick->synths[id]->output, channel);
}

enum entropictron_error
entropictron_get_midi_channel(struct entropictron *kick,
                          size_t id,
                          signed char *channel)
{
        if (kick == NULL
            || channel == NULL
            || id >= ENTROPICTRON_MAX_INSTRUMENTS) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_audio_output_get_midi_channel(kick->synths[id]->output, channel);
}

enum entropictron_error
entropictron_force_midi_channel(struct entropictron *kick,
                            signed char channel,
                            bool force)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        return ent_mixer_force_midi_channel(kick->audio->mixer, channel, force);
}

enum entropictron_error
entropictron_ged_forced_midi_channel(struct entropictron *kick,
                                 signed char *channel,
                                 bool *force)
{
        if (kick == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        return ent_mixer_get_forced_midi_channel(kick->audio->mixer, channel, force);
}

size_t
entropictron_midi_channels_number(void)
{
        return ENTROPICTRON_MAX_MIDI_CHANNELS;
}

enum entropictron_error
entropictron_set_percussion_name(struct entropictron *kick,
                             size_t id,
                             const char *name,
                             size_t size)
{
        if (kick == NULL || id >= ENTROPICTRON_MAX_INSTRUMENTS
            || name == NULL || size < 1) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        struct ent_synth *synth = kick->synths[id];
        ent_synth_lock(synth);
        memset(synth->name, '\0', sizeof(synth->name));
        if (size < strlen(synth->name))
                strcpy(synth->name, name);
        else
                strncpy(synth->name, name, sizeof(synth->name) - 1);
        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

enum entropictron_error
entropictron_get_percussion_name(struct entropictron *kick,
                             size_t id,
                             char *name,
                             size_t size)
{
        if (kick == NULL || id >= ENTROPICTRON_MAX_INSTRUMENTS
            || name == NULL || size < 1) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        struct ent_synth *synth = kick->synths[id];
        ent_synth_lock(synth);
        memset(name, '\0', size);
        if (size > strlen(synth->name))
                strcpy(name, synth->name);
        else
                strncpy(name, synth->name, size - 1);
        ent_synth_unlock(synth);
        return ENTROPICTRON_OK;
}

size_t
entropictron_channels_number()
{
        return ENTROPICTRON_MAX_CHANNELS;
}

size_t
entropictron_instruments_number()
{
        return ENTROPICTRON_MAX_INSTRUMENTS;
}

enum entropictron_error
entropictron_set_percussion_channel(struct entropictron *kick,
                                size_t id,
                                size_t channel)
{
        if (kick == NULL || id >= ENTROPICTRON_MAX_INSTRUMENTS
            || channel >= ENTROPICTRON_MAX_CHANNELS) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_audio_output_set_channel(kick->synths[id]->output, channel);
}

enum entropictron_error
entropictron_get_percussion_channel(struct entropictron *kick,
                                size_t id,
                                size_t *channel)
{
        if (kick == NULL || channel == NULL || id >= ENTROPICTRON_MAX_INSTRUMENTS) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_audio_output_get_channel(kick->synths[id]->output,
                                              channel);
}

enum entropictron_error
entropictron_percussion_set_limiter(struct entropictron *kick,
                                size_t id,
                                ent_real val)
{
        if (kick == NULL || id >= ENTROPICTRON_MAX_INSTRUMENTS) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_mixer_limiter_set(kick->audio->mixer, id, val);
}

enum entropictron_error
entropictron_percussion_get_limiter(struct entropictron *kick,
                                size_t id,
                                ent_real *val)
{
        if (kick == NULL || val == NULL || id >= ENTROPICTRON_MAX_INSTRUMENTS) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_mixer_limiter_get(kick->audio->mixer, id, val);
}

enum entropictron_error
entropictron_percussion_mute(struct entropictron *kick,
                         size_t id,
                         bool b)
{
        if (kick == NULL || id >= ENTROPICTRON_MAX_INSTRUMENTS) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_mixer_mute(kick->audio->mixer, id, b);
}

enum entropictron_error
entropictron_percussion_is_muted(struct entropictron *kick,
                             size_t id,
                             bool *b)
{
        if (kick == NULL || b == NULL || id >= ENTROPICTRON_MAX_INSTRUMENTS) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_mixer_is_muted(kick->audio->mixer, id, b);
}

enum entropictron_error
entropictron_percussion_solo(struct entropictron *kick,
                         size_t id,
                         bool b)
{
        if (kick == NULL || id >= ENTROPICTRON_MAX_INSTRUMENTS) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_mixer_solo(kick->audio->mixer, id, b);
}

enum entropictron_error
entropictron_percussion_is_solo(struct entropictron *kick,
                            size_t id,
                            bool *b)
{
        if (kick == NULL || b == NULL || id >= ENTROPICTRON_MAX_INSTRUMENTS) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }
        return ent_mixer_is_solo(kick->audio->mixer, id, b);
}

enum entropictron_error
entropictron_set_preview_sample(struct entropictron *kick,
                            const ent_real *data,
                            size_t size)
{
        if (kick == NULL || data == NULL || size < 1) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        struct ent_audio_output *output;
        output = kick->audio->mixer->audio_outputs[ENTROPICTRON_AUDITION_CHANNEL_INDEX];
        ent_audio_output_lock(output);
        ent_buffer_set_data((struct ent_buffer*)output->updated_buffer, data, size);
        ent_audio_output_unlock(output);
        return ENTROPICTRON_OK;
}

void
entropictron_play_sample_preview(struct entropictron *kick)
{
        ent_audio_play(kick->audio,
                         ENTROPICTRON_AUDITION_CHANNEL_INDEX);
}

enum entropictron_error
entropictron_set_sample_preview_limiter(struct entropictron *kick, ent_real val)
{
        return ent_audio_set_limiter_val(kick->audio,
                                           ENTROPICTRON_AUDITION_CHANNEL_INDEX,
                                           val);
}

enum entropictron_error
entropictron_get_sample_preview_limiter(struct entropictron *kick, ent_real *val)
{
        return ent_audio_get_limiter_val(kick->audio,
                                           ENTROPICTRON_AUDITION_CHANNEL_INDEX,
                                           val);
}

enum entropictron_error
entropictron_percussion_enable_note_off(struct entropictron *kick,
                                    size_t id,
                                    bool enable)
{
        return ent_mixer_enable_note_off(kick->audio->mixer, id, enable);
}

enum entropictron_error
entropictron_percussion_note_off_enabled(struct entropictron *kick,
                                     size_t id,
                                     bool *enabled)
{
        return ent_mixer_note_off_enabled(kick->audio->mixer, id, enabled);
}

void
entropictron_wakeup(struct entropictron *kick)
{
        if (kick->synthesis_on)
                entropictron_worker_wakeup();
}

bool
entropictron_is_plugin()
{
        return !GKICK_IS_STANDALONE;
}

void
entropictron_usleep(unsigned long int usecods)
{
        usleep(usecods);
}


int entropictron_rand(unsigned int *seed)
{
#ifdef ENTROPICTRON_OS_WINDOWS
        ENTROPICTRON_UNUSED(seed);
        // Use rand & srand since the calls are only from a single thread.
        return rand();
#else
        return rand_r(seed);
#endif // EONTRIPOCTRON_OS_GNU
}

size_t entropictron_layers_number(void)
{
        return GKICK_OSC_GROUPS_NUMBER;
}

