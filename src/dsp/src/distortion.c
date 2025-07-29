/**
 * File name: distortion.c
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

#include "distortion.h"
#include "envelope.h"

enum entropictron_error
ent_distortion_new(struct ent_distortion **distortion, int sample_rate)
{
        if (distortion == NULL) {
                ent_log_error("wrong arguments");
                return ENTROPICTRON_ERROR;
        }

        *distortion = (struct ent_distortion*)calloc(1, sizeof(struct ent_distortion));
        if (*distortion == NULL) {
                ent_log_error("can't allocate memory");
                return ENTROPICTRON_ERROR;
        }
	(*distortion)->drive_env   = NULL;
        (*distortion)->volume_env  = NULL;
	(*distortion)->drive       = 1.0f;
        (*distortion)->in_limiter  = 1.0f;
        (*distortion)->out_limiter = 1.0f;
        (*distortion)->sample_rate = sample_rate;
#if (ENTROPICTRON_VERSION_MAJOR > 3)
#warning Remove backward compatibility.
#endif // ENTROPICTRON_VERSION_MAJOR
        (*distortion)->type        = ENTROPICTRON_DISTORTION_SOFT_CLIPPING_TANH;

	struct ent_envelope *env = ent_envelope_create();
	if (env == NULL) {
		ent_log_error("can't create distortion drive envelope");
		ent_distortion_free(distortion);
		return ENTROPICTRON_ERROR;
	} else {
		/* Add two default points. */
                struct ent_envelope_point_info info = {0.0f, 1.0f, false};
		ent_envelope_add_point(env, &info);
                info.x = 1.0f;
		ent_envelope_add_point(env, &info);
		(*distortion)->drive_env = env;
	}

        env = ent_envelope_create();
	if (env == NULL) {
		ent_log_error("can't create distortion volume envelope");
		ent_distortion_free(distortion);
		return ENTROPICTRON_ERROR;
	} else {
		/* Add two default points. */
                struct ent_envelope_point_info info = {0.0f, 1.0f, false};
		ent_envelope_add_point(env, &info);
                info.x = 1.0f;
		ent_envelope_add_point(env, &info);
		(*distortion)->volume_env = env;
	}


        if (pthread_mutex_init(&(*distortion)->lock, NULL) != 0) {
                ent_log_error("error on init mutex");
                ent_distortion_free(distortion);
                return ENTROPICTRON_ERROR;
	}

        return ENTROPICTRON_OK;
}

void
ent_distortion_free(struct ent_distortion **distortion)
{
        if (distortion != NULL && *distortion != NULL) {
		if ((*distortion)->drive_env != NULL)
			ent_envelope_destroy((*distortion)->drive_env);
                if ((*distortion)->volume_env != NULL)
                        ent_envelope_destroy((*distortion)->volume_env);
                pthread_mutex_destroy(&(*distortion)->lock);
                free(*distortion);
                *distortion = NULL;
        }
}

void ent_distortion_lock(struct ent_distortion *distortion)
{
        pthread_mutex_lock(&distortion->lock);
}

void ent_distortion_unlock(struct ent_distortion *distortion)
{
        pthread_mutex_unlock(&distortion->lock);
}

