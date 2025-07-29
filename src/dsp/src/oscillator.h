/**
 * File name: oscillator.c
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

#ifndef GKICK_OSCILLATOR_H
#define GKICK_OSCILLATOR_H

#include "entropictron_internal.h"
#include "envelope.h"
#include "filter.h"

#define GKICK_OSC_DEFAULT_AMPLITUDE     1.0f
#define GKICK_OSC_DEFAULT_FREQUENCY     150.0f
#define GKICK_OSC_DEFAULT_PITCH         69.0f
#define GKICK_MAX_NOISE_DENSITY         400
#define GKICK_OSC_DEFAULT_NOISE_DENSITY GKICK_MAX_NOISE_DENSITY


enum entropictron_osc_state {
        ENTROPICTRON_OSC_STATE_DISABLED = 0,
        ENTROPICTRON_OSC_STATE_ENABLED  = 1
};

enum ent_osc_envelope_type {
        GKICK_OSC_AMPLITUDE_ENVELOPE        = 0,
        GKICK_OSC_FREQUENCY_ENVELOPE        = 1,
        GKICK_OSC_PITCH_SHIFT_ENVELOPE      = 2,
        GKICK_OSC_NOISE_DENSITY_ENVELOPE    = 3,
        GKICK_OSC_DISTORTION_DRIVE_ENVELOPE = 4
};

struct ent_oscillator {
        int sample_rate;
        enum entropictron_osc_state state;
	enum entropictron_osc_func_type func;
        /* Used for Brownian noise */
        ent_real brownian;
        /* User as a seed for pseudo random generator. */
        unsigned int seedp;
        unsigned int seed;
        ent_real initial_phase;
	ent_real phase;
	ent_real frequency;
	ent_real amplitude;
        /* Pitch shift semitones. */
        ent_real pitch_shift;

        /* Noise density. */
        ent_real noise_density;

        struct ent_buffer *sample;

        /* FM input value for this OSC. */
        ent_real fm_input;
        /* FM input coefficient. Used for compatibity of presets. */
        ent_real fm_k;

        /* Specifies if this OSC is a FM source to other oscillator. */
        bool is_fm;

	size_t env_number;
	struct ent_envelope **envelopes;
        bool filter_enabled;
        struct ent_filter *filter;
        bool distortion_enabled;
        struct ent_distortion *distortion;
	pthread_mutex_t lock;
};

struct ent_oscillator
*ent_osc_create(int sample_rate);

void ent_osc_free(struct ent_oscillator **osc);

void ent_osc_set_state(struct ent_oscillator *osc,
                         enum entropictron_osc_state state);

int ent_osc_enabled(struct ent_oscillator *osc);

enum entropictron_error
ent_osc_create_envelopes(struct ent_oscillator *osc);

struct ent_envelope*
ent_osc_get_envelope(struct ent_oscillator *osc,
                       size_t env_index);

void
ent_osc_increment_phase(struct ent_oscillator *osc,
			  ent_real t,
			  ent_real kick_len);
ent_real
ent_osc_value(struct ent_oscillator *osc,
		ent_real t,
		ent_real kick_len);

ent_real
ent_osc_func_sine(ent_real phase);

ent_real
ent_osc_func_square(ent_real phase);

ent_real
ent_osc_func_triangle(ent_real phase);

ent_real
ent_osc_func_sawtooth(ent_real phase);

ent_real
ent_osc_func_noise_white(unsigned int *seed, unsigned int density);

ent_real
ent_osc_func_noise_pink(void);

ent_real
ent_osc_func_noise_brownian(ent_real *previous,
                              unsigned int *seed,
                              unsigned int density);

ent_real
ent_osc_func_sample(struct ent_buffer *sample,
                      float pitch_shift);

void ent_osc_get_envelope_points(struct ent_oscillator *osc,
                                   size_t env_index,
                                   struct ent_envelope_point_info **buff,
                                   size_t *npoints);

void ent_osc_set_envelope_points(struct ent_oscillator *osc,
                                   size_t env_index,
                                   const struct ent_envelope_point_info *buff,
                                   size_t npoints);

unsigned int ent_osc_get_noise_density(struct ent_oscillator *osc,
                                         ent_real t,
                                         ent_real kick_len);

#endif // GKICK_OSCILLATOR_H
