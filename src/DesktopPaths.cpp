/**
 * File name: DesktopPaths.cpp
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

#include "DesktopPaths.h"

#ifdef ENT_OS_WINDOWS
#include <windows.h>
#endif // ENT_OS_WINDS

DesktopPaths::DesktopPaths()
{
	loadPaths();
}

std::filesystem::path DesktopPaths::getHomePath() const
{
	return homePath;
}

std::filesystem::path DesktopPaths::getDesktopPath() const
{
	return desktopPath;
}

std::filesystem::path DesktopPaths::getDownloadsPath() const
{
	return downloadsPath;
}

std::filesystem::path DesktopPaths::getConfigPath() const
{
	return configPath;
}

std::filesystem::path DesktopPaths::getDataPath() const
{
	return dataPath;
}

std::filesystem::path DesktopPaths::getFactoryPresetsPath() const
{
	return factoryPresetsPath;
}

std::filesystem::path DesktopPaths::getUserPresetsPath() const
{
	return userPresetsPath;
}

#ifdef ENT_OS_WINDOWS
void DesktopPaths::loadPaths()
{
	const char* userProfilePath = std::getenv("USERPROFILE");
	if (userProfilePath != nullptr) {
		homePath = userProfilePath;
	} else {
		ENT_LOG_ERROR("Failed to get home directory path.");
	}

	desktopPath = homePath / std::filesystem::path("Desktop");
	downloadsPath = homePath / std::filesystem::path("Downloads");

	const char* dataProfilePath = std::getenv("APPDATA");
	if (dataProfilePath != nullptr) {
		dataPath = dataProfilePath;
		dataPath /= std::filesystem::path(ENT_APP_NAME);
	} else {
		ENT_LOG_ERROR("Failed to get data directory path.");
	}
	configPath = dataPath / std::filesystem::path("config");

	const char* programDataPath = std::getenv("ProgramData");
	if (programDataPath != nullptr) {
		dataPath = programDataPath;
		dataPath /= std::filesystem::path(ENT_APP_NAME);
	} else {
		ENT_LOG_ERROR("Failed to get program data directory path.");
	}
	userPresetsPath = dataPath / std::filesystem::path("presets");
        factoryPresetsPath = userPresetsPath;

        loadDrivesList();
}

void DesktopPaths::loadDrivesList()
{
        auto drivesBitMask = GetLogicalDrives();
        for (auto driveLetter = 'A'; driveLetter <= 'Z'; ++driveLetter) {
                if ((drivesBitMask & 1) != 0) {
                        auto drivePath = std::filesystem::path(std::string(1, driveLetter) + std::string(":\\"));
                        auto driveType = GetDriveType(drivePath.string().c_str());
                        if (driveType == DRIVE_FIXED)
                                drivesList.push_back(drivePath);
                }
                drivesBitMask >>= 1;
        }
}
#else // ENT_OS_GNU
std::filesystem::path DesktopPaths::findFactoryPresetsPath(const std::filesystem::path &presetsPath) const
{
        const std::filesystem::path factoryFileName = "factory.txt";
        const std::filesystem::path presetsPathSuffix = "presets";
        if (std::filesystem::exists(presetsPath / factoryFileName))
                return presetsPath;

        // Compile-time ENT_DATA_DIR
#ifdef ENT_DATA_DIR
        auto compileTimePath = std::filesystem::path(ENT_DATA_DIR)
                               / std::filesystem::path(ENT_APP_NAME)
                               / presetsPathSuffix;
        if (std::filesystem::exists(compileTimePath / factoryFileName))
                return compileTimePath;
#endif

        // XDG_DATA_DIRS or fallback to /usr/local/share and /usr/share
        const char *dataDirs = std::getenv("XDG_DATA_DIRS");
        std::vector<std::filesystem::path> searchPaths;

        if (dataDirs && *dataDirs) {
                std::stringstream ss(dataDirs);
                std::string path;
                while (std::getline(ss, path, ':')) {
                        searchPaths.emplace_back(std::filesystem::path(path));
                }
        } else {
                searchPaths = {
                        "/usr/local/share",
                        "/usr/share"
                };
        }

        for (const auto &dir : searchPaths) {
                auto path = dir / std::filesystem::path(ENT_APP_NAME)
                            / presetsPathSuffix;
                if (std::filesystem::exists(path / factoryFileName))
                        return path;
        }

        return userPresetsPath;
}

void DesktopPaths::loadPaths()
{
        const auto *dataHome = std::getenv("XDG_DATA_HOME");
        if (dataHome == nullptr || *dataHome == '\0') {
                const auto *homeDir = std::getenv("HOME");
                if (homeDir == nullptr || *homeDir == '\0')
			homeDir = ".";
                dataPath = homeDir / std::filesystem::path(".local/share")
                           / std::filesystem::path(ENT_APP_NAME);
                homePath = homeDir;
        } else {
                dataPath = dataHome / std::filesystem::path(ENT_APP_NAME);
                homePath = dataHome;
        }
	desktopPath = homePath / std::filesystem::path("Desktop");
	downloadsPath = homePath / std::filesystem::path("Downloads");
	userPresetsPath = dataPath / std::filesystem::path("presets");
        factoryPresetsPath = findFactoryPresetsPath(userPresetsPath);

        const auto *configHome = std::getenv("XDG_CONFIG_HOME");
        if (configHome == nullptr || *configHome == '\0') {
		configPath = homePath / std::filesystem::path(".config")
			     / std::filesystem::path(ENT_APP_NAME);
        } else {
                configPath = configHome / std::filesystem::path(ENT_APP_NAME);
        }
}
#endif // ENT_OS_GNU

std::vector<std::filesystem::path> DesktopPaths::getDrivesList() const
{
        return drivesList;
}

bool DesktopPaths::isRootPath(const std::filesystem::path &path)
{
        if (!path.has_root_directory())
                return false;

        if (path.has_root_directory() && path.filename().empty())
                return true;

        if (path.root_name() / path.root_directory() == path)
                return true;

        return false;
}
