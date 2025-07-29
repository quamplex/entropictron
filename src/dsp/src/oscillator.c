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

#include "oscillator.h"

struct ent_oscillator
*ent_osc_create(int sample_rate)
{
        struct ent_oscillator *osc;

        osc = (struct ent_oscillator*)calloc(1, sizeof(struct ent_oscillator));
        if (osc == NULL)
                return NULL;
        osc->state = ENTROPICTRON_OSC_STATE_ENABLED;
        osc->func = ENTROPICTRON_OSC_FUNC_SINE;
        osc->initial_phase = 0.0f;
        osc->phase = osc->initial_phase;
        osc->sample_rate = sample_rate;
        osc->amplitude = GKICK_OSC_DEFAULT_AMPLITUDE;
        osc->frequency = GKICK_OSC_DEFAULT_FREQUENCY;
        osc->pitch_shift = 0.0f;
        osc->noise_density = 1.0f;
        osc->env_number = 4;
        osc->brownian = 0;
        osc->is_fm = false;
        osc->fm_input = 0.0f;
        osc->fm_k = 1.0f;
        osc->seed = 100;
        osc->seedp = osc->seed;

        if (ent_osc_create_envelopes(osc) != ENTROPICTRON_OK) {
               ent_osc_free(&osc);
                return NULL;
        }

        if (ent_distortion_new(&osc->distortion, osc->sample_rate) != ENTROPICTRON_OK) {
                ent_log_error("can't create distortion effect");
                ent_osc_free(&osc);
		return NULL;
        }
        osc->distortion_enabled = false;

        if (ent_filter_new(&osc->filter, osc->sample_rate) != ENTROPICTRON_OK) {
                ent_log_error("can't create filter");
                ent_osc_free(&osc);
		return NULL;
        }
        osc->filter_enabled = false;
        return osc;
}

void
ent_osc_free(struct ent_oscillator **osc)
{
        if (osc == NULL || *osc == NULL)
                return;

        if ((*osc)->envelopes != NULL) {
                for (size_t i = 0; i < (*osc)->env_number; i++)
                        ent_envelope_destroy((*osc)->envelopes[i]);
                free((*osc)->envelopes);
                ent_distortion_free(&(*osc)->distortion);
                ent_filter_free(&(*osc)->filter);
                ent_buffer_free(&(*osc)->sample);
        }

        free(*osc);
        *osc = NULL;
}

void
ent_osc_set_state(struct ent_oscillator *osc,
                         enum entropictron_osc_state state)
{
        osc->state = state;
}

enum entropictron_error
ent_osc_create_envelopes(struct ent_oscillator *osc)
{
        if (osc->env_number < 1)
                return ENTROPICTRON_ERROR;

        osc->envelopes = (struct ent_envelope**)calloc(1, sizeof(struct ent_envelope*) *osc->env_number);
        if (osc->envelopes == NULL)
                return ENTROPICTRON_ERROR_MEM_ALLOC;

        for (size_t i = 0; i < osc->env_number; i++) {
                struct ent_envelope *env = ent_envelope_create();
                if (env == NULL) {
                        return ENTROPICTRON_ERROR;
                } else {
                        /* Add two default points. */
                        if (i == GKICK_OSC_PITCH_SHIFT_ENVELOPE) {
                                struct ent_envelope_point_info info = {0.0f, 0.5f, false};
                                ent_envelope_add_point(env, &info);
                                info.x = 1.0f;
                                ent_envelope_add_point(env, &info);
                        } else {
                                struct ent_envelope_point_info info = {0.0f, 1.0f, false};
                                ent_envelope_add_point(env, &info);
                                info.x = 1.0f;
                                ent_envelope_add_point(env, &info);
                        }
                        osc->envelopes[i] = env;
                }
        }

        return ENTROPICTRON_OK;
}

