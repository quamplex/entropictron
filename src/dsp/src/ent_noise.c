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
#include "ent_log.h"

#include "qx_math.h"
#include "qx_randomizer.h"
#include "qx_fader.h"

struct ent_noise {
	bool enabled;
        enum ent_noise_type type;
        float density;
        float brightness;
        float gain;
        struct qx_randomizer prob_randomizer;
        struct qx_randomizer randomizer;
        struct qx_fader fader;

        // Pink noise filter states
        float b0, b1, b2;

        // Brown noise state
        float brown;
};

struct ent_noise* ent_noise_create(int sampler_rate)
{
        struct ent_noise* noise = calloc(1, sizeof(struct ent_noise));
        if (!noise)
                return NULL;

        noise->enabled = false;
        noise->type = ENT_NOISE_TYPE_WHITE;
        noise->density = 1.0f;
        noise->brightness = 1.0f;
        noise->gain = 1.0f;
        noise->brown = 0.0f;
        noise->b0 = noise->b1 = noise->b2 = 0.0f;
        qx_randomizer_init(&noise->prob_randomizer, -1.0f, 1.0f, 1.0f / 65536.0f);
        qx_randomizer_init(&noise->randomizer, -1.0f, 1.0f, 1.0f / 65536.0f);
        qx_fader_init(&noise->fader, 50, sampler_rate);

        return noise;
}

void ent_noise_free(struct ent_noise **noise)
{
        if (noise && *noise) {
                free(*noise);
                *noise = NULL;
        }
}

enum ent_error ent_noise_enable(struct ent_noise *noise, bool b)
{
        noise->enabled = b;
        qx_fader_enable(&noise->fader, b);
        return ENT_OK;
}

bool ent_noise_is_enabled(struct ent_noise *noise)
{
        return noise->enabled;
}

enum ent_error ent_noise_set_type(struct ent_noise *noise,
                                  enum ent_noise_type type)
{
        if (noise->type == type)
                return ENT_OK;

        noise->type = type;
        switch (noise->type) {
        case ENT_NOISE_TYPE_PINK:
                noise->b0 = noise->b1 = noise->b2 = 0.0f;
                break;
        case ENT_NOISE_TYPE_BROWN:
                noise->brown = 0.0f;
                break;
        default: // white
                break;
                }
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

inline static float ent_pink_noise(float white)
{
        static float b0, b1, b2;

        b0 = 0.99765f * b0 + 0.0990460f * white;
        b1 = 0.96300f * b1 + 0.2965164f * white;
        b2 = 0.57000f * b2 + 1.0526913f * white;

        float pink = b0 + b1 + b2 + 0.1848f * white;

        return pink * 0.05f;
}

static inline float pink_from_white(struct ent_noise *noise, float white)
{
    // Paul Kellet 3-pole filter
    noise->b0 = 0.99765f * noise->b0 + 0.0990460f * white;
    noise->b1 = 0.96300f * noise->b1 + 0.2965164f * white;
    noise->b2 = 0.57000f * noise->b2 + 1.0526913f * white;

    float pink = noise->b0 + noise->b1 + noise->b2 + 0.1848f * white;

    return pink * 0.05f;
}

static inline float brown_from_white(struct ent_noise *noise, float white)
{
        noise->brown += 0.02f * white;
        noise->brown = qx_clamp_float(noise->brown, -1.0f, 1.0f);
        return noise->brown * 0.5f;
}

void ent_noise_process(struct ent_noise *noise,
                       float **data,
                       size_t size)
{
        float threshold = 2.0f * noise->density - 1.0f;
        for (size_t i = 0; i < size; i++) {
                float val = 0.0f;
                float prob = qx_randomizer_get_float(&noise->prob_randomizer);
                if (prob <= threshold) {
                        val = qx_randomizer_get_float(&noise->randomizer);
                        val *= noise->gain;
                }

                switch (noise->type) {
                case ENT_NOISE_TYPE_PINK:
                        val = pink_from_white(noise, val);
                        break;
                case ENT_NOISE_TYPE_BROWN:
                        val = brown_from_white(noise, val);
                        break;
                default: // white
                        break;
                }

                val = qx_fader_fade(&noise->fader, val);

                data[0][i] += val;
                data[1][i] += val;
        }
}

