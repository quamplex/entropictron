/**
 * File name: envelope.h
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

#ifndef GKICK_ENVELOPE_H
#define GKICK_ENVELOPE_H

#include "entropictron_internal.h"

struct ent_envelope_point {
	struct ent_envelope_point *prev;
	struct ent_envelope_point *next;
	ent_real x;
	ent_real y;
        bool is_control;
};

struct ent_envelope {
        enum ent_envelope_apply_type apply_type;
        enum ent_envelope_curve_type curve_type;
	size_t npoints;
	struct ent_envelope_point *first;
	struct ent_envelope_point *last;
};

struct ent_envelope*
ent_envelope_create(void);

void ent_envelope_set_apply_type(struct ent_envelope* envelope,
				   enum ent_envelope_apply_type apply_type);

enum ent_envelope_apply_type
ent_envelope_get_apply_type(const struct ent_envelope* envelope);

void ent_envelope_set_curve_type(struct ent_envelope* envelope,
				   enum ent_envelope_curve_type type);

enum ent_envelope_curve_type
ent_envelope_get_curve_type(struct ent_envelope* envelope);

ent_real
ent_envelope_get_value(const struct ent_envelope* envelope,
                         ent_real xm);

struct ent_envelope_point*
ent_envelope_add_point(struct ent_envelope *envelope,
                         const struct ent_envelope_point_info *point_info);

void ent_envelope_destroy(struct ent_envelope *envelope);

void ent_envelope_get_points(struct ent_envelope *env,
                               struct ent_envelope_point_info **buff,
                               size_t *npoints);

void ent_envelope_set_points(struct ent_envelope *env,
			  const struct ent_envelope_point_info *buff,
			  size_t npoints);

void ent_envelope_clear(struct ent_envelope* env);

void ent_envelope_remove_point(struct ent_envelope *env,
                                 size_t index);

void ent_envelope_update_point(struct ent_envelope *env,
                                 size_t index,
                                 const struct ent_envelope_point_info *point_info);

#endif // GKICK_ENVELOPE_H