enum entropictron_error
ent_distortion_enable(struct ent_distortion *distortion, bool enable)
{
        ent_distortion_lock(distortion);
        distortion->enabled = enable;
        ent_distortion_unlock(distortion);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_distortion_is_enabled(struct ent_distortion *distortion, bool *enabled)
{
        ent_distortion_lock(distortion);
        *enabled = distortion->enabled;
        ent_distortion_unlock(distortion);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_distortion_set_type(struct ent_distortion *distortion,
                          enum ent_distortion_type type)
{
        ent_distortion_lock(distortion);
        distortion->type = type;
        ent_distortion_unlock(distortion);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_distortion_get_type(struct ent_distortion *distortion,
                          enum ent_distortion_type *type)
{
        ent_distortion_lock(distortion);
        *type = distortion->type;
        ent_distortion_unlock(distortion);
        return ENTROPICTRON_OK;
}

/* TODO: With old presets. In the next major release this to be removed. */
#if (ENTROPICTRON_VERSION_MAJOR > 3)
#warning Remove backward compatibility.
#endif // ENTROPICTRON_VERSION_MAJOR
static void distortion_backward_compatible(struct ent_distortion *distortion,
                                           ent_real in_val,
                                           ent_real *out_val,
                                           ent_real env_x)
{
	ent_real x = distortion->in_limiter * in_val;
	x *= 1.0f + (distortion->drive - 1.0f) * ent_envelope_get_value(distortion->drive_env, env_x);

        x = GKICK_CLAMP(x, -1.0f, 1.0f);
        *out_val= (x < 0.0f ? -1.0f : 1.0f) * (1.0f - exp(-4.0f * log(10.0f) * fabs(x)));
        *out_val *= distortion->out_limiter * ent_envelope_get_value(distortion->volume_env, env_x);
}

enum entropictron_error
ent_distortion_val(struct ent_distortion *distortion,
                     ent_real in_val,
                     ent_real *out_val,
                     ent_real env_x)
{
        ent_distortion_lock(distortion);
#if (ENTROPICTRON_VERSION_MAJOR > 3)
#warning Remove backward compatibility.
#endif // ENTROPICTRON_VERSION_MAJOR
        if (distortion->type == ENTROPICTRON_DISTORTION_BACKWARD_COMPATIBLE) {
                distortion_backward_compatible(distortion,
                                               in_val,
                                               out_val,
                                               env_x);
                ent_distortion_unlock(distortion);
                return ENTROPICTRON_OK;
        }

        ent_real x = distortion->in_limiter * in_val;
        ent_real drive_env_val = ent_envelope_get_value(distortion->drive_env, env_x);
        ent_real drive = distortion->drive * ent_envelope_get_value(distortion->drive_env, env_x);
        switch (distortion->type) {
        case ENTROPICTRON_DISTORTION_HARD_CLIPPING:
                *out_val = GKICK_CLAMP(x, -1.0f / drive, 1.0f / drive);
                break;
        case ENTROPICTRON_DISTORTION_SOFT_CLIPPING_TANH:
                *out_val = tanh(drive * x);
                break;
        case ENTROPICTRON_DISTORTION_ARCTAN:
                *out_val = atan(drive * x);
                break;
        case ENTROPICTRON_DISTORTION_EXPONENTIAL:
                *out_val = (x < 0.0f ? -1.0f : 1.0f) * (1.0f - exp(-drive * fabs(x)));
                break;
        case ENTROPICTRON_DISTORTION_POLYNOMIAL:
                *out_val = x - (0.1f * drive * x * x * x) / 3.0f;
                break;
        case ENTROPICTRON_DISTORTION_LOGARITHMIC:
                *out_val = log(1.0f + drive * fabs(x)) * (x < 0.0f ? -1.0f : 1.0f);
                break;
        case ENTROPICTRON_DISTORTION_FOLDBACK:
                {
                        ent_real threshold = 1.0f;
                        *out_val = fabs(fmod(x + threshold, 2.0f * threshold) - threshold) - threshold;
                }
                break;
        case ENTROPICTRON_DISTORTION_HALF_WAVE_RECT:
                *out_val = x > 0.0f ? x : 0.0f;
                break;
        case ENTROPICTRON_DISTORTION_FULL_WAVE_RECT:
                *out_val = fabs(x);
                break;
        default:
                *out_val = x;
                break;
        }

        *out_val *= distortion->out_limiter * ent_envelope_get_value(distortion->volume_env, env_x);
        ent_distortion_unlock(distortion);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_distortion_set_out_limiter(struct ent_distortion *distortion,
                                 ent_real value)
{
        ent_distortion_lock(distortion);
        distortion->out_limiter = value;
        ent_distortion_unlock(distortion);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_distortion_get_out_limiter(struct ent_distortion *distortion,
                            ent_real *value)
{
        ent_distortion_lock(distortion);
        *value = distortion->out_limiter;
        ent_distortion_unlock(distortion);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_distortion_set_in_limiter(struct ent_distortion *distortion,
                                ent_real value)
{
	ent_distortion_lock(distortion);
        distortion->in_limiter = value;
        ent_distortion_unlock(distortion);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_distortion_get_in_limiter(struct ent_distortion *distortion,
                                ent_real *value)
{
	ent_distortion_lock(distortion);
        *value = distortion->in_limiter;
        ent_distortion_unlock(distortion);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_distortion_set_drive(struct ent_distortion *distortion,
                           ent_real drive)
{
        ent_distortion_lock(distortion);
        distortion->drive = drive;
        ent_distortion_unlock(distortion);
        return ENTROPICTRON_OK;
}

enum entropictron_error
ent_distortion_get_drive(struct ent_distortion *distortion,
                           ent_real *drive)
{
        ent_distortion_lock(distortion);
        *drive = distortion->drive;
        ent_distortion_unlock(distortion);
        return ENTROPICTRON_OK;
}
