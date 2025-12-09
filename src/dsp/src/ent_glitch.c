/**
 * File name: ent_glitch.c
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

#include "ent_glitch.h"
#include "ent_log.h"
#include "qx_math.h"
#include "qx_randomizer.h"

#include <stdlib.h>

struct ent_glitch {
        int sample_rate;
        bool enabled;

        float probability;
        int jump_min_samples;
        int jump_max_samples;
        int glitch_length_samples;
        int glitch_repeat_count;

        float *buffer[2];
        size_t buffer_size;
        size_t write_pos;

        int glitch_pos;
        int glitch_count;
        int glitch_play_pos;

        struct qx_randomizer prob_randomizer;
        struct qx_randomizer randomizer;
};

struct ent_glitch* ent_glitch_create(int sample_rate)
{
        struct ent_glitch* g = calloc(1, sizeof(struct ent_glitch));
        if (!g) return NULL;

        g->sample_rate = sample_rate;
        g->enabled = false;
        g->probability = 0.25f;
        g->jump_min_samples = 0; // 0ms;
        g->jump_max_samples = sample_rate * (200.0f / 1000.0f); // 200ms
        g->glitch_length_samples = sample_rate *  (50.0f / 1000.0f); // 40 ms
        g->glitch_repeat_count = 3;

        g->buffer_size = sample_rate;
        g->buffer[0] = calloc(g->buffer_size * 2, sizeof(float)); // stereo
        g->buffer[1] = calloc(g->buffer_size * 2, sizeof(float)); // stereo
        g->write_pos = 0;
        g->glitch_pos = -1;
        g->glitch_count = 0;
        g->glitch_play_pos = 0;

        qx_randomizer_init(&g->prob_randomizer, 0.0f, 1.0f, 1.0f / 65536.0f);
        qx_randomizer_init(&g->randomizer, 0.0f, 1.0f, 1.0f / 65536.0f);

        return g;
}

void ent_glitch_free(struct ent_glitch **g)
{
        if (g && *g) {
                free((*g)->buffer[0]);
                free((*g)->buffer[1]);
                free(*g);
                *g = NULL;
        }
}

enum ent_error ent_glitch_enable(struct ent_glitch *g, bool b)
{
        g->enabled = b;
        return ENT_OK;
}

bool ent_glitch_is_enabled(struct ent_glitch *g)
{
        return g->enabled;
}

enum ent_error ent_glitch_set_probability(struct ent_glitch *g, float probability)
{
        ent_log_info("PROBABILITY: %f", probability);
        g->probability = qx_clamp_float(probability, 0.1f, 1.0f);
        return ENT_OK;
}

float ent_glitch_get_probability(struct ent_glitch *g)
{
        return g->probability;
}

enum ent_error ent_glitch_set_jump_min(struct ent_glitch *g, float jump_min_ms)
{
        ent_log_info("JUMP MIN: %f", jump_min_ms);
        jump_min_ms = qx_clamp_float(jump_min_ms, 0.0f, 2000.0f);
        g->jump_min_samples = (int)(jump_min_ms * g->sample_rate / 1000.0f);
        return ENT_OK;
}

float ent_glitch_get_jump_min(struct ent_glitch *g)
{
        return (float)g->jump_min_samples * 1000.0f / g->sample_rate;
}

enum ent_error ent_glitch_set_jump_max(struct ent_glitch *g, float jump_max_ms)
{
        ent_log_info("JUMP MAX: %f", jump_max_ms);
        jump_max_ms = qx_clamp_float(jump_max_ms, 0.0f, 2000.0f);
        g->jump_max_samples = (int)(jump_max_ms * g->sample_rate / 1000.0f);
        return ENT_OK;
}

float ent_glitch_get_jump_max(struct ent_glitch *g)
{
        return (float)g->jump_max_samples * 1000.0f / g->sample_rate;
}

enum ent_error ent_glitch_set_length(struct ent_glitch *g, float length_ms)
{
        ent_log_info("JUMP MIN: %f", length_ms);
        length_ms = qx_clamp_float(length_ms, 0.0f, 2000.0f);
        g->glitch_length_samples = (int)(length_ms * g->sample_rate / 1000.0f);
        return ENT_OK;
}

float ent_glitch_get_length(struct ent_glitch *g)
{
        return (float)g->glitch_length_samples * 1000.0f / g->sample_rate;
}

enum ent_error ent_glitch_set_repeat_count(struct ent_glitch *g, int repeats)
{
        ent_log_info("REPEATS: %d", repeats);
        g->glitch_repeat_count = QX_CLAMP(repeats, 1, 10);
        ent_log_error("glitch_repeat_count: %f", g->glitch_repeat_count);
        return ENT_OK;
}

int ent_glitch_get_repeat_count(struct ent_glitch *g)
{
        return g->glitch_repeat_count;
}

void ent_glitch_process(struct ent_glitch *g,
                          float **in,
                          float **out,
                          size_t size)
{
        for (size_t i = 0; i < size; i++) {
                out[0][i] = in[0][i];
                out[1][i] = in[1][i];
                g->buffer[0][g->write_pos] = in[0][i];
                g->buffer[1][g->write_pos] = in[1][i];
                if (g->glitch_count > 0 && g->glitch_pos >= 0) {
                        int playback_index = (g->glitch_pos + g->glitch_play_pos) % g->buffer_size;
                        out[0][i] = g->buffer[0][playback_index];
                        out[1][i] = g->buffer[1][playback_index];
                        g->glitch_play_pos++;
                        g->glitch_count--;
                } else if (qx_randomizer_get_float(&g->prob_randomizer) < g->probability) {
                        int jump_range = g->jump_max_samples - g->jump_min_samples;
                        float jump_prob = qx_randomizer_get_float(&g->randomizer);
                        int jump = g->jump_min_samples + jump_prob * jump_range;
                        g->glitch_pos = (g->write_pos - jump) % g->buffer_size;
                        g->glitch_count = g->glitch_length_samples * g->glitch_repeat_count;
                        g->glitch_play_pos = 0;
                }

                g->write_pos = (g->write_pos + 1) % g->buffer_size;
        }
}
