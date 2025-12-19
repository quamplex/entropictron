/**
 * File name: ent_log.h
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

#ifndef ENT_LOG_H
#define ENT_LOG_H

#include <stdarg.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

void
ent_log_msg(const char *message, ...);

#ifdef ENT_LIB_LOG_LEVEL_TRACE
#define ENT_LIB_LOG_LEVEL_DEBUG
#ifdef ENT_LIB_LOG_FUNCTION
#define ent_log_trace(message, ...) \
  ent_log_msg("[TRACE][%s] " message, __func__, ##__VA_ARGS__)
#else
#define ent_log_trace(message, ...) ent_log_msg("[TRACE] " message, ##__VA_ARGS__)
#endif
#else
#define ent_log_trace(message, ...)
#endif /* ENT_LIB_LOG_LEVEL_TRACE */

#ifdef ENT_LIB_LOG_LEVEL_DEBUG
#define ENT_LIB_LOG_LEVEL_INFO
#ifdef ENT_LIB_LOG_FUNCTION
#define ent_log_debug(message, ...) \
  ent_log_msg("[DEBUG][%s] " message, __func__, ##__VA_ARGS__)
#else
#define ent_log_debug(message, ...) ent_log_msg("[DEBUG] " message, ##__VA_ARGS__)
  #endif
#else
#define ent_log_debug(message, ...)
#endif /* ENT_LIB_LOG_LEVEL_DEBUG */

#ifdef ENT_LIB_LOG_LEVEL_INFO
#define ENT_LIB_LOG_LEVEL_WARNING
#ifdef ENT_LIB_LOG_FUNCTION
#define ent_log_info(message, ...) \
  ent_log_msg("[INFO][%s] " message, __func__, ##__VA_ARGS__)
#else
#define ent_log_info(message, ...) ent_log_msg("[INFO] " message, ##__VA_ARGS__)
  #endif
#else
#define ent_log_info(message, ...)
#endif /* ENT_LIB_LOG_LEVEL_INFO */

#ifdef ENT_LIB_LOG_LEVEL_WARNING
  #define ENT_LIB_LOG_LEVEL_ERROR
#ifdef ENT_LIB_LOG_FUNCTION
#define ent_log_warning(message, ...) \
  ent_log_msg("[WARNING][%s] " message, __func__, ##__VA_ARGS__)
#else
#define ent_log_warning(message, ...) ent_log_msg("[WARNING] " message, ##__VA_ARGS__)
#endif
#else
#define ent_log_warning(message, ...)
#endif /* ENT_LIB_LOG_LEVEL_WARNING */

#ifdef ENT_LIB_LOG_LEVEL_ERROR
#define ENT_LIB_LOG_LEVEL_CRITICAL
#ifdef ENT_LIB_LOG_FUNCTION
#define ent_log_error(message, ...) \
  ent_log_msg("[ERROR][%s] " message, __func__, ##__VA_ARGS__)
#else
#define ent_log_error(message, ...) ent_log_msg("[ERROR] " message, ##__VA_ARGS__)
#endif
#else
#define ent_log_error(message, ...)
#endif /* ENT_LIB_LOG_LEVEL_ERROR */

#ifdef ENT_LIB_LOG_LEVEL_CRITICAL
#ifdef ENT_LIB_LOG_FUNCTION
#define ent_log_critical(message, ...) \
  ent_log_msg("[CRITICAL] " message, __func__, ##__VA_ARGS__)
#else
#define ent_log_critical(message, ...) \
  ent_log_msg("[CRITICAL] " message, ##__VA_ARGS__)
#endif
#else
#define ent_log_critical(message, ...)
#endif /* ENT_LIB_LOG_LEVEL_CRITICAL */

#ifdef __cplusplus
}
#endif
#endif // ENT_LOG_H
