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

#include "ent_shelf_filter.h"
#include <math.h>

#include "ent_log.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846  // π – the magical circle constant
#endif

// Initialize high-shelf filter with linear gain
void ent_shelf_filter_init(struct ent_shelf_filter* filter,
                           float sample_rate,
                           float cut_off,
                           float gain)
{
        filter->gain = gain;
        float A = powf(10.0f, gain / 40.0f);
        float w0 = 2.0f * M_PI * cut_off / sample_rate;
        float cosw0 = cosf(w0);
        float sinw0 = sinf(w0);
        float alpha = sinw0 / 2.0f * sqrtf((A + 1/A) * 1.0f + 2.0f); // shelf factor = 1

        float b0 =    A * ((A+1) + (A-1)*cosw0 + 2*sqrtf(A)*alpha);
        float b1 = -2*A * ((A-1) + (A+1)*cosw0);
        float b2 =    A * ((A+1) + (A-1)*cosw0 - 2*sqrtf(A)*alpha);
        float a0 =        (A+1) - (A-1)*cosw0 + 2*sqrtf(A)*alpha;
        float a1 =  2 * ((A-1) - (A+1)*cosw0);
        float a2 =        (A+1) - (A-1)*cosw0 - 2*sqrtf(A)*alpha;

        // Normalize coefficients
        filter->b0 = b0 / a0;
        filter->b1 = b1 / a0;
        filter->b2 = b2 / a0;
        filter->a1 = a1 / a0;
        filter->a2 = a2 / a0;

        // Clear Direct Form II Transposed state
        filter->z1 = 0.0f;
        filter->z2 = 0.0f;
}

// Dynamically update cutoff and gain without resetting state
void ent_shelf_filter_set_cutoff(struct ent_shelf_filter* filter,
                                 float sample_rate,
                                 float cut_off,
                                 float gain) // linear gain
{
        filter->gain = gain;
        float A = powf(10.0f, gain / 40.0f);
        float w0 = 2.0f * M_PI * cut_off / sample_rate;
        float cosw0 = cosf(w0);
        float sinw0 = sinf(w0);
        float alpha = sinw0 / 2.0f * sqrtf((A + 1/A) * 1.0f + 2.0f);

        float b0 =    A * ((A+1) + (A-1)*cosw0 + 2*sqrtf(A)*alpha);
        float b1 = -2*A * ((A-1) + (A+1)*cosw0);
        float b2 =    A * ((A+1) + (A-1)*cosw0 - 2*sqrtf(A)*alpha);
        float a0 =        (A+1) - (A-1)*cosw0 + 2*sqrtf(A)*alpha;
        float a1 =  2 * ((A-1) - (A+1)*cosw0);
        float a2 =        (A+1) - (A-1)*cosw0 - 2*sqrtf(A)*alpha;

        // Normalize coefficients
        filter->b0 = b0 / a0;
        filter->b1 = b1 / a0;
        filter->b2 = b2 / a0;
        filter->a1 = a1 / a0;
        filter->a2 = a2 / a0;
}

// Process buffer with Direct Form II Transposed
void ent_shelf_filter_process(struct ent_shelf_filter* filter,
                              float *data,
                              size_t size)
{
        for (size_t i = 0; i < size; ++i) {
                float in = data[i];

                float out = filter->b0 * in + filter->z1;
                filter->z1 = filter->b1 * in - filter->a1 * out + filter->z2;
                filter->z2 = filter->b2 * in - filter->a2 * out;

                data[i] = out;
        }
}

