/**
 * File name: distortion.h
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

#ifndef ENTROPICTRON_DISTORTION_H
#define ENTROPICTRON_DISTORTION_H

#include "entropictron_internal.h"

#include <pthread.h>

struct ent_distortion {
        bool enabled;
        int sample_rate;
        enum ent_distortion_type type;
	ent_real in_limiter;
        ent_real out_limiter;
        ent_real drive;
	struct ent_envelope *drive_env;
        struct ent_envelope *volume_env;
        pthread_mutex_t lock;
};

enum entropictron_error
ent_distortion_new(struct ent_distortion **distortion, int sample_rate);

void ent_distortion_free(struct ent_distortion **distortion);

void ent_distortion_lock(struct ent_distortion *distortion);

void ent_distortion_unlock(struct ent_distortion *distortion);

enum entropictron_error
ent_distortion_enable(struct ent_distortion *distortion, bool enable);

enum entropictron_error
ent_distortion_is_enabled(struct ent_distortion *distortion, bool *enabled);

enum entropictron_error
ent_distortion_set_type(struct ent_distortion *distortion,
                          enum ent_distortion_type type);

enum entropictron_error
ent_distortion_get_type(struct ent_distortion *distortion,
                          enum ent_distortion_type *type);

enum entropictron_error
ent_distortion_val(struct ent_distortion *distortion,
                     ent_real in_val,
                     ent_real *out_val,
		     ent_real env_x);

enum entropictron_error
ent_distortion_set_out_limiter(struct ent_distortion *distortion,
                                 ent_real value);

enum entropictron_error
ent_distortion_get_out_limiter(struct ent_distortion *distortion,
                                 ent_real *value);

enum entropictron_error
ent_distortion_set_in_limiter(struct ent_distortion *distortion,
                                ent_real value);

enum entropictron_error
ent_distortion_get_in_limiter(struct ent_distortion *distortion,
                                ent_real *value);

enum entropictron_error
ent_distortion_set_drive(struct ent_distortion *distortion,
                           ent_real drive);

enum entropictron_error
ent_distortion_get_drive(struct ent_distortion *distortion,
                           ent_real *drive);

#endif // ENTROPICTRON_DISTORTION_H