struct ent_envelope*
ent_osc_get_envelope(struct ent_oscillator *osc,
                       size_t env_index)
{
        if (osc == NULL)
                return NULL;

        switch (env_index) {
        case ENTROPICTRON_FILTER_CUTOFF_ENVELOPE:
                return osc->filter->cutoff_env;
	case ENTROPICTRON_FILTER_Q_ENVELOPE:
                return osc->filter->q_env;
        case ENTROPICTRON_DISTORTION_DRIVE_ENVELOPE:
                return osc->distortion->drive_env;
        case ENTROPICTRON_AMPLITUDE_ENVELOPE:
                return osc->envelopes[GKICK_OSC_AMPLITUDE_ENVELOPE];
        case ENTROPICTRON_FREQUENCY_ENVELOPE:
                return osc->envelopes[GKICK_OSC_FREQUENCY_ENVELOPE];
        case ENTROPICTRON_PITCH_SHIFT_ENVELOPE:
                return osc->envelopes[GKICK_OSC_PITCH_SHIFT_ENVELOPE];
        case ENTROPICTRON_NOISE_DENSITY_ENVELOPE:
                return osc->envelopes[GKICK_OSC_NOISE_DENSITY_ENVELOPE];
        default:
                return NULL;
        }
}

void ent_osc_increment_phase(struct ent_oscillator *osc,
                               ent_real t,
			       ent_real kick_len)
{
	struct ent_envelope *frequency_envelope = osc->envelopes[GKICK_OSC_FREQUENCY_ENVELOPE];
        ent_real env_val = ent_envelope_get_value(frequency_envelope, t / kick_len);
	enum ent_envelope_apply_type apply_type = ent_envelope_get_apply_type(frequency_envelope);
	ent_real f;
	if (apply_type == ENTROPICTRON_ENVELOPE_APPLY_LOGARITHMIC)
		f = pow(10, (log10(osc->frequency) - GKICK_LOG20) * env_val + GKICK_LOG20);
	else
		f = osc->frequency * env_val;
        f += f * osc->fm_k * osc->fm_input;
        osc->phase += (GKICK_2PI * f) / (osc->sample_rate);
        if (osc->phase > GKICK_2PI)
	          osc->phase -= GKICK_2PI;
}

ent_real ent_osc_value(struct ent_oscillator *osc,
		       ent_real t,
		       ent_real kick_len)
{
        ent_real amp;
        ent_real v;
        ent_real env_x;
#ifdef ENTROPICTRON_OS_WINDOWS
        // Use rand & srand since the calls are only from a single thread.
        if (t == 0.0)
                srand(osc->seed);
#endif // ENTROPICTRON_OS_WINDOWS

        // Calculate the x coordinate between 0 and 1.0 for the envelope.
        env_x = t / kick_len;
        amp = osc->amplitude * ent_envelope_get_value(osc->envelopes[GKICK_OSC_AMPLITUDE_ENVELOPE], env_x);
        v = 0.0f;
        switch (osc->func) {
        case ENTROPICTRON_OSC_FUNC_SINE:
                v = amp * ent_osc_func_sine(osc->phase);
                break;
        case ENTROPICTRON_OSC_FUNC_SQUARE:
                v = amp * ent_osc_func_square(osc->phase);
                break;
        case ENTROPICTRON_OSC_FUNC_TRIANGLE:
                v = amp * ent_osc_func_triangle(osc->phase);
                break;
        case ENTROPICTRON_OSC_FUNC_SAWTOOTH:
                v = amp * ent_osc_func_sawtooth(osc->phase);
                break;
        case ENTROPICTRON_OSC_FUNC_NOISE_WHITE:
                v = amp * ent_osc_func_noise_white(&osc->seedp,
                                                     ent_osc_get_noise_density(osc, t, kick_len));
                break;
        case ENTROPICTRON_OSC_FUNC_NOISE_PINK:
                v = amp * ent_osc_func_noise_pink();
                break;
        case ENTROPICTRON_OSC_FUNC_NOISE_BROWNIAN:
                v = amp * ent_osc_func_noise_brownian(&(osc)->brownian,
                                                        &osc->seedp,
                                                        ent_osc_get_noise_density(osc, t, kick_len));
                break;
        case ENTROPICTRON_OSC_FUNC_SAMPLE:
                if (osc->sample != NULL
                    && (t > (0.25f * osc->initial_phase / M_PI) * kick_len)) {
                        struct ent_envelope *env = osc->envelopes[GKICK_OSC_PITCH_SHIFT_ENVELOPE];
                        float env_val = ent_envelope_get_value(env, env_x);
                        float pitch_shift = 2.0f * (env_val - 0.5f) * osc->pitch_shift;
                        v = amp * ent_osc_func_sample(osc->sample, pitch_shift);
                }
                break;
        default:
                v = amp * ent_osc_func_sine(osc->phase);
        };

        if (osc->distortion->enabled)
                ent_distortion_val(osc->distortion, v, &v, env_x);

        if (osc->filter_enabled)
                ent_filter_val(osc->filter, v, &v, env_x);

        return v;
}

