/**
 * File name: synthesizer.h
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

#ifndef GKICK_SYNTHESIZER_H
#define GKICK_SYNTHESIZER_H

#include "entropictron_internal.h"
#include "distortion.h"
#include "audio_output.h"

#include <stdatomic.h>

#include <pthread.h>

struct ent_synth {
	int sample_rate;
      	atomic_size_t id;
        char name[30];

	/* Speciafies if the synthesizer is active. */
	atomic_bool is_active;

        ent_real current_time;
        struct ent_oscillator **oscillators;
        size_t oscillators_number;

        /* Groups of oscillators. */
        bool osc_groups[GKICK_OSC_GROUPS_NUMBER];

        /* Total amplitude for every group of oscillatos. */
        ent_real osc_groups_amplitude[GKICK_OSC_GROUPS_NUMBER];

        /* Kick general amplitude */
        ent_real amplitude;

        /* Time length of the kick in seconds. */
        ent_real length;

        /* Kick general filter */
        struct ent_filter *filter;
        int filter_enabled;

        /* Kick distortion */
        struct ent_distortion *distortion;

        /* General synthesizer amplitude envelope. */
        struct ent_envelope *envelope;

        /* To update or not the buffer. */
        atomic_bool buffer_update;

        /**
         * Kick samples buffer where the synthesizer is doing the synthesis.
         * It is swapped with one of the oudio output buffers atomically.
         */
        struct ent_buffer* buffer;

        /**
         * Audio output that is shared with audio thread
         */
        struct ent_audio_output *output;

        /**
         * Pointer to a function to be
         * called when the synth has finished the synthesis.
         */
        void (*buffer_callback) (void*, ent_real *buff,
                                 size_t size, size_t id);
        void *callback_args;
        pthread_mutex_t lock;
};

enum entropictron_error
ent_synth_new(struct ent_synth **synth, int sample_rate);

void
ent_synth_free(struct ent_synth **synth);

void
ent_synth_lock(struct ent_synth *synth);

void
ent_synth_unlock(struct ent_synth *synth);

enum entropictron_error
ent_synth_create_oscillators(struct ent_synth *synth);

enum entropictron_error
ent_synth_get_oscillators_number(struct ent_synth *synth,
				   size_t *number);

struct ent_oscillator*
ent_synth_get_oscillator(struct ent_synth *synth,
			   size_t index);

enum entropictron_error
ent_synth_enable_oscillator(struct ent_synth *synth,
			      size_t index,
			      int enable);

enum entropictron_error
ent_synth_osc_is_enabled(struct ent_synth *synth,
			   size_t index,
			   int *enabled);

struct ent_envelope*
ent_synth_osc_get_env(struct ent_synth *synth,
                        size_t osc_index,
                        size_t env_index);

enum entropictron_error
ent_synth_osc_set_fm(struct ent_synth *synth,
                       size_t index,
		       bool is_fm);

enum entropictron_error
ent_synth_osc_is_fm(struct ent_synth *synth,
                      size_t index,
		      bool *is_fm);

enum entropictron_error
ent_synth_osc_envelope_points(struct ent_synth *synth,
                                int osc_index,
                                int env_index,
                                struct ent_envelope_point_info **buf,
                                size_t *npoints);

enum entropictron_error
ent_synth_osc_envelope_set_points(struct ent_synth *synth,
                                    int osc_index,
                                    int env_index,
                                    const struct ent_envelope_point_info *buf,
                                    size_t npoints);

enum entropictron_error
ent_synth_osc_env_add_point(struct ent_synth *synth,
                              int osc_index,
                              int env_index,
                              struct ent_envelope_point_info *point_info);

enum entropictron_error
ent_synth_osc_env_remove_point(struct ent_synth *synth,
                                 size_t osc_index,
                                 size_t env_index,
                                 size_t index);

enum entropictron_error
ent_synth_osc_env_update_point(struct ent_synth *synth,
                                 int osc_index,
                                 int env_index,
                                 int index,
                                 struct ent_envelope_point_info *point_info);

enum entropictron_error
synth_osc_env_set_apply_type(struct ent_synth *synth,
			     size_t osc_index,
			     size_t env_index,
			     enum ent_envelope_apply_type apply_type);

enum entropictron_error
synth_osc_env_get_apply_type(struct ent_synth *synth,
			     size_t osc_index,
			     size_t env_index,
			     enum ent_envelope_apply_type *apply_type);

enum entropictron_error
ent_synth_set_osc_function(struct ent_synth *synth,
                             size_t osc_index,
                             enum entropictron_osc_func_type type);

