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
#include "ent_log.h"

#include "qx_math.h"

enum ent_error
ent_create(struct entropictron **ent, unsigned int sample_rate)
{
	if (ent == NULL)
		return ENT_ERROR;

	*ent = calloc(1, sizeof(struct entropictron));
	if (*ent == NULL)
		return ENT_ERROR_MEM_ALLOC;

	(*ent)->sample_rate = sample_rate;

        // Create noise
        size_t num_noises = QX_ARRAY_SIZE((*ent)->noise);
        for (size_t i = 0; i < num_noises; i++) {
                (*ent)->noise[i] = ent_noise_create();
                if ((*ent)->noise[i] == NULL) {
                        ent_log_error("can't create noise");
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

enum ent_error
ent_process(struct entropictron *ent, float** data, size_t size)
{
        size_t num_noises = QX_ARRAY_SIZE(ent->noise);
        for (size_t i = 0; i < num_noises; i++) {
                struct ent_noise *noise = ent->noise[i];
                if (ent_noise_is_enabled(noise))
                        ent_noise_process(noise, data, size);
        }

        return ENT_OK;
}

struct ent_noise*
ent_get_noise(struct entropictron *ent, int id)
{
        if (id > 1)
                return NULL;

        return ent->noise[id];
}
