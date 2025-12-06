/**
 * File name: ent_filter.h
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

#ifndef ENT_FILTER_H
#define ENT_FILTER_H

#include "ent_defs.h"

enum ent_filter_type {
        ENT_FILTER_TYPE_LOWPASS,
        ENT_FILTER_TYPE_BANDPASS,
        ENT_FILTER_TYPE_HIGHPASS
};

struct ent_filter {
        enum ent_filter_type type;
        float cutoff;
        float resonance;
        int sample_rate;
        float ic1eq[2];
        float ic2eq[2];
        float g;
        float k;
        float denom;
        bool isnan_val;
};

void ent_filter_init(struct ent_filter* filter,
                     float sample_rate,
                     float cut_off,
                     float gain);

void ent_filter_set_type(struct ent_filter* filter,
                         enum ent_filter_type type);

enum ent_filter_type ent_filter_get_type(struct ent_filter* filter);

void ent_filter_set_cutoff(struct ent_filter* filter,
                           float cut_off);

float ent_filter_get_cutoff(struct ent_filter* filter);

void ent_filter_set_resonance(struct ent_filter* filter,
                           float cut_off);

float ent_filter_get_resonance(struct ent_filter* filter);

void ent_filter_process(struct ent_filter* filter,
                        float **data,
                        size_t size);

#endif // ENT_FILTER_H