enum entropictron_error
ent_synth_get_osc_function(struct ent_synth *synth,
                             size_t osc_index,
                             enum entropictron_osc_func_type *type);

enum entropictron_error
ent_synth_set_osc_phase(struct ent_synth *synth,
                          size_t osc_index,
                          ent_real phase);

enum entropictron_error
ent_synth_get_osc_phase(struct ent_synth *synth,
                          size_t osc_index,
                          ent_real *phase);

enum entropictron_error
ent_synth_set_osc_seed(struct ent_synth *synth,
                         size_t osc_index,
                         unsigned int seed);

enum entropictron_error
ent_synth_get_osc_seed(struct ent_synth *synth,
                         size_t osc_index,
                         unsigned *seed);

enum entropictron_error
ent_synth_get_length(struct ent_synth *synth,
		       ent_real *len);

enum entropictron_error
ent_synth_set_length(struct ent_synth *synth,
		       ent_real len);

enum entropictron_error
ent_synth_kick_set_amplitude(struct ent_synth *synth,
			       ent_real amplitude);

enum entropictron_error
ent_synth_kick_get_amplitude(struct ent_synth *synth,
			       ent_real *amplitude);

enum entropictron_error
entropictron_synth_kick_filter_enable(struct ent_synth *synth,
				  int enable);

enum entropictron_error
entropictron_synth_kick_filter_is_enabled(struct ent_synth *synth,
				      int *enabled);

enum entropictron_error
ent_synth_kick_set_filter_frequency(struct ent_synth *synth,
				      ent_real frequency);

enum entropictron_error
ent_synth_kick_get_filter_frequency(struct ent_synth *synth,
				      ent_real *frequency);

enum entropictron_error
ent_synth_kick_set_filter_factor(struct ent_synth *synth,
				   ent_real factor);

enum entropictron_error
ent_synth_kick_get_filter_factor(struct ent_synth *synth,
				   ent_real *factor);

enum entropictron_error
ent_synth_set_kick_filter_type(struct ent_synth *synth,
				 enum ent_filter_type type);

enum entropictron_error
ent_synth_get_kick_filter_type(struct ent_synth *synth,
				 enum ent_filter_type *type);

struct ent_envelope*
synth_get_kick_envelope(struct ent_synth *synth,
			enum entropictron_envelope_type env_type);

enum entropictron_error
ent_synth_kick_envelope_get_points(struct ent_synth *synth,
                                     enum entropictron_envelope_type env_type,
                                     struct ent_envelope_point_info **buf,
                                     size_t *npoints);

enum entropictron_error
ent_synth_kick_envelope_set_points(struct ent_synth *synth,
                                     enum entropictron_envelope_type env_type,
                                     const struct ent_envelope_point_info *buf,
                                     size_t npoints);

enum entropictron_error
ent_synth_kick_add_env_point(struct ent_synth *synth,
                               enum entropictron_envelope_type env_type,
                               const struct ent_envelope_point_info *point_info);

enum entropictron_error
ent_synth_kick_remove_env_point(struct ent_synth *synth,
                                  enum entropictron_envelope_type env_type,
                                  size_t index);

enum entropictron_error
ent_synth_kick_update_env_point(struct ent_synth *synth,
                                  enum entropictron_envelope_type env_type,
                                  size_t index,
                                  const struct ent_envelope_point_info *point_info);

enum entropictron_error
synth_kick_env_set_apply_type(struct ent_synth *synth,
			      enum entropictron_envelope_type env_type,
			      enum ent_envelope_apply_type apply_type);

enum entropictron_error
synth_kick_env_get_apply_type(struct ent_synth *synth,
			      enum entropictron_envelope_type env_type,
			      enum ent_envelope_apply_type *apply_type);

enum entropictron_error
ent_synth_set_osc_frequency(struct ent_synth *synth,
                             size_t osc_index,
                             ent_real v);

enum entropictron_error
ent_synth_get_osc_frequency(struct ent_synth *synth,
                             size_t osc_index,
                             ent_real *v);

enum entropictron_error
ent_synth_set_osc_pitch_shift(struct ent_synth *synth,
                                size_t osc_index,
                                ent_real semitones);

enum entropictron_error
ent_synth_get_osc_pitch_shift(struct ent_synth *synth,
                                size_t osc_index,
                                ent_real *semitones);

enum entropictron_error
ent_synth_set_osc_noise_density(struct ent_synth *synth,
                                size_t osc_index,
                                ent_real density);

