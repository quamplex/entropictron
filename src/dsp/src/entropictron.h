/**
 * File name: entropictron.h
 * Project: GeonKick (A kick synthesizer)
 *
 * Copyright (C) 2017 Iurie Nistor
 *
 * This file is part of GeonKick.
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

#ifndef ENTROPICTRON_H
#define ENTROPICTRON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <inttypes.h>
#include <float.h>
#include <stdbool.h>

#ifdef __FAST_MATH__
#error -ffast-math disables nan detection needed by entropictron
#endif

#ifdef __STDC_NO_ATOMICS__
#error atomic operations are not supported
#endif

#include "ent_log.h"

#ifdef ENTROPICTRON_DOUBLE_PRECISION
typedef double ent_real;
#else
typedef float ent_real;
#endif

#define ENTROPICTRON_UNUSED(expr) (void)expr

#define ENTROPICTRON_VERSION ENTROPICTRON_VERSION_HEX
#define ENTROPICTRON_VERSION_MAJOR ((ENTROPICTRON_VERSION_HEX >> 16) & 0xFF)
#define ENTROPICTRON_VERSION_MINOR ((ENTROPICTRON_VERSION_HEX >> 8) & 0xFF)
#define ENTROPICTRON_VERSION_PATCH (ENTROPICTRON_VERSION_HEX & 0xFF)
#define ENTROPICTRON_NAME "Geonkick"
#define ENTROPICTRON_APP_NAME "entropictron"
#define GEOKICK_VERSION_STRING ENTROPICTRON_VERSION_STR
#define ENTROPICTRON_VERSION_STRING ENTROPICTRON_VERSION_STR

#if defined(ENTROPICTRON_LIMITED_VERSION) && !defined(ENTROPICTRON_SINGLE)
#define ENTROPICTRON_SINGLE
#endif // ENTROPICTRON_LIMITED_VERSION

#ifdef ENTROPICTRON_AUDIO_JACK
#define GKICK_IS_STANDALONE (1)
#else
#define GKICK_IS_STANDALONE (0)
#endif

#define ENTROPICTRON_DEFAULT_SAMPLE_RATE 48000

enum entropictron_error {
  	ENTROPICTRON_OK                    = 0,
	ENTROPICTRON_ERROR                 = 1,
	ENTROPICTRON_ERROR_MEM_ALLOC       = 2,
	ENTROPICTRON_ERROR_WRONG_ARGUMENTS = 3
};

enum ent_key_state {
        GKICK_KEY_STATE_DEFAULT  = 0,
        GKICK_KEY_STATE_PRESSED  = 1,
        GKICK_KEY_STATE_RELEASED = 2
};

enum entropictron_envelope_type {
        ENTROPICTRON_AMPLITUDE_ENVELOPE         = 0,
        ENTROPICTRON_FREQUENCY_ENVELOPE         = 1,
        ENTROPICTRON_FILTER_CUTOFF_ENVELOPE     = 2,
	ENTROPICTRON_DISTORTION_DRIVE_ENVELOPE  = 3,
        ENTROPICTRON_DISTORTION_VOLUME_ENVELOPE = 4,
        ENTROPICTRON_PITCH_SHIFT_ENVELOPE       = 5,
	ENTROPICTRON_FILTER_Q_ENVELOPE          = 6,
        ENTROPICTRON_NOISE_DENSITY_ENVELOPE     = 7
};

enum ent_filter_type {
        ENTROPICTRON_FILTER_LOW_PASS  = 0,
        ENTROPICTRON_FILTER_HIGH_PASS = 1,
        ENTROPICTRON_FILTER_BAND_PASS = 2
};

enum entropictron_osc_func_type {
	ENTROPICTRON_OSC_FUNC_SINE           = 0,
	ENTROPICTRON_OSC_FUNC_SQUARE         = 1,
	ENTROPICTRON_OSC_FUNC_TRIANGLE       = 2,
	ENTROPICTRON_OSC_FUNC_SAWTOOTH       = 3,
        ENTROPICTRON_OSC_FUNC_NOISE_WHITE    = 4,
        ENTROPICTRON_OSC_FUNC_NOISE_PINK     = 5,
        ENTROPICTRON_OSC_FUNC_NOISE_BROWNIAN = 6,
        ENTROPICTRON_OSC_FUNC_SAMPLE         = 7
};

enum ENTROPICTRON_MODULE {
        ENTROPICTRON_MODULE_UNKNOWN = 0,
        ENTROPICTRON_MODULE_JACK    = 1
};

#ifdef ENTROPICTRON_LIMITED_VERSION
#define GKICK_OSC_GROUPS_NUMBER 1
#else
#define GKICK_OSC_GROUPS_NUMBER 3
#endif // ENTROPICTRON_LIMITED_VERSION
#define GKICK_OSC_GROUP_SIZE 3

enum entropictron_channel_type {
        ENTROPICTRON_CHANNEL_AUDIO_INPUT  = 0,
        ENTROPICTRON_CHANNEL_AUDIO_OUTPUT = 1,
        ENTROPICTRON_CHANNEL_MIDI_INPUT   = 2,
        ENTROPICTRON_CHANNEL_MIDI_OUTPUT  = 3
};

enum ent_envelope_apply_type {
	ENTROPICTRON_ENVELOPE_APPLY_LINEAR       = 0,
	ENTROPICTRON_ENVELOPE_APPLY_LOGARITHMIC  = 1
};

enum ent_envelope_curve_type {
	ENTROPICTRON_ENVELOPE_CURVE_LINEAR  = 0,
	ENTROPICTRON_ENVELOPE_CURVE_BEZIER  = 1
};

enum ent_distortion_type {
        ENTROPICTRON_DISTORTION_HARD_CLIPPING       = 0,
        ENTROPICTRON_DISTORTION_SOFT_CLIPPING_TANH  = 1,
        ENTROPICTRON_DISTORTION_ARCTAN              = 2,
        ENTROPICTRON_DISTORTION_EXPONENTIAL         = 3,
        ENTROPICTRON_DISTORTION_POLYNOMIAL          = 4,
        ENTROPICTRON_DISTORTION_LOGARITHMIC         = 5,
        ENTROPICTRON_DISTORTION_FOLDBACK            = 6,
        ENTROPICTRON_DISTORTION_HALF_WAVE_RECT      = 7,
        ENTROPICTRON_DISTORTION_FULL_WAVE_RECT      = 8,
        /* Backward compatible with old presets.
           In the next major release this to be removed. */
