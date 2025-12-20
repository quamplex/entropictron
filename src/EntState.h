/**
 * File name: EntState.h
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

#ifndef ENT_STATE_H
#define ENT_STATE_H

#include "globals.h"
#include "DspTypes.h"

#include <rapidjson/document.h>

struct ent_state;

class EntState
{
 public:
        struct Noise {
                int id = 1;
                bool enabled = true;
                int type = 0;
                double density = 0.0;
                double brightness = 0.0;
                double gain = 0.0;
                double stereo = 0.0;
                bool filter_enabled = false;
                int filter_type = 0;
                double cutoff = 0.0;
                double resonance = 0.0;
        };

        struct Crackle {
                int id = 2;
                bool enabled = true;
                double rate = 0.0;
                double randomness = 0.0;
                double amplitude = 0.0;
                int envelope_shape = 0;
                double brightness = 0.0;
                double duration = 0.0;
                double stereo_spread = 0.0;
        };

        struct Glitch {
                int id = 3;
                bool enabled = true;
                int repeats = 0;
                double probability = 0.0;
                double length = 0.0;
                double min_jump = 0.0;
                double max_jump = 0.0;
        };

        Noise noise[2];
        Crackle crackle[2];
        Glitch glitch[2];

        EntState();
        explicit EntState(const std::string &jsonData);
        explicit EntState(const struct ent_state *state);
        void setState(const struct ent_state* state);
        void getState(struct ent_state* state) const;
        void setName(const std::string_view &name);
        std::string getName() const;
        void setAuthor(const std::string_view &author);
        std::string getAuthor() const;
        void setAuthorURL(const std::string_view &url);
        std::string getAuthorURL() const;
        void setLicense(const std::string_view &license);
        std::string getLicense() const;
        void setPlayMode(int mode);
        int getPlayMode() const;
        EntState::Noise& getNoise(NoiseId id);
        std::string toJson(bool asPreset = false) const;
        bool fromJson(const std::string& jsonStr);
        bool saveToFile(const std::filesystem::path& filepath) const;
        bool loadFromFile(const std::filesystem::path& filepath);

 protected:
        void writeNoise(rapidjson::Value& modulesArray,
                        rapidjson::Document::AllocatorType& a) const;
        void writeCrackle(rapidjson::Value& modulesArray,
                          rapidjson::Document::AllocatorType& a) const;
        void writeGlitch(rapidjson::Value& modulesArray,
                         rapidjson::Document::AllocatorType& a) const;
        void readNoise(const rapidjson::Value& m, size_t id);
        void readCrackle(const rapidjson::Value& m, size_t id);
        void readGlitch(const rapidjson::Value& m, size_t id);

 private:
        std::string presetName;
        std::string presetAuthor;
        std::string presetAuthorURL;
        std::string presetLicense;
        int playMode = 0;
};

#endif // ENT_STATE_H
