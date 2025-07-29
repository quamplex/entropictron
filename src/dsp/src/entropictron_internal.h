/**
 * File name: entropictron_internal.h
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

#ifndef ENTROPICTRON_INTERNAL_H
#define ENTROPICTRON_INTERNAL_H

#include "entropictron.h"
#include "synthesizer.h"
#include "ent_audio.h"
#include "ent_buffer.h"

#include <pthread.h>
#include <stdatomic.h>
#include <math.h>

#define GKICK_2PI 6.2831853f
#define GKICK_LOG20 1.3010299f

#define ENTROPICTRON_MAX_INSTANCES 500

#include "worker.h"

/* Kick maximum length in seconds. */
#define ENTROPICTRON_MAX_LENGTH 4

struct entropictron {
        size_t id;
        char name[30];
	int sample_rate;
        /* The list of synths of available synths. */
        struct ent_synth *synths[ENTROPICTRON_MAX_INSTRUMENTS];
        struct ent_audio *audio;

        /* Current controllable percussion index. */
        _Atomic size_t per_index;

        /**
         * Specifies if the synthesis is tuned off.
         * If it is false any updates of the synthesizers parameters
         * will not trigger the synthesis.
         */
        atomic_bool synthesis_on;
        pthread_mutex_t lock;
        struct ent_worker *worker;
};

void
entropictron_lock(struct entropictron *kick);

void
entropictron_unlock(struct entropictron *kick);

void
entropictron_process(struct entropictron *kick);

void
entropictron_wakeup(struct entropictron *kick);

void
entropictron_usleep(unsigned long int usecods);

int
entropictron_rand(unsigned int *seed);

#define min(a, b) ((a) < (b) ? (a) : (b))

#define GKICK_CLAMP(value, min_v, max_v) \
    ((value) < (min_v) ? (min_v) : ((value) > (max_v) ? (max_v) : (value)))

#endif // ENTROPICTRON_INTERNAL_H