#if (ENTROPICTRON_VERSION_MAJOR > 3)
#warning Remove backward compatibility.
#endif // ENTROPICTRON_VERSION_MAJOR
        ENTROPICTRON_DISTORTION_BACKWARD_COMPATIBLE = 9
};

#ifdef ENTROPICTRON_SINGLE
#define ENTROPICTRON_MAX_INSTRUMENTS 1
#else
#define ENTROPICTRON_MAX_INSTRUMENTS 16
#endif

#define ENTROPICTRON_MAX_CHANNELS (ENTROPICTRON_MAX_INSTRUMENTS + 1)
#define ENTROPICTRON_AUDITION_CHANNEL_INDEX (ENTROPICTRON_MAX_CHANNELS - 1)

#define ENTROPICTRON_ANY_KEY -1
#define ENTROPICTRON_ANY_MIDI_CHANNEL -1
#define ENTROPICTRON_MAX_MIDI_CHANNELS 16

struct entropictron;

struct ent_envelope_point_info {
        ent_real x;
        ent_real y;
        bool control_point;
};

enum entropictron_error
entropictron_create(struct entropictron **kick, int sample_rate);

void
entropictron_free(struct entropictron **kick);

enum entropictron_error
entropictron_add_oscillator(struct entropictron* kick,
                        size_t *index);

enum entropictron_error
entropictron_remove_oscillator(struct entropictron* kick,
                           size_t index);

enum entropictron_error
entropictron_enable_oscillator(struct entropictron* kick,
                           size_t index);

enum entropictron_error
entropictron_disable_oscillator(struct entropictron* kick,
                            size_t index);

enum entropictron_error
entropictron_is_oscillator_enabled(struct entropictron* kick,
                               size_t index,
                               int *enabled);

enum entropictron_error
entropictron_get_oscillators_number(struct entropictron* kick,
                                size_t *number);

