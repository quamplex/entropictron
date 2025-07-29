/**
 * File name: ent_buffer.h
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

#ifndef GKICK_BUFFER_H
#define GKICK_BUFFER_H

#include "entropictron_internal.h"

struct ent_buffer {
        ent_real *buff;

        /**
         * Real allocated fixed size, alwayse bigger than current size.
         * Never changes during the life-time of the buffer.
         */
        size_t max_size;

        /**
         * Current position in the buffer.
         */
        size_t currentIndex;
        ent_real floatIndex;

        /**
         * Current size of the buffer. Can change during
         * the life-time of the buffer.
         */
        size_t size;
};

void
ent_buffer_new(struct ent_buffer **buffer, int size);

void
ent_buffer_free(struct ent_buffer **buffer);

void
ent_buffer_reset(struct ent_buffer *buffer);

void
ent_buffer_set_data(struct ent_buffer *buffer,
                      const ent_real *data,
                      size_t size);

void
ent_buffer_set_at(struct ent_buffer *buffer,
                    size_t index,
                    ent_real val);

ent_real
ent_buffer_get_at(struct ent_buffer *buffer,
                    size_t index);

ent_real
ent_buffer_get_next(struct ent_buffer *buffer);

ent_real
ent_buffer_stretch_get_next(struct ent_buffer *buffer,
                                         ent_real factor);

void
ent_buffer_set_size(struct ent_buffer *buffer,
                      size_t size);

size_t
ent_buffer_size(struct ent_buffer *buffer);

size_t
ent_buffer_index(struct ent_buffer *buffer);

void
ent_buffer_push_back(struct ent_buffer *buffer,
                       ent_real val);

bool
ent_buffer_is_end(struct ent_buffer *buffer);

#endif // GKICK_BUFFER_H
