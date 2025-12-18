/**
 * File name: entropictron.c
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

#include "entropictron.h"
#include "ent_noise.h"
#include "ent_crackle.h"
#include "ent_glitch.h"
#include "ent_log.h"

#include "qx_math.h"

struct entropictron {
	unsigned int sample_rate;
        enum ent_play_mode play_mode;
        bool is_playing;
        struct ent_noise* noise[2];
        struct ent_crackle *crackle[2];
        struct ent_glitch *glitch[2];
};

enum ent_error
ent_create(struct entropictron **ent, unsigned int sample_rate)
{
	if (ent == NULL)
		return ENT_ERROR;

	*ent = calloc(1, sizeof(struct entropictron));
	if (*ent == NULL)
		return ENT_ERROR_MEM_ALLOC;

	(*ent)->sample_rate = sample_rate;
        (*ent)->is_playing = false;
        (*ent)->play_mode = ENT_PLAY_MODE_PLAYBACK;

        // Create noise
        size_t num_noises = QX_ARRAY_SIZE((*ent)->noise);
        for (size_t i = 0; i < num_noises; i++) {
                (*ent)->noise[i] = ent_noise_create(sample_rate);
                if ((*ent)->noise[i] == NULL) {
                        ent_log_error("can't create noise");
                        ent_free(ent);
                        return ENT_ERROR;
                }
        }

        // Create crackle
        size_t num_crackles = QX_ARRAY_SIZE((*ent)->crackle);
        for (size_t i = 0; i < num_crackles; i++) {
                (*ent)->crackle[i] = ent_crackle_create(sample_rate);
                if ((*ent)->crackle[i] == NULL) {
                        ent_log_error("can't create crackle");
                        ent_free(ent);
                        return ENT_ERROR;
                }
        }

        // Create glitch
        size_t num_glitchs = QX_ARRAY_SIZE((*ent)->glitch);
        for (size_t i = 0; i < num_glitchs; i++) {
                (*ent)->glitch[i] = ent_glitch_create(sample_rate);
                if ((*ent)->glitch[i] == NULL) {
                        ent_log_error("can't create glitch");
                        ent_free(ent);
                        return ENT_ERROR;
                }
        }

	return ENT_OK;
}

void ent_free(struct entropictron **ent)
{
        if (ent != NULL && *ent != NULL) {
                // Free noise
                size_t num_noises = QX_ARRAY_SIZE((*ent)->noise);
                for (size_t i = 0; i < num_noises; i++)
                        ent_noise_free(&(*ent)->noise[i]);

                // Free crackle
                size_t num_crackles = QX_ARRAY_SIZE((*ent)->crackle);
                for (size_t i = 0; i < num_crackles; i++)
                        ent_crackle_free(&(*ent)->crackle[i]);

                // Free glitch
                size_t num_glitchs = QX_ARRAY_SIZE((*ent)->glitch);
                for (size_t i = 0; i < num_glitchs; i++)
                        ent_glitch_free(&(*ent)->glitch[i]);

                free(*ent);
                *ent = NULL;
        }
}

enum ent_error
ent_set_sample_rate(struct entropictron *ent, unsigned int rate)
{
        if (ent == NULL) {
                ent_log_error("wrong arguments");
                return ENT_ERROR;
        }

        ent->sample_rate = rate;
        return ENT_OK;
}

enum ent_error
ent_get_sample_rate(struct entropictron *ent, unsigned int *sample_rate)
{
        if (ent == NULL || sample_rate == NULL) {
                ent_log_error("wrong arguments");
                return ENT_ERROR;
        }

        *sample_rate = ent->sample_rate;
        return ENT_OK;
}

enum ent_error ent_set_play_mode(struct entropictron *ent, enum ent_play_mode mode)
{
        ent->play_mode = mode;
        if (ent->play_mode == ENT_PLAY_MODE_ON)
                ent->is_playing = true;
        else
                ent->is_playing = false;
        return ENT_OK;
}

enum ent_play_mode ent_get_play_mode(struct entropictron *ent)
{
        return ent->play_mode;
}

enum ent_error
ent_process(struct entropictron *ent, float** data, size_t size)
{
        if (!ent->is_playing)
                return ENT_OK;

        float *in[2] = {data[0], data[1]};
        float *out[2] = {data[2], data[3]};

        size_t n = QX_ARRAY_SIZE(ent->noise);
        for (size_t i = 0; i < n; i++) {
                struct ent_noise *noise = ent->noise[i];
                if (ent_noise_is_enabled(noise))
                        ent_noise_process(noise, out, size);
        }

        n = QX_ARRAY_SIZE(ent->crackle);
        for (size_t i = 0; i < n; i++) {
                struct ent_crackle *crackle = ent->crackle[i];
                if (ent_crackle_is_enabled(crackle))
                        ent_crackle_process(crackle, out, size);
        }

        n = QX_ARRAY_SIZE(ent->glitch);
        for (size_t i = 0; i < n; i++) {
                struct ent_glitch *glitch = ent->glitch[i];
                if (ent_glitch_is_enabled(glitch))
                        ent_glitch_process(glitch, in, out, size);
        }

        return ENT_OK;
}

void ent_get_state(struct entropictron *ent, struct ent_state *state)
{
        state->play_mode = ent->play_mode;

        size_t n = QX_ARRAY_SIZE(ent->noise);
        for (size_t i = 0; i < n; i++) {
                ent_noise_get_state(ent->noise[i], &state->noises[i]);

        n = QX_ARRAY_SIZE(ent->crackle);
        for (size_t i = 0; i < n; i++)
                ent_crackle_get_state(ent->crackle[i], &state->crackles[i]);

        n = QX_ARRAY_SIZE(ent->glitch);
        for (size_t i = 0; i < n; i++) {
                ent_glitch_get_state(ent->glitch[i], &state->glitches[i]);
}

void ent_press_key(struct entropictron *ent, bool on, int pitch, int velocity)
{
        ent->is_playing = on;
}

struct ent_noise*
ent_get_noise(struct entropictron *ent, int id)
{
        if (id > 1)
                return NULL;

        return ent->noise[id];
}

struct ent_crackle*
ent_get_crackle(struct entropictron *ent, int id)
{
        if (id > 1)
                return NULL;

        return ent->crackle[id];
}

struct ent_glitch*
ent_get_glitch(struct entropictron *ent, int id)
{
        if (id > 1)
                return NULL;

        return ent->glitch[id];
}