enum entropictron_error
entropictron_osc_envelope_get_points(struct entropictron *kick,
				 size_t osc_index,
				 size_t env_index,
				 struct ent_envelope_point_info **buf,
				 size_t *npoints);

enum entropictron_error
entropictron_osc_envelope_set_points(struct entropictron *kick,
                                 size_t osc_index,
                                 size_t env_index,
                                 const struct ent_envelope_point_info *buff,
                                 size_t npoints);

enum entropictron_error
entropictron_osc_envelope_add_point(struct entropictron *kick,
				size_t osc_index,
				size_t env_index,
                                struct ent_envelope_point_info *point_info);

enum entropictron_error
entropictron_osc_envelope_remove_point(struct entropictron *kick,
				   size_t osc_index,
				   size_t env_index,
				   size_t index);
enum entropictron_error
entropictron_osc_envelope_update_point(struct entropictron *kick,
				   size_t osc_index,
				   size_t env_index,
  				   size_t index,
                                   struct ent_envelope_point_info *point_info);

enum entropictron_error
entropictron_osc_envelope_set_apply_type(struct entropictron *kick,
				     size_t osc_index,
				     size_t env_index,
				     enum ent_envelope_apply_type apply_type);

enum entropictron_error
entropictron_osc_envelope_get_apply_type(struct entropictron *kick,
				     size_t osc_index,
				     size_t env_index,
				     enum ent_envelope_apply_type *apply_type);

enum entropictron_error
entropictron_osc_set_fm(struct entropictron *kick,
                    size_t index,
                    bool is_fm);

/* Used only for compatibility of older presets. */
enum entropictron_error
entropictron_osc_set_fm_k(struct entropictron *kick, size_t index,
                      ent_real k);

enum entropictron_error
entropictron_osc_is_fm(struct entropictron *kick,
                   size_t index,
                   bool *is_fm);

enum entropictron_error
entropictron_set_osc_function(struct entropictron *kick,
			  size_t osc_index,
			  enum entropictron_osc_func_type type);

enum entropictron_error
entropictron_get_osc_function(struct entropictron *kick,
			  size_t osc_index,
			  enum entropictron_osc_func_type *type);

enum entropictron_error
entropictron_set_osc_phase(struct entropictron *kick,
                       size_t osc_index,
                       ent_real phase);

enum entropictron_error
entropictron_get_osc_phase(struct entropictron *kick,
                       size_t osc_index,
                       ent_real *phase);

enum entropictron_error
entropictron_set_osc_seed(struct entropictron *kick,
                      size_t osc_index,
                      unsigned int seed);

enum entropictron_error
entropictron_get_osc_seed(struct entropictron *kick,
                      size_t osc_index,
                      unsigned int *seed);

enum entropictron_error
entropictron_set_length(struct entropictron *kick,
                    ent_real len);

enum entropictron_error
entropictron_get_length(struct entropictron *kick,
                    ent_real *len);

enum entropictron_error
entropictron_get_max_length(struct entropictron *kick,
                        ent_real *len);

enum entropictron_error
entropictron_kick_set_amplitude(struct entropictron *kick,
                            ent_real amplitude);

enum entropictron_error
entropictron_kick_get_amplitude(struct entropictron *kick,
                            ent_real *amplitude);

enum entropictron_error
entropictron_kick_filter_enable(struct entropictron *kick,
                            int enable);

enum entropictron_error
entropictron_kick_filter_is_enabled(struct entropictron *kick,
                                int *enabled);

enum entropictron_error
entropictron_kick_set_filter_frequency(struct entropictron *kick,
                                   ent_real frequency);

enum entropictron_error
entropictron_kick_get_filter_frequency(struct entropictron *kick,
                                   ent_real *frequency);

enum entropictron_error
entropictron_kick_set_filter_factor(struct entropictron *kick,
                                ent_real factor);

enum entropictron_error
entropictron_kick_get_filter_factor(struct entropictron *kick,
                                ent_real *factor);

enum entropictron_error
entropictron_set_kick_filter_type(struct entropictron *kick,
                              enum ent_filter_type type);

enum entropictron_error
entropictron_get_kick_filter_type(struct entropictron *kick,
                              enum ent_filter_type *type);

