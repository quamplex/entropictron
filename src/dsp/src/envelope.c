/**
 * File name: envelope.c
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

#include "envelope.h"

struct ent_envelope*
ent_envelope_create(void)
{
	struct ent_envelope *envelope;
	envelope = (struct ent_envelope*)calloc(1, sizeof(struct ent_envelope));
	envelope->apply_type = ENTROPICTRON_ENVELOPE_APPLY_LINEAR;
        envelope->curve_type = ENTROPICTRON_ENVELOPE_CURVE_LINEAR;
	return envelope;
}

void ent_envelope_set_apply_type(struct ent_envelope* envelope,
				   enum ent_envelope_apply_type apply_type)
{
        envelope->apply_type = apply_type;
}

enum ent_envelope_apply_type
ent_envelope_get_apply_type(const struct ent_envelope* envelope)
{
        return envelope->apply_type;
}

void ent_envelope_set_curve_type(struct ent_envelope* envelope,
				   enum ent_envelope_curve_type type)
{
        envelope->curve_type = type;
}

enum ent_envelope_curve_type
ent_envelope_get_curve_type(struct ent_envelope* envelope)
{
        return envelope->curve_type;
}

static ent_real linear_interpolate(ent_real x0,
                                     ent_real y0,
                                     ent_real x1,
                                     ent_real y1,
                                     ent_real x)
{
    return y0 + (y1 - y0) * ((x - x0) / (x1 - x0));
}

static ent_real bezier_x(ent_real t,
                           ent_real x0,
                           ent_real x1,
                           ent_real x2,
                           ent_real x3)
{
        const ent_real one_minus_t = 1.0f - t;
        return pow(one_minus_t, 3.0f) * x0
                + 3.0f * pow(one_minus_t, 2.0f) * t * x1
                + 3.0f * one_minus_t * pow(t, 2.0f) * x2
                + pow(t, 3.0f) * x3;
}

static ent_real bezier_y(ent_real t,
                           ent_real y0,
                           ent_real y1,
                           ent_real y2,
                           ent_real y3)
{
        const ent_real one_minus_t = 1.0f - t;
        return pow(one_minus_t, 3.0f) * y0
                + 3.0f * pow(one_minus_t, 2.0f) * t * y1
                + 3.0f * one_minus_t * pow(t, 2.0f) * y2
                + pow(t, 3.0f) * y3;
}

static ent_real find_t(const struct ent_envelope_point *p1,
                         const struct ent_envelope_point *p2,
                         const struct ent_envelope_point *p3,
                         ent_real xm)
{
        ent_real t0 = 0.0f;
        ent_real t1 = 1.0f;
        ent_real t;
        const ent_real p1_x = p1->x;
        const ent_real p2_x = p2->x;
        const ent_real p3_x = p3->x;
        // Binary search to find t corresponding to xm.
        while (t1 - t0 > 1e-6) {
                t = (t0 + t1) * 0.5f;
                ent_real x_t = bezier_x(t, p1_x, p1_x, p2_x, p3_x);
                if (x_t < xm)
                        t0 = t;
                else
                        t1 = t;
        }
        return t;
}

static ent_real bezier(struct ent_envelope_point *p1,
                         struct ent_envelope_point *p2,
                         struct ent_envelope_point *p3,
                         float xm)
{
    if (fabsl(p3->x - p1->x) < DBL_EPSILON)
            return p1->y;

    ent_real t = find_t(p1, p2, p3, xm);
    return bezier_y(t, p1->y, p1->y, p2->y, p3->y);
}

ent_real
ent_envelope_get_value(const struct ent_envelope* envelope,
                         ent_real xm)
{
    if (envelope == NULL
        || envelope->npoints < 2
        || envelope->first == NULL
        || envelope->last == NULL)
        return 0.0f;

    struct ent_envelope_point *p = envelope->first;
    if (xm < p->x || xm > envelope->last->x)
            return 0.0f;
    else if (fabsl(xm - p->x) < DBL_EPSILON)
            return p->y;
    else if (fabsl(envelope->last->x - xm) < DBL_EPSILON)
            return envelope->last->y;

    if (envelope->npoints == 2)
        return linear_interpolate(envelope->first->x, envelope->first->y,
                                  envelope->last->x, envelope->last->y, xm);

    struct ent_envelope_point *p1 = NULL, *p2 = NULL;
    while (p) {
            p1 = p;
            p2 = p->next;
            if (p2 && p1->x <= xm && xm <= p2->x)
                    break;
            p = p->next;
    }

    if (!p1 || !p2)
            return 0.0f;

    if (p1->is_control && p1->prev)
            return bezier(p1->prev, p1, p2, xm);

    if (p2->is_control && p2->next)
            return bezier(p1, p2, p2->next, xm);

    return linear_interpolate(p1->x, p1->y, p2->x, p2->y, xm);
}

static void ent_envelope_add_sorted(struct ent_envelope *envelope,
			       struct ent_envelope_point *point)
{
	struct ent_envelope_point *p;
        if (point->x >= envelope->last->x) {
                /* Add as a last element. */
		envelope->last->next = point;
	        point->prev = envelope->last;
		envelope->last = point;
	} else if (point->x <= envelope->first->x) {
                /* Add as a first element. */
		envelope->first->prev = point;
	        point->next = envelope->first;
		envelope->first = point;
        } else {
                p = envelope->first;
		while (p) {
                        if (p->next != NULL && point->x < p->next->x) {
                                p->next->prev = point;
                                point->next = p->next;
                                point->prev = p;
                                p->next = point;
                                break;
                        }
                        p = p->next;
		}
	}
}

