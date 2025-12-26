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
#include <stdatomic.h>

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
        atomic_store_explicit(&state->play_mode,
                              static_cast<enum ent_play_mode>(getPlayMode()),
                              memory_order_release);

        for (size_t i = 0; i < 2; i++) {
                // Noise
                atomic_store_explicit(&state->noises[i].enabled,
                                      noise[i].enabled,
                                      memory_order_relaxed);
                atomic_store_explicit(&state->noises[i].type,
                                      static_cast<enum ent_noise_type>(noise[i].type),
                                      memory_order_relaxed);
                atomic_store_explicit(&state->noises[i].density,
                                      noise[i].density,
                                      memory_order_relaxed);
                atomic_store_explicit(&state->noises[i].brightness,
                                      noise[i].brightness,
                                      memory_order_relaxed);
                atomic_store_explicit(&state->noises[i].gain,
                                      noise[i].gain,
                                      memory_order_relaxed);
                atomic_store_explicit(&state->noises[i].stereo,
                                      noise[i].stereo,
                                      memory_order_relaxed);
                atomic_store_explicit(&state->noises[i].filter_type,
                                      static_cast<enum ent_filter_type>(noise[i].filter_type),
                                      memory_order_relaxed);
                atomic_store_explicit(&state->noises[i].cutoff,
                                      noise[i].cutoff,
                                      memory_order_relaxed);
                atomic_store_explicit(&state->noises[i].resonance,
                                      noise[i].resonance,
                                      memory_order_relaxed);

                // Crackle
                atomic_store_explicit(&state->crackles[i].enabled,
                                      crackle[i].enabled,
                                      memory_order_relaxed);
                atomic_store_explicit(&state->crackles[i].rate,
                                      crackle[i].rate,
                                      memory_order_relaxed);
                atomic_store_explicit(&state->crackles[i].duration,
                                      crackle[i].duration,
                                      memory_order_relaxed);
                atomic_store_explicit(&state->crackles[i].amplitude,
                                      crackle[i].amplitude,
                                      memory_order_relaxed);
                atomic_store_explicit(&state->crackles[i].randomness,
                                      crackle[i].randomness,
                                      memory_order_relaxed);
                atomic_store_explicit(&state->crackles[i].brightness,
                                      crackle[i].brightness,
                                      memory_order_relaxed);
                atomic_store_explicit(&state->crackles[i].envelope_shape,
                                      static_cast<ent_crackle_envelope>(crackle[i].envelope_shape),
                                      memory_order_relaxed);
                atomic_store_explicit(&state->crackles[i].stereo_spread,
                                      crackle[i].stereo_spread,
                                      memory_order_relaxed);

                // Glitch
                atomic_store_explicit(&state->glitches[i].enabled,
                                      glitch[i].enabled,
                                      memory_order_relaxed);
                atomic_store_explicit(&state->glitches[i].probability,
                                      glitch[i].probability,
                                      memory_order_relaxed);
                atomic_store_explicit(&state->glitches[i].min_jump,
                                      glitch[i].min_jump,
                                      memory_order_relaxed);
                atomic_store_explicit(&state->glitches[i].max_jump,
                                      glitch[i].max_jump,
                                      memory_order_relaxed);
                atomic_store_explicit(&state->glitches[i].length,
                                      glitch[i].length,
                                      memory_order_relaxed);
                atomic_store_explicit(&state->glitches[i].repeats,
                                      glitch[i].repeats,
                                      memory_order_relaxed);
        }
}

void EntState::setState(const struct ent_state* state)
{
        setPlayMode(atomic_load_explicit(&state->play_mode, memory_order_acquire));

        for (size_t i = 0; i < 2; i++) {
                //  Noise
                noise[i].enabled = atomic_load_explicit(&state->noises[i].enabled,
                                                        memory_order_relaxed);
                noise[i].type = atomic_load_explicit(&state->noises[i].type,
                                                     memory_order_relaxed);
                noise[i].density = atomic_load_explicit(&state->noises[i].density,
                                                        memory_order_relaxed);
                noise[i].brightness = atomic_load_explicit(&state->noises[i].brightness,
                                                           memory_order_relaxed);
                noise[i].gain = atomic_load_explicit(&state->noises[i].gain,
                                                     memory_order_relaxed);
                noise[i].stereo = atomic_load_explicit(&state->noises[i].stereo,
                                                       memory_order_relaxed);
                noise[i].filter_type = atomic_load_explicit(&state->noises[i].filter_type,
                                                            memory_order_relaxed);
                noise[i].cutoff = atomic_load_explicit(&state->noises[i].cutoff,
                                                       memory_order_relaxed);
                noise[i].resonance = atomic_load_explicit(&state->noises[i].resonance,
                                                          memory_order_relaxed);

                // Crackle
                crackle[i].enabled = atomic_load_explicit(&state->crackles[i].enabled,
                                                          memory_order_relaxed);
                crackle[i].rate = atomic_load_explicit(&state->crackles[i].rate,
                                                       memory_order_relaxed);
                crackle[i].duration = atomic_load_explicit(&state->crackles[i].duration,
                                                           memory_order_relaxed);
                crackle[i].amplitude = atomic_load_explicit(&state->crackles[i].amplitude,
                                                            memory_order_relaxed);
                crackle[i].randomness = atomic_load_explicit(&state->crackles[i].randomness,
                                                             memory_order_relaxed);
                crackle[i].brightness = atomic_load_explicit(&state->crackles[i].brightness,
                                                             memory_order_relaxed);
                crackle[i].envelope_shape = atomic_load_explicit(&state->crackles[i].envelope_shape,
                                                                 memory_order_relaxed);
                crackle[i].stereo_spread = atomic_load_explicit(&state->crackles[i].stereo_spread,
                                                                memory_order_relaxed);

                // Glitch
                glitch[i].enabled = atomic_load_explicit(&state->glitches[i].enabled,
                                                         memory_order_relaxed);
                glitch[i].probability = atomic_load_explicit(&state->glitches[i].probability,
                                                             memory_order_relaxed);
                glitch[i].min_jump = atomic_load_explicit(&state->glitches[i].min_jump,
                                                          memory_order_relaxed);
                glitch[i].max_jump = atomic_load_explicit(&state->glitches[i].max_jump,
                                                          memory_order_relaxed);
                glitch[i].length = atomic_load_explicit(&state->glitches[i].length,
                                                        memory_order_relaxed);
                glitch[i].repeats = atomic_load_explicit(&state->glitches[i].repeats,
                                                         memory_order_relaxed);
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