enum entropictron_error
entropictron_kick_envelope_get_points(struct entropictron *kick,
                                  enum entropictron_envelope_type env_type,
                                  struct ent_envelope_point_info **buf,
                                  size_t *npoints);

enum entropictron_error
entropictron_kick_envelope_set_points(struct entropictron *kick,
                                  enum entropictron_envelope_type env_type,
                                  const struct ent_envelope_point_info *buff,
                                  size_t npoints);

enum entropictron_error
entropictron_kick_add_env_point(struct entropictron *kick,
                            enum entropictron_envelope_type env_type,
                            const struct ent_envelope_point_info *point_info);

enum entropictron_error
entropictron_kick_remove_env_point(struct entropictron *kick,
                               enum entropictron_envelope_type env_type,
                               size_t index);

enum entropictron_error
entropictron_kick_update_env_point(struct entropictron *kick,
                               enum entropictron_envelope_type env_type,
                               size_t index,
                               const struct ent_envelope_point_info *point_info);

enum entropictron_error
entropictron_kick_env_set_apply_type(struct entropictron *kick,
				 enum entropictron_envelope_type env_type,
				 enum ent_envelope_apply_type apply_type);

enum entropictron_error
entropictron_kick_env_get_apply_type(struct entropictron *kick,
				 enum entropictron_envelope_type env_type,
				 enum ent_envelope_apply_type *apply_type);

enum entropictron_error
entropictron_set_osc_amplitude(struct entropictron *kick,
                           size_t osc_index,
                           ent_real v);
enum entropictron_error
entropictron_set_osc_frequency(struct entropictron *kick,
                           size_t osc_index,
                           ent_real v);

enum entropictron_error
entropictron_set_osc_pitch_shift(struct entropictron *kick,
                           size_t osc_index,
                           ent_real semitones);

enum entropictron_error
entropictron_set_osc_noise_density(struct entropictron *kick,
                               size_t osc_index,
                               ent_real density);

enum entropictron_error
entropictron_get_osc_amplitude(struct entropictron *kick,
                           size_t osc_index,
                           ent_real *v);
enum entropictron_error
entropictron_get_osc_frequency(struct entropictron *kick,
                           size_t osc_index,
                           ent_real *v);

enum entropictron_error
entropictron_get_osc_pitch_shift(struct entropictron *kick,
                             size_t osc_index,
                             ent_real *semitones);

enum entropictron_error
entropictron_get_osc_noise_density(struct entropictron *kick,
                               size_t osc_index,
                               ent_real *density);

enum entropictron_error
entropictron_play(struct entropictron *kick, size_t id);

/**
 * This function must be called
 * only from the audio thread.
 */
enum entropictron_error
entropictron_key_pressed(struct entropictron *kick,
                     bool pressed,
                     int note,
                     int velocity);

enum entropictron_error
entropictron_get_kick_buffer_size(struct entropictron *kick,
                              size_t *size);

enum entropictron_error
entropictron_get_kick_buffer(struct entropictron *kick,
                         ent_real *buffer,
                         size_t size);

enum entropictron_error
entropictron_set_kick_buffer_callback(struct entropictron *kick,
                                  void (*callback)(void*,
                                                   ent_real *buff,
                                                   size_t size,
                                                   size_t id),
                                  void *arg);

enum entropictron_error
entropictron_set_kick_limiter_callback(struct entropictron *kick,
                                   void (*callback)(void*,
                                                    size_t index,
                                                    ent_real val),
                                   void *arg);

enum entropictron_error
entropictron_set_limiter_value(struct entropictron *kick,
                           ent_real limit);

enum entropictron_error
entropictron_get_limiter_value(struct entropictron *kick,
                           ent_real *limit);

enum entropictron_error
entropictron_set_osc_filter_type(struct entropictron *kick,
                             size_t osc_index,
                             enum ent_filter_type type);
enum entropictron_error
entropictron_get_osc_filter_type(struct entropictron *kick,
                             size_t osc_index,
                             enum ent_filter_type *type);

enum entropictron_error
entropictron_set_osc_filter_cutoff_freq(struct entropictron *kick,
                                    size_t osc_index,
                                    ent_real cutoff);
enum entropictron_error
entropictron_get_osc_filter_cutoff_freq(struct entropictron *kick,
                                    size_t osc_index,
                                    ent_real *cutoff);

