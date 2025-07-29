/**
 * File name: worker.h
 * Project: Geonkick (A kick synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor 
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

#include "worker.h"

struct ent_worker *entropictron_worker = NULL;

bool
entropictron_worker_created()
{
        return entropictron_worker != NULL;
}

size_t
entropictron_worker_reference_count()
{
        if (entropictron_worker != NULL)
                return entropictron_worker->ref_count;
        return 0;
}

enum entropictron_error
entropictron_worker_create()
{
        /*        if (entropictron_worker != NULL)
                return ENTROPICTRON_OK;

        entropictron_worker = (struct ent_worker*)calloc(1, sizeof(struct ent_worker));
	if (entropictron_worker == NULL)
		return ENTROPICTRON_ERROR_MEM_ALLOC;

	entropictron_worker->running = false;
        if (pthread_cond_init(&entropictron_worker->condition_var, NULL) != 0) {
                ent_log_error("can't init worker condition variable");
		return ENTROPICTRON_ERROR;
	}
	entropictron_worker->cond_var_initilized = true;*/
	return ENTROPICTRON_OK;
}

enum entropictron_error
entropictron_worker_start()
{
        /*        if (!entropictron_worker)
                return ENTROPICTRON_ERROR;
        if (entropictron_worker->running)
                return ENTROPICTRON_OK;
        entropictron_worker->running = true;
        if (pthread_create(&entropictron_worker->thread, NULL,
                           &entropictron_worker_thread, NULL) != 0) {
                ent_log_error("can't create worker thread");
                entropictron_worker->running = false;
                return ENTROPICTRON_ERROR;
                }*/
        return ENTROPICTRON_OK;
}

void entropictron_worker_destroy()
{
	/*if (entropictron_worker->running)
		entropictron_worker->running = false;
        pthread_mutex_lock(&entropictron_worker->lock);
        pthread_cond_signal(&entropictron_worker->condition_var);
        pthread_mutex_unlock(&entropictron_worker->lock);
        ent_log_debug("join thread: %d", entropictron_worker->running);
	pthread_join(entropictron_worker->thread, NULL);

	pthread_mutex_lock(&entropictron_worker->lock);
	if (entropictron_worker->cond_var_initilized)
		pthread_cond_destroy(&entropictron_worker->condition_var);
	entropictron_worker->cond_var_initilized = false;
	pthread_mutex_unlock(&entropictron_worker->lock);
        free(entropictron_worker);
        entropictron_worker = NULL;*/
}

void entropictron_worker_add_instance(struct entropictron *instance)
{
        /*        if (entropictron_worker->ref_count < ENTROPICTRON_MAX_INSTANCES) {
                pthread_mutex_lock(&entropictron_worker->lock);
                instance->id = entropictron_worker->ref_count;
                entropictron_worker->instances[entropictron_worker->ref_count++] = instance;
                pthread_mutex_unlock(&entropictron_worker->lock);
                }*/
}

void entropictron_worker_remove_instance(struct entropictron *instance)
{
        /*        if (entropictron_worker->ref_count == 0)
                return;
        pthread_mutex_lock(&entropictron_worker->lock);
        if (entropictron_worker->ref_count) {
                entropictron_worker->instances[instance->id] =
                        entropictron_worker->instances[entropictron_worker->ref_count - 1];
                entropictron_worker->instances[instance->id]->id = instance->id;
        }
        entropictron_worker->instances[--entropictron_worker->ref_count] = NULL;
        pthread_mutex_unlock(&entropictron_worker->lock);*/
}

void *entropictron_worker_thread(void *arg)
{
}

void entropictron_worker_wakeup()
{
}
