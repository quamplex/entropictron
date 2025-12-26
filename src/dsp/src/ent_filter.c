/**
 * File name: ent_shelf_filter.c
 * Project: Entropictron (A texture synthesizer)
 *
 * Copyright (C) 2025 Iurie Nistor
 *
 * This file is part of Entropictron.
 *
 * Entropictron is free software; you can redistribute it and/or modify
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

#include <math.h>
#include <string.h>
#include "ent_filter.h"
#include "ent_log.h"
#include "qx_math.h"

static inline float q_max_from_cutoff(float Qmin,
                                      float Qmax,
                                      float C,
                                      float fs)
{
        if (C <= 0.0f)
                return Qmax;
        if (C >= fs * 0.5f)
                return Qmin;

        float k = 5.0f;
        float c = C / (fs * 0.5f);
        float exp_k = expf(-k);
        float denom = 1.0f - exp_k;
        float num   = expf(-k * c) - exp_k;
        float Q = Qmin + (Qmax - Qmin) * (num / denom);

        return qx_clamp_float(Q, Qmin, Qmax);
}

static void ent_filter_update_coeffs(struct ent_filter* filter)
{
        filter->isnan_val = false;

        float Qmin = 0.1f;
        float Qmax = 5.0f;
        float Q = q_max_from_cutoff(Qmin,
                                    Qmin + filter->resonance * (Qmax - Qmin),
                                    filter->cutoff,
                                    filter->sample_rate);

        float fc = qx_clamp_float(filter->cutoff, 20.0f, 0.49f * filter->sample_rate);
        filter->g = tanf((float)M_PI * fc / filter->sample_rate);
        filter->k = 1.0f / Q;
        filter->denom = 1.0f + filter->g * (filter->g + filter->k);
}

void ent_filter_init(struct ent_filter* filter,
                     float sample_rate,
                     float cut_off,
                     float resonance)
{
        memset(filter, 0, sizeof(*filter));

        filter->sample_rate = sample_rate;
        filter->cutoff = cut_off;
        filter->resonance = resonance;
        filter->type = ENT_FILTER_TYPE_ALLPASS;
        filter->isnan_val = false;

        filter->ic1eq[0] = filter->ic1eq[1] = 0.0f;
        filter->ic2eq[0] = filter->ic2eq[1] = 0.0f;

        ent_filter_update_coeffs(filter);
}

void ent_filter_set_type(struct ent_filter* filter,
                         enum ent_filter_type type)
{
        filter->type = type;
}

enum ent_filter_type ent_filter_get_type(const struct ent_filter* filter)
{
        return filter->type;
}

void ent_filter_set_cutoff(struct ent_filter* filter,
                           float cut_off)
{
        filter->cutoff = cut_off;
        ent_filter_update_coeffs(filter);
}

float ent_filter_get_cutoff(const struct ent_filter* filter)
{
        return filter->cutoff;
}

void ent_filter_set_resonance(struct ent_filter* filter,
                              float resonance)
{
        filter->resonance = resonance;
        ent_filter_update_coeffs(filter);
}

float ent_filter_get_resonance(const struct ent_filter* filter)
{
        return filter->resonance;
}

void ent_filter_process(struct ent_filter* filter,
                        float **data,
                        size_t size)
{
        if (filter->type == ENT_FILTER_TYPE_ALLPASS)
                return;

        const float g = filter->g;
        const float k = filter->k;
        const float denom = filter->denom;

        float *L = data[0];
        float *R = data[1];

        for (size_t i = 0; i < size; i++) {
                /* Left channel */
                {
                        float v0 = L[i];

                        float v1 = (v0 - k * filter->ic1eq[0] - filter->ic2eq[0]) / denom;
                        float v2 = filter->ic1eq[0] + g * v1;
                        float v3 = filter->ic2eq[0] + g * v2;

                        filter->ic1eq[0] = 2.0f * v2 - filter->ic1eq[0];
                        filter->ic2eq[0] = 2.0f * v3 - filter->ic2eq[0];

                        switch (filter->type) {
                        case ENT_FILTER_TYPE_LOWPASS:  L[i] = v3;
                                break;
                        case ENT_FILTER_TYPE_BANDPASS: L[i] = v2;
                                break;
                        case ENT_FILTER_TYPE_HIGHPASS: L[i] = v0 - k * v2 - v3;
                                break;
                        default: L[i] = v3;
                                break;
                        }
                }

                /* Right channel */
                {
                        float v0 = R[i];

                        float v1 = (v0 - k * filter->ic1eq[1] - filter->ic2eq[1]) / denom;
                        float v2 = filter->ic1eq[1] + g * v1;
                        float v3 = filter->ic2eq[1] + g * v2;

                        filter->ic1eq[1] = 2.0f * v2 - filter->ic1eq[1];
                        filter->ic2eq[1] = 2.0f * v3 - filter->ic2eq[1];

                        switch (filter->type) {
                        case ENT_FILTER_TYPE_LOWPASS:  R[i] = v3;
                                break;
                        case ENT_FILTER_TYPE_BANDPASS: R[i] = v2;
                                break;
                        case ENT_FILTER_TYPE_HIGHPASS: R[i] = v0 - k * v2 - v3;
                                break;
                        default: R[i] = v3;
                                break;
                        }
                }
        }

        for (size_t i = 0; i < size; i++) {
                L[i] = qx_clamp_float(L[i], -1.0f, 1.0f);
                R[i] = qx_clamp_float(R[i], -1.0f, 1.0f);
        }
}

