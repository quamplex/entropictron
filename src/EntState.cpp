/**
 * File name: EntState.cpp
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

#include "EntState.h"
#include "ent_state.h"

#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

using namespace rapidjson;

EntState::EntState()
{
}

EntState::EntState(const std::string &jsonData)
{
        fromJson(jsonData);
}

EntState::EntState(const struct ent_state *state)
{
        setState(state);
}

void EntState::getState(struct ent_state* state) const
{
        // Play mode
        ent_state_set_play_mode(state, getPlayMode());

        for (size_t i = 0; i < 2; i++) {
                // Noise
                auto ns = ent_state_get_noise(state, i);
                ent_state_noise_set_enabled(ns, noise[i].enabled);
                ent_state_noise_set_type(ns, noise[i].type);
                ent_state_noise_set_density(ns, noise[i].density);
                ent_state_noise_set_brightness(ns, noise[i].brightness);
                ent_state_noise_set_gain(ns, noise[i].gain);
                ent_state_noise_set_stereo(ns, noise[i].stereo);
                ent_state_noise_set_filter_type(ns, noise[i].filter_type);
                ent_state_noise_set_cutoff(ns, noise[i].cutoff);
                ent_state_noise_set_resonance(ns, noise[i].resonance);
 
                // Crackle
                auto cs = ent_state_get_crackle(state, i);
                ent_state_crackle_set_enabled(cs, crackle[i].enabled);
                ent_state_crackle_set_rate(cs, crackle[i].rate);
                ent_state_crackle_set_duration(cs, crackle[i].duration);
                ent_state_crackle_set_amplitude(cs, crackle[i].amplitude);
                ent_state_crackle_set_randomness(cs, crackle[i].randomness);
                ent_state_crackle_set_brightness(cs, crackle[i].brightness);
                ent_state_crackle_set_envelope_shape(cs, crackle[i].envelope_shape);
                ent_state_crackle_set_stereo_spread(cs, crackle[i].stereo_spread);

                // Glitch
                auto gs = ent_state_get_glitch(state, i);
                ent_state_glitch_set_enabled(gs, glitch[i].enabled);
                ent_state_glitch_set_probability(gs, glitch[i].probability);
                ent_state_glitch_set_min_jump(gs, glitch[i].min_jump);
                ent_state_glitch_set_max_jump(gs, glitch[i].max_jump);
                ent_state_glitch_set_length(gs, glitch[i].length);
                ent_state_glitch_set_repeats(gs, glitch[i].repeats);
        }
}

void EntState::setState(const struct ent_state* state)
{
        // Play mode
        setPlayMode(ent_state_get_play_mode(state));

        for (size_t i = 0; i < 2; i++) {
                // Noise
                const auto* ns = ent_state_get_noise_const(state, i);
                noise[i].enabled = ent_state_noise_get_enabled(ns);
                noise[i].type = ent_state_noise_get_type(ns);
                noise[i].density = ent_state_noise_get_density(ns);
                noise[i].brightness = ent_state_noise_get_brightness(ns);
                noise[i].gain = ent_state_noise_get_gain(ns);
                noise[i].stereo = ent_state_noise_get_stereo(ns);
                noise[i].filter_type = ent_state_noise_get_filter_type(ns);
                noise[i].cutoff = ent_state_noise_get_cutoff(ns);
                noise[i].resonance = ent_state_noise_get_resonance(ns);

                // Crackle
                const auto* cs = ent_state_get_crackle_const(state, i);
                crackle[i].enabled = ent_state_crackle_get_enabled(cs);
                crackle[i].rate = ent_state_crackle_get_rate(cs);
                crackle[i].duration = ent_state_crackle_get_duration(cs);
                crackle[i].amplitude = ent_state_crackle_get_amplitude(cs);
                crackle[i].randomness = ent_state_crackle_get_randomness(cs);
                crackle[i].brightness = ent_state_crackle_get_brightness(cs);
                crackle[i].envelope_shape = ent_state_crackle_get_envelope_shape(cs);
                crackle[i].stereo_spread = ent_state_crackle_get_stereo_spread(cs);

                // Glitch
                const auto* gs = ent_state_get_glitch_const(state, i);
                glitch[i].enabled = ent_state_glitch_get_enabled(gs);
                glitch[i].probability = ent_state_glitch_get_probability(gs);
                glitch[i].min_jump = ent_state_glitch_get_min_jump(gs);
                glitch[i].max_jump = ent_state_glitch_get_max_jump(gs);
                glitch[i].length = ent_state_glitch_get_length(gs);
                glitch[i].repeats = ent_state_glitch_get_repeats(gs);
        }
}

void EntState::setName(const std::string_view &name)
{
        presetName = name;
}

std::string EntState::getName() const
{
        return presetName;
}

void EntState::setAuthor(const std::string_view &author)
{
        presetAuthor = author;
}

std::string EntState::getAuthor() const
{
        return presetAuthor;
}

void EntState::setAuthorURL(const std::string_view &url)
{
        presetAuthorURL = url;
}

std::string EntState::getAuthorURL() const
{
        return presetAuthorURL;
}

void EntState::setLicense(const std::string_view &license)
{
        presetLicense = license;
}

std::string EntState::getLicense() const
{
        return presetLicense;
}

void EntState::setPlayMode(int mode)
{
        playMode = mode;
}

int EntState::getPlayMode() const
{
        return playMode;
}

EntState::Noise& EntState::getNoise(NoiseId id)
{
        return noise[static_cast<size_t>(id)];
}

std::string EntState::toJson(bool asPreset) const
{
        Document doc;
        doc.SetObject();
        auto& a = doc.GetAllocator();

        if (asPreset) {
                doc.AddMember("application_name", Entropictron::applicationName, a);
                doc.AddMember("application_version", Entropictron::applicationVersion, a);

                // Preset metadata
                doc.AddMember("preset_name",
                              rapidjson::Value(getName().c_str(), a), a);
                doc.AddMember("author",
                              rapidjson::Value(getAuthor().c_str(), a), a);
                doc.AddMember("authorURL",
                              rapidjson::Value(getAuthorURL().c_str(), a), a);
                doc.AddMember("license",
                              rapidjson::Value(getLicense().c_str(), a), a);
        }

        Value global(kObjectType);
        if (!asPreset)
                global.AddMember("playmode", getPlayMode(), a);
        doc.AddMember("global", global, a);

        Value modules(kArrayType);

        writeNoise(modules, a);
        writeCrackle(modules, a);
        writeGlitch(modules, a);

        doc.AddMember("modules", modules, a);

        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        doc.Accept(writer);

        return buffer.GetString();
}

bool EntState::fromJson(const std::string& jsonStr)
{
    Document doc;
    if (doc.Parse(jsonStr.c_str()).HasParseError())
        return false;

    // Preset metadata
    if (doc.HasMember("preset_name") && doc["preset_name"].IsString())
            setName(doc["preset_name"].GetString());

    if (doc.HasMember("author") && doc["author"].IsString())
            setAuthor(doc["author"].GetString());

    if (doc.HasMember("authorURL") && doc["authorURL"].IsString())
            setAuthorURL(doc["authorURL"].GetString());

    if (doc.HasMember("license") && doc["license"].IsString())
            setLicense(doc["license"].GetString());

    // Global section
    if (!doc.HasMember("global") || !doc["global"].IsObject())
        return false;

    const auto& global = doc["global"];

    if (global.HasMember("playmode") && global["playmode"].IsInt())
            setPlayMode(global["playmode"].GetInt());

    // Modules
    if (!doc.HasMember("modules") || !doc["modules"].IsArray())
        return false;

    for (const auto& m : doc["modules"].GetArray()) {
        if (!m.HasMember("id") || !m["id"].IsInt())
            continue;

        if (!m.HasMember("name") || !m["name"].IsString())
                continue;

        std::string moduleName = m["name"].GetString();
        if (moduleName == "noise")
                readNoise(m, m["id"].GetInt());
        else if (moduleName == "crackle")
                readCrackle(m, m["id"].GetInt());
        else if (moduleName == "glitch")
                readGlitch(m, m["id"].GetInt());
    }

    return true;
}

bool EntState::saveToFile(const std::filesystem::path& filepath) const
{
        std::ofstream ofs(filepath, std::ios::trunc);
        if (!ofs.is_open())
                return false;

        ofs << toJson(true);
        return true;
}

bool EntState::loadFromFile(const std::filesystem::path& filepath)
{
        std::ifstream ifs(filepath);
        if (!ifs.is_open())
                return false;

        std::string jsonStr((std::istreambuf_iterator<char>(ifs)),
                            std::istreambuf_iterator<char>());
        return fromJson(jsonStr);
}

void EntState::writeNoise(Value& modulesArray,
                          Document::AllocatorType& a) const
{
        for (size_t i = 0; i < std::size(noise); i++) {
                Value m(kObjectType);
                m.AddMember("id", i, a);
                m.AddMember("name", "noise", a);
                m.AddMember("enabled", noise[i].enabled, a);
                m.AddMember("type", noise[i].type, a);
                m.AddMember("density", noise[i].density, a);
                m.AddMember("brightness", noise[i].brightness, a);
                m.AddMember("gain", noise[i].gain, a);
                m.AddMember("stereo", noise[i].stereo, a);
                m.AddMember("filter_type", noise[i].filter_type, a);
                m.AddMember("cutoff", noise[i].cutoff, a);
                m.AddMember("resonance", noise[i].resonance, a);
                modulesArray.PushBack(m, a);
        }
}

void EntState::writeCrackle(Value& modulesArray,
                            Document::AllocatorType& a) const
{
        for (size_t i = 0; i < std::size(noise); i++) {
                Value m(kObjectType);
                m.AddMember("id", i, a);
                m.AddMember("name", "crackle", a);
                m.AddMember("enabled", crackle[i].enabled, a);
                m.AddMember("rate", crackle[i].rate, a);
                m.AddMember("randomness", crackle[i].randomness, a);
                m.AddMember("amplitude", crackle[i].amplitude, a);
                m.AddMember("env_type", crackle[i].envelope_shape, a);
                m.AddMember("brightness", crackle[i].brightness, a);
                m.AddMember("duration", crackle[i].duration, a);
                m.AddMember("stereo", crackle[i].stereo_spread, a);
                modulesArray.PushBack(m, a);
        }
}

void EntState::writeGlitch(Value& modulesArray,
                           Document::AllocatorType& a) const
{
        for (size_t i = 0; i < std::size(noise); i++) {
                Value m(kObjectType);
                m.AddMember("id", i, a);
                m.AddMember("name", "glitch", a);
                m.AddMember("enabled", glitch[i].enabled, a);
                m.AddMember("repeats", glitch[i].repeats, a);
                m.AddMember("probability", glitch[i].probability, a);
                m.AddMember("length", glitch[i].length, a);
                m.AddMember("min_jump", glitch[i].min_jump, a);
                m.AddMember("max_jump", glitch[i].max_jump, a);
                modulesArray.PushBack(m, a);
        }
}

void EntState::readNoise(const Value& m, size_t id)
{
        if (id >= std::size(noise))
                return;

        if (m.HasMember("enabled") && m["enabled"].IsBool())
                noise[id].enabled = m["enabled"].GetBool();
        if (m.HasMember("type") && m["type"].IsInt())
                noise[id].type = std::clamp(m["type"].GetInt(), 0, 2);
        if (m.HasMember("density") && m["density"].IsNumber())
                noise[id].density = m["density"].GetDouble();
        if (m.HasMember("brightness") && m["brightness"].IsNumber())
                noise[id].brightness = m["brightness"].GetDouble();
        if (m.HasMember("gain") && m["gain"].IsNumber())
                noise[id].gain = m["gain"].GetDouble();
        if (m.HasMember("stereo") && m["stereo"].IsNumber())
                noise[id].stereo = m["stereo"].GetDouble();
        if (m.HasMember("filter_type") && m["filter_type"].IsInt())
                noise[id].filter_type =
                        std::clamp(m["filter_type"].GetInt(), 0, 2);
        if (m.HasMember("cutoff") && m["cutoff"].IsNumber())
                noise[id].cutoff = m["cutoff"].GetDouble();
        if (m.HasMember("resonance") && m["resonance"].IsNumber())
                noise[id].resonance = m["resonance"].GetDouble();
}

void EntState::readCrackle(const Value& m, size_t id)
{
        if (id >= std::size(crackle))
                return;

        if (m.HasMember("enabled") && m["enabled"].IsBool())
                crackle[id].enabled = m["enabled"].GetBool();
        if (m.HasMember("rate") && m["rate"].IsNumber())
                crackle[id].rate = m["rate"].GetDouble();
        if (m.HasMember("randomness") && m["randomness"].IsNumber())
                crackle[id].randomness = m["randomness"].GetDouble();
        if (m.HasMember("amplitude") && m["amplitude"].IsNumber())
                crackle[id].amplitude = m["amplitude"].GetDouble();
        if (m.HasMember("env_type") && m["env_type"].IsInt())
                crackle[id].envelope_shape =
                        std::clamp(m["env_type"].GetInt(), 0, 2);
        if (m.HasMember("brightness") && m["brightness"].IsNumber())
                crackle[id].brightness = m["brightness"].GetDouble();
        if (m.HasMember("duration") && m["duration"].IsNumber())
                crackle[id].duration = m["duration"].GetDouble();
        if (m.HasMember("stereo") && m["stereo"].IsNumber())
                crackle[id].stereo_spread = m["stereo"].GetDouble();
}

void EntState::readGlitch(const Value& m, size_t id)
{
        if (id >= std::size(glitch))
                return;

        if (m.HasMember("enabled") && m["enabled"].IsBool())
                glitch[id].enabled = m["enabled"].GetBool();
        if (m.HasMember("repeats") && m["repeats"].IsInt())
                glitch[id].repeats = m["repeats"].GetInt();
        if (m.HasMember("probability") && m["probability"].IsNumber())
                glitch[id].probability = m["probability"].GetDouble();
        if (m.HasMember("length") && m["length"].IsNumber())
                glitch[id].length = m["length"].GetDouble();
        if (m.HasMember("min_jump") && m["min_jump"].IsNumber())
                glitch[id].min_jump = m["min_jump"].GetDouble();
        if (m.HasMember("max_jump") && m["max_jump"].IsNumber())
                glitch[id].max_jump = m["max_jump"].GetDouble();
}
