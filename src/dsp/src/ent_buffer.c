/**
 * File name: ent_buffer.c
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2018 Iurie Nistor
 *
 * This file is part of Geonkick.
 *
 * GeonKick is free software; you can redistribute it and/or modify
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

#include "ent_buffer.h"

void
ent_buffer_new(struct ent_buffer **buffer, int size)
{
        if (buffer == NULL || size < 1) {
                ent_log_error("wrong arguments");
                return;
        }

        *buffer = (struct ent_buffer*)malloc(sizeof(struct ent_buffer));
        if (*buffer == NULL) {
                ent_log_error("can't allocate memory");
                return;
        }
        (*buffer)->max_size = size;
        (*buffer)->size = (*buffer)->max_size;
        (*buffer)->currentIndex = 0;
        (*buffer)->floatIndex = 0.0f;

        (*buffer)->buff = (ent_real*)malloc(sizeof(ent_real) * (*buffer)->max_size);
        if ((*buffer)->buff == NULL) {
                ent_log_error("can't allocate memory");
                ent_buffer_free(buffer);
                return;
        }
}

void
ent_buffer_free(struct ent_buffer **buffer)
{
        if (buffer == NULL || *buffer == NULL)
                return;
        if ((*buffer)->buff != NULL)
                free((*buffer)->buff);
        free(*buffer);
        *buffer = NULL;
}

void
ent_buffer_reset(struct ent_buffer *buffer)
{
        buffer->currentIndex = 0;
        buffer->floatIndex = 0.0f;
}

void
ent_buffer_set_data(struct ent_buffer *buffer,
                           const ent_real *data,
                           size_t size)
{
        if (buffer == NULL)
                return;

        if (size > buffer->max_size)
                size = buffer->max_size;
        if (data != NULL && size > 0)
                memcpy(buffer->buff, data, sizeof(ent_real) * size);
        else
                memset(buffer->buff, 0, sizeof(ent_real) * buffer->max_size);
        buffer->size = size;
        buffer->currentIndex = buffer->size;
        buffer->floatIndex   = buffer->size;
}

void
ent_buffer_set_at(struct ent_buffer *buffer,
                         size_t index,
                         ent_real val)
{
        if (buffer != NULL && buffer->size > 0
            && index < buffer->size)
                buffer->buff[index] = val;
}

ent_real
ent_buffer_get_at(struct ent_buffer *buffer,
                               size_t index)
{
        if (buffer != NULL && buffer->size > 0
            && index < buffer->size)
                return buffer->buff[index];
        return 0.0;
}

ent_real
ent_buffer_get_next(struct ent_buffer *buffer)
{
        ent_real val = 0.0f;
        if (buffer->size > 0 && buffer->currentIndex < buffer->size) {
                val = buffer->buff[buffer->currentIndex++];
                buffer->floatIndex = buffer->currentIndex;
        }
        return val;
}

ent_real
ent_buffer_stretch_get_next(struct ent_buffer *buffer,
                              ent_real factor)
{
        if (buffer->size < 1) {
                return 0.0f;
        } else if (buffer->size == 1) {
                return buffer->buff[0];
        } else if (buffer->currentIndex >= 0 && buffer->currentIndex < buffer->size - 1) {
                ent_real d = buffer->floatIndex - buffer->currentIndex;
                ent_real val = (buffer->buff[buffer->currentIndex + 1] - buffer->buff[buffer->currentIndex]) * d
                        + buffer->buff[buffer->currentIndex];
                buffer->floatIndex += factor;
                buffer->currentIndex = (size_t)buffer->floatIndex;
                return val;
        } else if (buffer->currentIndex == buffer->size - 1) {
                float val = buffer->buff[buffer->currentIndex++];
                buffer->floatIndex = buffer->currentIndex;
                return val;
        }
        return 0.0f;
}

void
ent_buffer_set_size(struct ent_buffer *buffer,
                           size_t size)
{
        if (size < 1)
                buffer->size = 0;
        else if (size > buffer->max_size)
                buffer->size = buffer->max_size;
        else
                buffer->size = size;
        buffer->currentIndex = 0;
        buffer->floatIndex = 0.0f;
}

size_t
ent_buffer_index(struct ent_buffer *buffer)
{
        return buffer->currentIndex;
}

size_t
ent_buffer_size(struct ent_buffer *buffer)
{
        return buffer->size;
}

void
ent_buffer_push_back(struct ent_buffer *buffer,
                       ent_real val)
{
        if (buffer->size < 1 || buffer->currentIndex > buffer->size - 1)
                return;
        buffer->buff[buffer->currentIndex++] = val;
        buffer->floatIndex = buffer->currentIndex;
}

bool
ent_buffer_is_end(struct ent_buffer *buffer)
{
        return (buffer->size < 1) || (buffer->currentIndex > (buffer->size - 1));
}