enum entropictron_error
entropictron_set_osc_filter_factor(struct entropictron *kick,
                               size_t osc_index,
                               ent_real factor);

enum entropictron_error
entropictron_get_osc_filter_factor(struct entropictron *kick,
                               size_t osc_index,
                               ent_real *factor);

enum entropictron_error
entropictron_enbale_osc_filter(struct entropictron *kick,
                           size_t osc_index,
                           int enable);

enum entropictron_error
entropictron_osc_filter_is_enabled(struct entropictron *kick,
                               size_t osc_index,
                               int *enable);

enum entropictron_error
entropictron_get_sample_rate(struct entropictron *kick,
                         int *sample_rate);

enum entropictron_error
entropictron_enable_synthesis(struct entropictron *kick,
                          bool enable);

enum entropictron_error
entropictron_audio_process(struct entropictron *kick,
                       float **out,
                       size_t channel,
                       size_t size);

enum entropictron_error
entropictron_distortion_enable(struct entropictron *kick,
                           bool enable);

enum entropictron_error
entropictron_distortion_is_enabled(struct entropictron *kick,
                               bool *enabled);

enum entropictron_error
entropictron_distortion_set_type(struct entropictron *kick,
                             enum ent_distortion_type type);

enum entropictron_error
entropictron_distortion_get_type(struct entropictron *kick,
                             enum ent_distortion_type *type);

enum entropictron_error
entropictron_distortion_set_in_limiter(struct entropictron *kick,
                                   ent_real limit);

enum entropictron_error
entropictron_distortion_get_in_limiter(struct entropictron *kick,
                                   ent_real *limit);

enum entropictron_error
entropictron_distortion_set_out_limiter(struct entropictron *kick,
                                    ent_real value);

enum entropictron_error
entropictron_distortion_get_out_limiter(struct entropictron *kick,
                                    ent_real *volume);

enum entropictron_error
entropictron_distortion_set_drive(struct entropictron *kick,
                              ent_real drive);

enum entropictron_error
entropictron_distortion_get_drive(struct entropictron *kick,
                              ent_real *drive);

enum entropictron_error
entropictron_osc_distortion_enable(struct entropictron *kick,
                               size_t index,
                               bool enable);

enum entropictron_error
entropictron_osc_distortion_is_enabled(struct entropictron *kick,
                                   size_t index,
                                   bool *enabled);

enum entropictron_error
entropictron_osc_distortion_set_type(struct entropictron *kick,
                                 size_t index,
                                 enum ent_distortion_type type);

enum entropictron_error
entropictron_osc_distortion_get_type(struct entropictron *kick,
                                 size_t index,
                                 enum ent_distortion_type *type);

enum entropictron_error
entropictron_osc_distortion_set_in_limiter(struct entropictron *kick,
                                       size_t index,
                                       ent_real value);

enum entropictron_error
entropictron_osc_distortion_get_in_limiter(struct entropictron *kick,
                                       size_t index,
                                       ent_real *value);

enum entropictron_error
entropictron_osc_distortion_set_out_limiter(struct entropictron *kick,
                                        size_t index,
                                        ent_real value);

enum entropictron_error
entropictron_osc_distortion_get_out_limiter(struct entropictron *kick,
                                        size_t index,
                                        ent_real *value);

enum entropictron_error
entropictron_osc_distortion_set_drive(struct entropictron *kick,
                                  size_t index,
                                  ent_real drive);

enum entropictron_error
entropictron_osc_distortion_get_drive(struct entropictron *kick,
                                  size_t index,
                                  ent_real *drive);

int entropictron_is_module_enabed(struct entropictron *kick,
                              enum ENTROPICTRON_MODULE module);

enum entropictron_error
entropictron_enable_group(struct entropictron *kick,
                      size_t index,
                      bool enable);

enum entropictron_error
entropictron_group_enabled(struct entropictron *kick,
                       size_t index,
                       bool *enabled);

enum entropictron_error
entropictron_group_set_amplitude(struct entropictron *kick,
                             size_t index,
                             ent_real amplitude);

enum entropictron_error
entropictron_group_get_amplitude(struct entropictron *kick,
                             size_t index,
                             ent_real *amplitude);

