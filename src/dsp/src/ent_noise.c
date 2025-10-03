/**
 * File name: ent_noise.c
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

#include "ent_noise.h"

#include "qx_randomizer.h"

struct ent_noise {
	bool enabled;
        enum ent_noise_type type;
        float density;
        float brightness;
        float gain;
        struct qx_randomizer randomizer;
};

struct ent_noise* ent_noise_create()
{
}

void ent_noise_free(struct ent_noise **noise)
{
}

enum ent_error ent_noise_enable(struct ent_noise *noise, bool b)
{
        noise->enabled = b;
        return ENT_OK;
}

bool ent_noise_is_enabled(struct ent_noise *noise)
{
        return noise->enabled;
}

enum ent_error ent_noise_set_type(struct ent_noise *noise,
                                  enum ent_noise_type type)
{
        noise->type = type;
        return ENT_OK;
}

enum ent_noise_type ent_noise_get_type(struct ent_noise *noise)
{
        return noise->type;
}

enum ent_error ent_noise_set_density(struct ent_noise *noise, float density)
{
        noise->density = density;
        return ENT_OK;
}

float ent_noise_get_density(struct ent_noise *noise)
{
        return noise->density;
}

enum ent_error ent_noise_set_brightness(struct ent_noise *noise, float brightness)
{
        noise->brightness = brightness;
        return ENT_OK;
}

float ent_noise_get_brightness(struct ent_noise *noise)
{
        return noise->brightness;
}

enum ent_error ent_noise_set_gain(struct ent_noise *noise, float gain)
{
        noise->gain = gain;
        return ENT_OK;
}

float ent_noise_get_gain(struct ent_noise *noise)
{
        return noise->gain;
}

void ent_noise_process(struct ent_noise *noise,
                       float *data,
                       size_t size)
{

}

