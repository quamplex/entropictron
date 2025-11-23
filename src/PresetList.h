/**
 * File name: PresetList.h
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

#ifndef ENT_PRESET_LIST_H
#define ENT_PRESET_LIST_H

#include "globals.h"
#include "EntState.h"

#include "RkObject.h"

class PresetList
{
public:
        PresetList();
        bool loadFromDefaultPath();
        const std::vector<std::unique_ptr<EntState>>& getPresets() const;
        size_t size() const;
        const EntState& getPreset(size_t index) const;

private:
        std::vector<std::unique_ptr<EntState>> presetsList;
        std::filesystem::path presetFolder = "/usr/local/share/entropictron/presets";
};

#endif // ENT_PRESET_LIST_H