enum entropictron_error
entropictron_tune_audio_output(struct entropictron *kick,
                           size_t index,
                           bool tune);

enum entropictron_error
entropictron_is_audio_output_tuned(struct entropictron *kick,
                               size_t index,
                               bool *tune);

enum entropictron_error
entropictron_set_osc_sample(struct entropictron *kick,
                        size_t osc_index,
                        const ent_real *data,
                        size_t size);

enum entropictron_error
entropictron_get_osc_sample(struct entropictron *kick,
                        size_t osc_index,
                        ent_real **data,
                        size_t *size);

enum entropictron_error
entropictron_set_current_percussion(struct entropictron *kick,
                                size_t index);

enum entropictron_error
entropictron_get_current_percussion(struct entropictron *kick,
                                size_t *index);

enum entropictron_error
entropictron_unused_percussion(struct entropictron *kick,
                           int *index);

enum entropictron_error
entropictron_enable_percussion(struct entropictron *kick,
                           size_t index,
                           bool enable);

enum entropictron_error
entropictron_is_percussion_enabled(struct entropictron *kick,
                               size_t index,
                               bool *enable);

enum entropictron_error
entropictron_set_playing_key(struct entropictron *kick,
                         size_t id,
                         signed char key);

enum entropictron_error
entropictron_get_playing_key(struct entropictron *kick,
                         size_t id,
                         signed char *key);

enum entropictron_error
entropictron_set_midi_channel(struct entropictron *kick,
                          size_t id,
                          signed char channel);

enum entropictron_error
entropictron_get_midi_channel(struct entropictron *kick,
                          size_t id,
                          signed char *channel);

enum entropictron_error
entropictron_force_midi_channel(struct entropictron *kick,
                            signed char channel,
                            bool force);

enum entropictron_error
entropictron_ged_forced_midi_channel(struct entropictron *kick,
                                 signed char *channel,
                                 bool *force);

size_t
entropictron_midi_channels_number(void);

enum entropictron_error
entropictron_set_percussion_name(struct entropictron *kick,
                             size_t id,
                             const char *name,
                             size_t size);

enum entropictron_error
entropictron_get_percussion_name(struct entropictron *kick,
                             size_t id,
                             char *name,
                             size_t size);


size_t
entropictron_instruments_number();

size_t
entropictron_channels_number();

enum entropictron_error
entropictron_set_percussion_channel(struct entropictron *kick,
                                size_t id,
                                size_t channel);

enum entropictron_error
entropictron_get_percussion_channel(struct entropictron *kick,
                                size_t id,
                                size_t *channel);

enum entropictron_error
entropictron_percussion_set_limiter(struct entropictron *kick,
                                size_t id,
                                ent_real val);

enum entropictron_error
entropictron_percussion_get_limiter(struct entropictron *kick,
                                size_t id,
                                ent_real *val);

enum entropictron_error
entropictron_percussion_mute(struct entropictron *kick,
                         size_t id,
                         bool b);

enum entropictron_error
entropictron_percussion_is_muted(struct entropictron *kick,
                             size_t id,
                             bool *b);

enum entropictron_error
entropictron_percussion_solo(struct entropictron *kick,
                         size_t id,
                         bool b);

enum entropictron_error
entropictron_percussion_is_solo(struct entropictron *kick,
                            size_t id,
                            bool *b);


enum entropictron_error
entropictron_set_preview_sample(struct entropictron *kick,
                            const ent_real *data,
                            size_t size);

void
entropictron_play_sample_preview(struct entropictron *kick);

enum entropictron_error
entropictron_set_sample_preview_limiter(struct entropictron *kick, ent_real val);

enum entropictron_error
entropictron_get_sample_preview_limiter(struct entropictron *kick, ent_real *val);

enum entropictron_error
entropictron_percussion_enable_note_off(struct entropictron *kick,
                                    size_t id,
                                    bool enable);

enum entropictron_error
entropictron_percussion_note_off_enabled(struct entropictron *kick,
                                     size_t id,
                                     bool *enabled);

enum entropictron_error
entropictron_note_off_enabled(struct entropictron *kick,
                          size_t id,
                          bool *enabled);

size_t entropictron_layers_number(void);

bool
entropictron_is_plugin();

#ifdef __cplusplus
}
#endif

#endif
