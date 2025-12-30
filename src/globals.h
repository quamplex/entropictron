/**
 * File name: globals.h
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

#ifndef ENTROPICTRON_GLOBALS_H
#define ENTROPICTRON_GLOBALS_H

#include "entropictron.h"
#include "RkLog.h"

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <atomic>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <string_view>
#include <array>
#include <mutex>
#include <thread>
#include <iomanip>
#include <math.h>
#include <optional>
#include <ranges>
#include <numbers>

namespace fs = std::filesystem;

#include <RkObject.h>

#ifdef ENT_LOG_DEBUG_LEVEL
#define ENT_LOG_INFO(msg) std::cerr << "[" << ENT_NAME << "] "    \
        << " [" << std::this_thread::get_id() << "] "             \
        << __PRETTY_FUNCTION__ << "[INFO] "                       \
        << msg << std::endl;
#define ENT_LOG_ERROR(msg) std::cerr << "[" << ENT_NAME << "] "   \
        << " [" << std::this_thread::get_id() << "] "             \
        << __PRETTY_FUNCTION__ << "[ERROR] "                      \
        << msg << std::endl;
#define ENT_LOG_DEBUG(msg) std::cerr << "[" \
        << std::this_thread::get_id() << "][" << ENT_NAME << "] " \
        << " [" << std::this_thread::get_id() << "] "             \
        << __PRETTY_FUNCTION__ << "[DEBUG] "                      \
        << msg << std::endl;
#else
#define ENT_LOG_INFO(msg) std::cerr << "[" << ENT_NAME << "] " \
                               << "[INFO] " << msg << std::endl;
#define ENT_LOG_ERROR(msg) std::cerr << "[" << ENT_NAME << "] " \
                               << "[ERROR] " << msg << std::endl;
#define ENT_LOG_DEBUG(msg)
#endif // ENT_LIB_LOG_LEVEL_DEBUG

namespace Entropictron
{
        constexpr int applicationVersion = ENT_VERSION;
        constexpr int defaultSampleRate = ENT_DEFAULT_SAMPLE_RATE;
        constexpr char applicationName[] = "Entropictron";
        constexpr char applicationVersionStr[] = ENT_VERSION_STRING;
        constexpr double toDecibel(double val)
        {
                if (val < 1e-5)
                        return -100;
                return 20 * log10(val);
        }

        constexpr double fromDecibel(double decibel)
        {
                if (decibel < -100.0)
                        return 0;
                return pow(10,  decibel / 20);
        }

        constexpr double truncateDouble(double val, unsigned int precision)
        {
                double n = std::pow(10, precision);
                return std::trunc(val * n) / n;
        }

        inline std::string doubleToStr(double val, unsigned int precision)
        {
                std::stringstream ss;
                ss << std::fixed
                   << std::setprecision(precision)
                   << truncateDouble(val, precision);
                return  ss.str();
        }

        inline std::string toLower(const std::string& input)
        {
                std::string result = input;
                std::transform(result.begin(), result.end(), result.begin(),
                               [](unsigned char c) { return std::tolower(c); });
                return result;
        }

        using MidiKey = unsigned char;
        constexpr MidiKey entropictronAnyKey = ENT_ANY_KEY;
        constexpr int entropictronAnyMidiChannel = ENT_ANY_MIDI_CHANNEL;
        constexpr MidiKey maxKeyVelocity = ENT_MAX_KEY_VELOCITY;
        constexpr MidiKey defaultMidiKey = ENT_DEFALUT_MIDI_KEY;
} // namespace Entropictron

#endif // ENTROPICTRON_GLOBALS_H
