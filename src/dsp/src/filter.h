/**
 * File name: filter.h
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

#ifndef ENTROPICTRON_FILTER_H
#define ENTROPICTRON_FILTER_H

#include "entropictron_internal.h"
#include "envelope.h"

#define ENTROPICTRON_DEFAULT_FILTER_CUTOFF_FREQ (350.0f)
#define ENTROPICTRON_DEFAULT_FILTER_FACTOR      (1.0f)

struct ent_filter {
        enum ent_filter_type type;
        int sample_rate;

        /* Filter cuttoff frequency. */
        ent_real cutoff_freq;

        /* Filter damping factor. */
        ent_real factor;

        /* A queue of a three elements */
        ent_real queue_l[2];
        ent_real queue_b[2];
        ent_real queue_h[2];
        bool queue_empty;

        /* Filter coefficients. */
        ent_real coefficients[2];

        /* Filter cutoff envelope. */
        struct ent_envelope *cutoff_env;
	struct ent_envelope *q_env;
        pthread_mutex_t lock;
};

enum entropictron_error
ent_filter_new(struct ent_filter **filter, int sample_rate);

enum entropictron_error
ent_filter_init(struct ent_filter *filter);

void ent_filter_free(struct ent_filter **filter);

void ent_filter_lock(struct ent_filter *filter);

void ent_filter_unlock(struct ent_filter *filter);

enum entropictron_error
ent_filter_update_coefficents(struct ent_filter *filter);

enum entropictron_error
ent_filter_set_type(struct ent_filter *filter,
                      enum ent_filter_type type);

enum entropictron_error
ent_filter_get_type(struct ent_filter *filter,
                      enum ent_filter_type *type);

enum entropictron_error
ent_filter_set_cutoff_freq(struct ent_filter *filter,
                             ent_real cutoff);

enum entropictron_error
ent_filter_set_factor(struct ent_filter *filter,
                        ent_real factor);

enum entropictron_error
ent_filter_get_cutoff_freq(struct ent_filter *filter,
                             ent_real *cutoff);

enum entropictron_error
ent_filter_get_factor(struct ent_filter *filter,
                        ent_real *factor);

enum entropictron_error
ent_filter_val(struct ent_filter *filter,
                 ent_real in_val,
                 ent_real *out_val,
                 ent_real env_x);

#endif // ENTROPICTRON_FILTER_H
