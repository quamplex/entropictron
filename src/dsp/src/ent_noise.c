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
#include "ent_shelf_filter.h"
#include "ent_filter.h"

#include "qx_math.h"
#include "qx_randomizer.h"
#include "qx_fader.h"

struct ent_noise {
        int sample_rate;
	bool enabled;
        enum ent_noise_type type;
        float density;
        float brightness;
        float gain;
        float stereo;
        int sample_channel;
        struct qx_randomizer prob_randomizer;
        struct qx_randomizer randomizer;
        struct qx_randomizer stereo_randomizer;
        struct qx_fader fader;

        // Pink noise filter states
        float b0, b1, b2;

        // Brown noise state
        float brown;

        // Brightness shelf filter
        struct ent_shelf_filter sh_filter_l;
        struct ent_shelf_filter sh_filter_r;

        // Low, band, high pass filter
        bool filter_enabled;
        struct ent_filter filter;

        float buffer[2][4096];
};

struct ent_noise* ent_noise_create(int sample_rate)
{
        struct ent_noise* noise = calloc(1, sizeof(struct ent_noise));
        if (!noise)
                return NULL;

        noise->sample_rate = sample_rate;
        noise->enabled = false;
        noise->type = ENT_NOISE_TYPE_WHITE;
        noise->density = 1.0f;
        noise->brightness = 0.0f;
        noise->gain = 1.0f;
        noise->stereo = 0.0f;
        noise->sample_channel = 0;
        noise->brown = 0.0f;
        noise->b0 = 0.0f;
        noise->b1 = 0.0f;
        noise->b2 = 0.0f;
        noise->filter_enabled = false;

        qx_randomizer_init(&noise->prob_randomizer, -1.0f, 1.0f, 1.0f / 65536.0f);
        qx_randomizer_init(&noise->randomizer, -1.0f, 1.0f, 1.0f / 65536.0f);
        qx_randomizer_init(&noise->stereo_randomizer, 0.0f, 1.0f, 1.0f / 65536.0f);

        qx_fader_init(&noise->fader, 50.0f, noise->sample_rate);

        ent_shelf_filter_init(&noise->sh_filter_l, noise->sample_rate, 4000.0f, noise->gain);
        ent_shelf_filter_init(&noise->sh_filter_r, noise->sample_rate, 4000.0f, noise->gain);

        ent_filter_init(&noise->filter, noise->sample_rate, 18000.0f, 1.0f);

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
                noise->b0 = 0.0f;
                noise->b1 = 0.0f;
                noise->b2 = 0.0f;
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
        ent_log_info("DENSITY: %f", density);
        noise->density = qx_clamp_float(density, 0.0f, 1.0f);
        return ENT_OK;
}

float ent_noise_get_density(struct ent_noise *noise)
{
        return noise->density;
}

enum ent_error ent_noise_set_brightness(struct ent_noise *noise, float brightness)
{
    ent_log_info("BRIGHTNESS: %f", brightness);
    noise->brightness = qx_clamp_float(brightness, 0.01f, 1.0f);

    float min_cutoff = 4000.0f;
    float max_cutoff = 8000.0f;
    float cutoff = min_cutoff + (max_cutoff - min_cutoff) * noise->brightness;

    float min_gain = 0.0f;
    float max_gain = 6.0f;
    float gain = min_gain + (max_gain - min_gain) *  brightness;

    // Update high-shelf filter for both channels
    ent_shelf_filter_set_cutoff(&noise->sh_filter_l, noise->sample_rate, cutoff, gain);
    ent_shelf_filter_set_cutoff(&noise->sh_filter_r, noise->sample_rate, cutoff, gain);

    return ENT_OK;
}

float ent_noise_get_brightness(struct ent_noise *noise)
{
        return noise->brightness;
}

enum ent_error ent_noise_set_gain(struct ent_noise *noise, float gain)
{
        ent_log_info("GAIN: %f db", gain);
        noise->gain = gain;
        return ENT_OK;
}

float ent_noise_get_gain(struct ent_noise *noise)
{
        return noise->gain;
}

enum ent_error ent_noise_set_stereo(struct ent_noise *noise, float stereo)
{
        ent_log_info("STEREO: %f", stereo);
        noise->stereo = qx_clamp_float(stereo, 0.0f, 1.0f);
        return ENT_OK;
}

float ent_noise_get_stereo(struct ent_noise *noise)
{
        return noise->stereo;
}

enum ent_error ent_noise_set_filter_type(struct ent_noise *noise, enum ent_filter_type type)
{
        ent_filter_set_type(&noise->filter, type);
        return ENT_OK;
}

enum ent_error
ent_noise_filter_enable(struct ent_noise *noise, bool enable)
{
        noise->filter_enabled = enable;
}

bool ent_noise_filter_is_enabled(struct ent_noise *noise)
{
        return noise->filter_enabled;
}

enum ent_filter_type ent_noise_get_filter_type(struct ent_noise *noise)
{
        return ent_filter_get_type(&noise->filter);
}

enum ent_error ent_noise_set_cutoff(struct ent_noise *noise, float cutoff)
{
        ent_log_info("CUTOFF: %f Hz", cutoff);
        cutoff = qx_clamp_float(cutoff, 20.0f, 18000.0f);
        ent_filter_set_cutoff(&noise->filter, cutoff);
        return ENT_OK;
}

float ent_noise_get_cutoff(struct ent_noise *noise)
{
        return ent_filter_get_cutoff(&noise->filter);
}

enum ent_error ent_noise_set_resonance(struct ent_noise *noise, float resonance)
{
        ent_log_info("RESOANCE: %f Hz", resonance);
        resonance = qx_clamp_float(resonance, 0.0f, 1.0f);
        ent_filter_set_resonance(&noise->filter, resonance);
        return ENT_OK;
}

float ent_noise_get_resonance(struct ent_noise *noise)
{
        return ent_filter_get_resonance(&noise->filter);
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
                if (prob <= threshold)
                        val = qx_randomizer_get_float(&noise->randomizer);

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

                // Calculate random stereo channel
                float rand_val = qx_randomizer_get_float(&noise->stereo_randomizer);
                float width = noise->stereo / 2.0f;
                if (rand_val < width) {
                        noise->buffer[0][i] = val;
                        noise->buffer[1][i] = 0.0f;
                } else if (rand_val > 1.0f - width) {
                        noise->buffer[0][i] = 0.0f;
                        noise->buffer[1][i] = val;
                } else {
                        noise->buffer[0][i] = val;
                        noise->buffer[1][i] = val;
                }
        }

        if (noise->brightness > 1.0e-6f) {
                ent_shelf_filter_process(&noise->sh_filter_l, noise->buffer[0], size);
                ent_shelf_filter_process(&noise->sh_filter_r, noise->buffer[1], size);

                float k = 1.0f / powf(10.0f, noise->sh_filter_r.gain / 20.0f);
                for (size_t i = 0; i < size; i++) {
                        noise->buffer[0][i] *= k;
                        noise->buffer[1][i] *= k;
                }
        }

        if (noise->filter_enabled) {
                float *buffer[2] = { noise->buffer[0], noise->buffer[1] };
                ent_filter_process(&noise->filter, buffer, size);
        }

        for (size_t i = 0; i < size; i++) {
                data[0][i] += noise->buffer[0][i] * noise->gain;
                data[1][i] += noise->buffer[1][i] * noise->gain;
        }
}