enum entropictron_error
ent_synth_get_osc_noise_density(struct ent_synth *synth,
                                size_t osc_index,
                                ent_real *density);

enum entropictron_error
ent_synth_set_osc_amplitude(struct ent_synth *synth,
                              size_t osc_index,
                              ent_real v);

enum entropictron_error
ent_synth_get_osc_amplitude(struct ent_synth *synth,
                              size_t osc_index,
                              ent_real *v);

enum entropictron_error
ent_synth_get_buffer_size(struct ent_synth *synth,
                            size_t *size);

enum entropictron_error
ent_synth_get_buffer(struct ent_synth *synth,
                       ent_real *buffer,
                       size_t size);

void ent_synth_set_output(struct ent_synth *synth,
                            struct ent_audio_output *output);

enum entropictron_error
ent_synth_process(struct ent_synth *synth);

ent_real
ent_synth_get_value(struct ent_synth *synth,
                      ent_real t);

void
ent_synth_reset_oscillators(struct ent_synth *synth);

int
ent_synth_is_update_buffer(struct ent_synth *synth);

enum entropictron_error
ent_synth_set_osc_filter_type(struct ent_synth *synth,
                                size_t osc_index,
                                enum ent_filter_type type);

enum entropictron_error
ent_synth_get_osc_filter_type(struct ent_synth *synth,
                                size_t osc_index,
                                enum ent_filter_type *type);

enum entropictron_error
ent_synth_set_osc_filter_cutoff(struct ent_synth *synth,
                                  size_t osc_index,
                                  ent_real cutoff);

enum entropictron_error
ent_synth_get_osc_filter_cutoff(struct ent_synth *synth,
                                  size_t osc_index,
                                  ent_real *cutoff);

enum entropictron_error
ent_synth_set_osc_filter_factor(struct ent_synth *synth,
                                  size_t osc_index,
                                  ent_real factor);

enum entropictron_error
ent_synth_get_osc_filter_factor(struct ent_synth *synth,
                                  size_t osc_index,
                                  ent_real *factor);

enum entropictron_error
ent_synth_osc_enable_filter(struct ent_synth *synth,
                              size_t osc_index,
                              int enable);

enum entropictron_error
ent_synth_osc_is_enabled_filter(struct ent_synth *synth,
                                  size_t osc_index,
                                  int *enabled);

enum entropictron_error
ent_synth_distortion_enable(struct ent_synth *synth,
			      bool enable);

enum entropictron_error
ent_synth_distortion_is_enabled(struct ent_synth *synth,
				  bool *enabled);

enum entropictron_error
ent_synth_distortion_set_type(struct ent_synth *synth,
                                enum ent_distortion_type type);

enum entropictron_error
ent_synth_distortion_get_type(struct ent_synth *synth,
                                enum ent_distortion_type *type);

enum entropictron_error
ent_synth_distortion_get_in_limiter(struct ent_synth *synth,
                                      ent_real *value);

enum entropictron_error
ent_synth_distortion_set_in_limiter(struct ent_synth *synth,
                                      ent_real value);

enum entropictron_error
ent_synth_distortion_set_out_limiter(struct ent_synth *synth,
                                       ent_real value);

enum entropictron_error
ent_synth_distortion_get_out_limiter(struct ent_synth *synth,
                                       ent_real *value);

enum entropictron_error
ent_synth_distortion_set_drive(struct ent_synth *synth,
				 ent_real drive);

enum entropictron_error
ent_synth_distortion_get_drive(struct ent_synth *synth,
				 ent_real *drive);

enum entropictron_error
ent_synth_enable_group(struct ent_synth *synth,
			 size_t index,
			 bool enable);

enum entropictron_error
ent_synth_group_enabled(struct ent_synth *synth,
			  size_t index,
			  bool *enabled);

enum entropictron_error
entropictron_synth_group_set_amplitude(struct ent_synth *synth,
				   size_t index,
				   ent_real amplitude);

enum entropictron_error
entropictron_synth_group_get_amplitude(struct ent_synth *synth,
				   size_t index,
				   ent_real *amplitude);

enum entropictron_error
entropictron_synth_set_osc_sample(struct ent_synth *synth,
                              size_t osc_index,
                              const ent_real *data,
                              size_t size);

enum entropictron_error
entropictron_synth_get_osc_sample(struct ent_synth *synth,
                              size_t osc_index,
                              ent_real **data,
                              size_t *size);



#endif // SYNTHESIZER_H
