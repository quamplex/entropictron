/**
 * File name: filter.c
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

#include "filter.h"

enum entropictron_error
ent_filter_new(struct ent_filter **filter, int sample_rate)
{
        if (filter == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        *filter = (struct ent_filter *)calloc(1, sizeof(struct ent_filter));
        if (*filter == NULL) {
                ent_log_error("can't allocate memory");
                return ENTROPICTRON_ERROR_MEM_ALLOC;
        }
        (*filter)->type = ENTROPICTRON_FILTER_LOW_PASS;
        (*filter)->queue_empty = true;
        (*filter)->sample_rate = sample_rate;

        (*filter)->cutoff_env = ent_envelope_create();
        if ((*filter)->cutoff_env == NULL) {
                ent_log_error("can't create filter cutoff envelope");
                ent_filter_free(filter);
                return ENTROPICTRON_ERROR;
        } else {
                struct ent_envelope_point_info info = {.x = 0.0f,
                                .y = 1.0f, .control_point = false};
                ent_envelope_add_point((*filter)->cutoff_env, &info);
                info.x = 1.0f;
                ent_envelope_add_point((*filter)->cutoff_env, &info);
        }

	(*filter)->q_env = ent_envelope_create();
        if ((*filter)->q_env == NULL) {
                ent_log_error("can't create filter q factor envelope");
                ent_filter_free(filter);
                return ENTROPICTRON_ERROR;
        } else {
                struct ent_envelope_point_info info = {.x = 0.0f,
                                                         .y = 1.0f, .control_point = false};
                ent_envelope_add_point((*filter)->q_env, &info);
                info.x = 1.0f;
                ent_envelope_add_point((*filter)->q_env, &info);
        }

        if (pthread_mutex_init(&(*filter)->lock, NULL) != 0) {
                ent_log_error("error on init mutex");
                ent_filter_free(filter);
                return ENTROPICTRON_ERROR;
	}

        (*filter)->cutoff_freq = ENTROPICTRON_DEFAULT_FILTER_CUTOFF_FREQ;
        (*filter)->factor      = ENTROPICTRON_DEFAULT_FILTER_FACTOR;
        ent_filter_update_coefficents(*filter);

        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_filter_init(struct ent_filter *filter)
{
        if (filter == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_filter_lock(filter);
        filter->queue_empty = true;
        memset(filter->queue_l, 0, sizeof(filter->queue_l));
        memset(filter->queue_b, 0, sizeof(filter->queue_b));
        memset(filter->queue_h, 0, sizeof(filter->queue_h));
        ent_filter_update_coefficents(filter);
        ent_filter_unlock(filter);

        return ENTROPICTRON_OK;
}

void ent_filter_free(struct ent_filter **filter)
{
        if (filter != NULL && *filter != NULL) {
                ent_envelope_destroy((*filter)->cutoff_env);
		ent_envelope_destroy((*filter)->q_env);
                pthread_mutex_destroy(&(*filter)->lock);
                free(*filter);
                *filter = NULL;
        }
}

void ent_filter_lock(struct ent_filter *filter)
{
        pthread_mutex_lock(&filter->lock);
}

void ent_filter_unlock(struct ent_filter *filter)
{
        pthread_mutex_unlock(&filter->lock);
}

enum entropictron_error
ent_filter_update_coefficents(struct ent_filter *filter)
{
        if (filter == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        filter->coefficients[1] = filter->factor;
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_filter_set_type(struct ent_filter *filter,
                      enum ent_filter_type type)
{
        if (filter == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_filter_lock(filter);
        filter->type = type;
        ent_filter_update_coefficents(filter);
        ent_filter_unlock(filter);

        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_filter_get_type(struct ent_filter *filter,
                      enum ent_filter_type *type)
{
        if (filter == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_filter_lock(filter);
        *type = filter->type;
        ent_filter_unlock(filter);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_filter_set_cutoff_freq(struct ent_filter *filter,
                             ent_real cutoff)
{
        if (filter == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_filter_lock(filter);
        filter->cutoff_freq = cutoff;
        ent_filter_update_coefficents(filter);
        ent_filter_unlock(filter);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_filter_set_factor(struct ent_filter *filter,
                        ent_real factor)
{
        if (filter == NULL || factor < 0.5f) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_filter_lock(filter);
        filter->factor = 10.0f / factor;
        ent_filter_update_coefficents(filter);
        ent_filter_unlock(filter);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_filter_get_factor(struct ent_filter *filter, ent_real *factor)
{
        if (filter == NULL || factor == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_filter_lock(filter);
        *factor = 10.0f / filter->factor;
        ent_filter_unlock(filter);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_filter_get_cutoff_freq(struct ent_filter *filter, ent_real *cutoff)
{
        if (filter == NULL || cutoff == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_filter_lock(filter);
        *cutoff = filter->cutoff_freq;
        ent_filter_unlock(filter);
        return ENTROPICTRON_OK;
}

/**
 * ent_filter_val function
 *
 * Implements low, high, and band pass digital state variable filter.
 */
enum entropictron_error
ent_filter_val(struct ent_filter *filter,
                 ent_real in_val,
                 ent_real *out_val,
                 ent_real env_x)
{
        if (isnan(in_val))
                in_val = 0.0;
        else
                in_val = GKICK_CLAMP(in_val, -1.0f, 1.0f);

        if (filter == NULL || out_val == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        ent_filter_lock(filter);

        ent_real *l = filter->queue_l;
        ent_real *b = filter->queue_b;
        ent_real *h = filter->queue_h;

	ent_real env_val = ent_envelope_get_value(filter->cutoff_env, env_x);
	enum ent_envelope_apply_type apply_type = ent_envelope_get_apply_type(filter->cutoff_env);
	ent_real f;
	if (apply_type == ENTROPICTRON_ENVELOPE_APPLY_LOGARITHMIC)
		f = pow(10, (log10(filter->cutoff_freq) - GKICK_LOG20) * env_val + GKICK_LOG20);
	else
		f = filter->cutoff_freq * env_val;
        ent_real F = 2.0f * sin(M_PI * f / filter->sample_rate);
	ent_real q_env_val = ent_envelope_get_value(filter->q_env, env_x);
	ent_real Q;
	if (q_env_val > 0) {
		Q = filter->coefficients[1] * (1.0f / q_env_val);
		if (Q > 10.0f)
			Q  = 10.0f;
	} else {
		Q = 10.0f;
	}

        size_t n = 1;
        if (filter->queue_empty) {
                l[n - 1] = l[n] = 0;
                b[n - 1] = b[n] = 0;
                h[n - 1] = h[n] = 0;
                filter->queue_empty = false;
        } else {
                h[n - 1] = h[n];
                b[n - 1] = b[n];
                l[n - 1] = l[n];
        }
        h[n] = in_val - l[n - 1] - Q * b[n - 1];
        b[n] = F * h[n] + b[n - 1];
        l[n] = F * b[n] + l[n - 1];

        ent_real val = 0.0f;
        if (filter->type == ENTROPICTRON_FILTER_HIGH_PASS)
                val = h[n];
        else if (filter->type == ENTROPICTRON_FILTER_BAND_PASS)
                val = b[n];
        else
                val = l[n];
        *out_val = val;
        ent_filter_unlock(filter);

        return ENTROPICTRON_OK;
}