struct ent_envelope_point*
ent_envelope_add_point(struct ent_envelope *envelope,
                         const struct ent_envelope_point_info *point_info)
{
	struct ent_envelope_point *point;
	if (envelope == NULL)
		return NULL;
	point = (struct ent_envelope_point*)malloc(sizeof(struct ent_envelope_point));
	if (point == NULL)
		return NULL;
	point->x = point_info->x;
	point->y = point_info->y;
        point->is_control = point_info->control_point;
        point->next = point->prev = NULL;

	if (envelope->first == NULL ||  envelope->last == NULL)
		envelope->first = envelope->last = point;
	else
		ent_envelope_add_sorted(envelope, point);
	envelope->npoints++;
	return point;
}

void ent_envelope_destroy(struct ent_envelope *envelope)
{
	struct ent_envelope_point *point;
	if (envelope == NULL)
		return;

	if (envelope->npoints != 0) {
		while (envelope->first != NULL) {
			point = envelope->first;
			envelope->first = point->next;
			free(point);
		}
	}
	free(envelope);
}

void
ent_envelope_get_points(struct ent_envelope *env,
			  struct ent_envelope_point_info **buff,
			  size_t *npoints)
{
        struct ent_envelope_point *p;
        struct ent_envelope_point_info *points_info;
        size_t i;

        if (buff == NULL)
                return;

        *buff = NULL;
        if (env->npoints < 1)
                return;

        points_info = (struct ent_envelope_point_info*)calloc(1, sizeof(struct ent_envelope_point_info) * env->npoints);
        p = env->first;
        i = 0;
        while (p) {
                points_info[i].x = p->x;
                points_info[i].y = p->y;
                points_info[i].control_point = p->is_control;
                p = p->next;
                i++;
        }

        *buff = points_info;
        *npoints = env->npoints;
}

void
ent_envelope_set_points(struct ent_envelope *env,
			  const struct ent_envelope_point_info *buff,
			  size_t npoints)
{
        if (env == NULL || buff == NULL)
                return;

        ent_envelope_clear(env);
        for (size_t i = 0; i < npoints; i++)
                ent_envelope_add_point(env, &buff[i]);
}

void ent_envelope_clear(struct ent_envelope* env)
{
        struct ent_envelope_point *p = env->first;
        struct ent_envelope_point *curr;
        while (p != NULL) {
                curr = p;
                p = p->next;
                free(curr);
                env->npoints--;
        }
        env->first = env->last = NULL;
}

void
ent_envelope_remove_point(struct ent_envelope *env, size_t index)
{
        if (env == NULL || index >= env->npoints)
                return;

        struct ent_envelope_point *p = env->first;
        size_t i = 0;
        while (p) {
                if (i == index) {
                        if (p == env->first) {
                                env->first = p->next;
                        } else if (p == env->last) {
                                env->last = p->prev;
                        } else {
                                p->prev->next = p->next;
                                p->next->prev = p->prev;
                        }
                        free(p);
                        env->npoints--;
                        break;
                }
                p = p->next;
                i++;
        }
}

void
ent_envelope_update_point(struct ent_envelope *env,
			    size_t index,
                            const struct ent_envelope_point_info *point_info)
{
        if (env == NULL || index >= env->npoints)
                return;

        struct ent_envelope_point *p = env->first;
        size_t i = 0;
        while (p) {
                if (i == index) {
                        p->x = point_info->x;
                        p->y = point_info->y;
                        p->is_control = point_info->control_point;
                        break;
                }
                p = p->next;
                i++;
        }
}
