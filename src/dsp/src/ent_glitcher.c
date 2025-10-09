/**
 * File name: ent_glitcher.c
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

#include "ent_glitcher.h"
#include "ent_log.h"
#include "qx_randomizer.h"

#include <stdlib.h>

struct ent_glitcher {
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

struct ent_glitcher* ent_glitcher_create(int sample_rate)
{
        struct ent_glitcher* g = calloc(1, sizeof(struct ent_glitcher));
        if (!g) return NULL;

        g->sample_rate = sample_rate;
        g->enabled = true;
        g->probability = 0.1f;
        g->jump_min_samples = sample_rate / 20;
        g->jump_max_samples = sample_rate / 3;
        g->glitch_length_samples = sample_rate / 24; // 40 ms
        g->glitch_repeat_count = 2;

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

void ent_glitcher_free(struct ent_glitcher **g)
{
        if (g && *g) {
                free((*g)->buffer[0]);
                free((*g)->buffer[1]);
                free(*g);
                *g = NULL;
        }
}

enum ent_error ent_glitcher_enable(struct ent_glitcher *g, bool b)
{
        g->enabled = b;
        return ENT_OK;
}

bool ent_glitcher_is_enabled(struct ent_glitcher *g)
{
        return g->enabled;
}

enum ent_error ent_glitcher_set_probability(struct ent_glitcher *g, float probability)
{
        g->probability = probability;
        return ENT_OK;
}

float ent_glitcher_get_probability(struct ent_glitcher *g)
{
        return g->probability;
}

enum ent_error ent_glitcher_set_jump_min(struct ent_glitcher *g, float jump_min_ms)
{
        g->jump_min_samples = (int)(jump_min_ms * g->sample_rate / 1000.0f);
        return ENT_OK;
}

float ent_glitcher_get_jump_min(struct ent_glitcher *g)
{
        return (float)g->jump_min_samples * 1000.0f / g->sample_rate;
}

enum ent_error ent_glitcher_set_jump_max(struct ent_glitcher *g, float jump_max_ms)
{
        g->jump_max_samples = (int)(jump_max_ms * g->sample_rate / 1000.0f);
        return ENT_OK;
}

float ent_glitcher_get_jump_max(struct ent_glitcher *g)
{
        return (float)g->jump_max_samples * 1000.0f / g->sample_rate;
}

enum ent_error ent_glitcher_set_length(struct ent_glitcher *g, float length_ms)
{
        g->glitch_length_samples = (int)(length_ms * g->sample_rate / 1000.0f);
        return ENT_OK;
}

float ent_glitcher_get_length(struct ent_glitcher *g)
{
        return (float)g->glitch_length_samples * 1000.0f / g->sample_rate;
}

enum ent_error ent_glitcher_set_repeat_count(struct ent_glitcher *g, int repeats)
{
        g->glitch_repeat_count = repeats > 0 ? repeats : 1;
        ent_log_error("glitch_repeat_count: %f", g->glitch_repeat_count);
        return ENT_OK;
}

int ent_glitcher_get_repeat_count(struct ent_glitcher *g)
{
        return g->glitch_repeat_count;
}

void ent_glitcher_process(struct ent_glitcher *g,
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
