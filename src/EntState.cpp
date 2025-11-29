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

#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

using namespace rapidjson;

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

    // --- Preset metadata ---
    if (doc.HasMember("preset_name") && doc["preset_name"].IsString())
            setName(doc["preset_name"].GetString());

    if (doc.HasMember("author") && doc["author"].IsString())
            setAuthor(doc["author"].GetString());

    if (doc.HasMember("authorURL") && doc["authorURL"].IsString())
            setAuthorURL(doc["authorURL"].GetString());

    if (doc.HasMember("license") && doc["license"].IsString())
            setLicense(doc["license"].GetString());

    // --- Global section ---
    if (!doc.HasMember("global") || !doc["global"].IsObject())
        return false;

    const auto& global = doc["global"];

    if (global.HasMember("playmode") && global["playmode"].IsInt())
            setPlayMode(global["playmode"].GetInt());

    // --- Modules ---
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
                m.AddMember("env_type", crackle[i].env_type, a);
                m.AddMember("brightness", crackle[i].brightness, a);
                m.AddMember("duration", crackle[i].duration, a);
                m.AddMember("stereo", crackle[i].stereo, a);
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

        noise[id].enabled     = m["enabled"].GetBool();
        noise[id].type        = m["type"].GetInt();
        noise[id].density     = m["density"].GetDouble();
        noise[id].brightness  = m["brightness"].GetDouble();
        noise[id].gain        = m["gain"].GetDouble();
        noise[id].stereo      = m["stereo"].GetDouble();
        noise[id].filter_type = m["filter_type"].GetInt();
        noise[id].cutoff      = m["cutoff"].GetDouble();
        noise[id].resonance   = m["resonance"].GetDouble();
}

void EntState::readCrackle(const Value& m, size_t id)
{
        if (id >= std::size(crackle))
                return;

        crackle[id].enabled     = m["enabled"].GetBool();
        crackle[id].rate        = m["rate"].GetDouble();
        crackle[id].randomness  = m["randomness"].GetDouble();
        crackle[id].amplitude   = m["amplitude"].GetDouble();
        crackle[id].env_type    = m["env_type"].GetInt();
        crackle[id].brightness  = m["brightness"].GetDouble();
        crackle[id].duration    = m["duration"].GetDouble();
        crackle[id].stereo      = m["stereo"].GetDouble();
}

void EntState::readGlitch(const Value& m, size_t id)
{
        if (id >= std::size(glitch))
                return;

        glitch[id].enabled     = m["enabled"].GetBool();
        glitch[id].repeats     = m["repeats"].GetInt();
        glitch[id].probability = m["probability"].GetDouble();
        glitch[id].length      = m["length"].GetDouble();
        glitch[id].min_jump    = m["min_jump"].GetDouble();
        glitch[id].max_jump    = m["max_jump"].GetDouble();
}