ent_real
ent_osc_func_sine(ent_real phase)
{
        return sin(phase);
}

ent_real
ent_osc_func_square(ent_real phase)
{
        if (phase < M_PI)
                return -1.0f;
        else
                return 1.0f;
}

ent_real
ent_osc_func_triangle(ent_real phase)
{
	if (phase < M_PI)
		return -1.0f + M_2_PI * phase;
	else
		return 3.0f - M_2_PI * phase;
}

ent_real
ent_osc_func_sawtooth(ent_real phase)
{
        if (phase < M_PI)
                return M_1_PI * phase;
        else
                return M_1_PI * phase - 2.0f;
}

ent_real ent_osc_func_noise_white(unsigned int *seed, unsigned int density)
{
        ent_real result = 0.0f;
        if (density >=1 && !(entropictron_rand(seed) % (GKICK_MAX_NOISE_DENSITY + 1 - density)))
                result = 2.0f * ((ent_real)(entropictron_rand(seed) % RAND_MAX)) / (ent_real)RAND_MAX - 1.0f;
        return result;
}

ent_real ent_osc_func_noise_pink(void)
{
        return 0.0f;
}

ent_real
ent_osc_func_noise_brownian(ent_real *previous,
                              unsigned int *seed,
                              unsigned int density)
{
        ent_real sign = 1.0f;
        ent_real walk;
        if (entropictron_rand(seed) % 2)
                sign = -1.0f;

        if (density >=1 && !(entropictron_rand(seed) % (GKICK_MAX_NOISE_DENSITY + 1 - density)))
                walk = sign * 0.1f * (((ent_real)(entropictron_rand(seed) % RAND_MAX)) / (ent_real)RAND_MAX);
        else
                walk = 0.0f;
        if (*previous + walk > 1.0f || *previous + walk < -1.0f)
                *previous -= walk;
        else
                *previous += walk;
        return *previous;
}

ent_real
ent_osc_func_sample(struct ent_buffer *sample,
                      float semitones)
{
        float factor = exp2f(semitones / 12.0f);
        return ent_buffer_stretch_get_next(sample, factor);
}

void
ent_osc_get_envelope_points(struct ent_oscillator *osc,
			      size_t env_index,
			      struct ent_envelope_point_info **buff,
			      size_t *npoints)
{
        if (buff != NULL)
                *buff = NULL;
        struct ent_envelope *env = ent_osc_get_envelope(osc, env_index);
        if (env != NULL)
                ent_envelope_get_points(env, buff, npoints);
}

void
ent_osc_set_envelope_points(struct ent_oscillator *osc,
			      size_t env_index,
			      const struct ent_envelope_point_info *buff,
			      size_t npoints)
{
        if (buff == NULL)
                return;

        struct ent_envelope *env = ent_osc_get_envelope(osc, env_index);
        if (env != NULL)
                ent_envelope_set_points(env, buff, npoints);
}

int
ent_osc_enabled(struct ent_oscillator *osc)
{
        return osc != NULL && (osc->state == ENTROPICTRON_OSC_STATE_ENABLED);
}

unsigned int
ent_osc_get_noise_density(struct ent_oscillator *osc, ent_real t, ent_real kick_len)
{
        struct ent_envelope *density_envelope = osc->envelopes[GKICK_OSC_NOISE_DENSITY_ENVELOPE];
        ent_real env_val = ent_envelope_get_value(density_envelope, t / kick_len);
        return GKICK_MAX_NOISE_DENSITY * (osc->noise_density * env_val);
}
