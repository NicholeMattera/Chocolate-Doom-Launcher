/*
 * Chocolate Doom Launcher
 * Copyright (c) 2020 Nichole Mattera
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <iostream>
#include <fstream> 
#include <switch.h>

#include "Version.hpp"

namespace ChocolateDoomLauncher::Services {
    std::string Version::getCurrentVersion() {
        return VERSION;
    }

    std::string Version::getVersionOfApp(std::string path) {
        std::ifstream file(path, std::ios::binary);
        if (file.fail())
            return "0.0.0";

        // Get NRO Header
        NroHeader header;
        file.seekg(sizeof(NroStart));
        file.read((char *) &header, sizeof(NroHeader));

        // Make sure we are dealing with an NRO file.
        if (header.magic != NROHEADER_MAGIC) {
            file.close();
            return "0.0.0";
        }

        // Get the asset header.
        NroAssetHeader assetHeader;
        file.seekg(header.size);
        file.read((char *) &assetHeader, sizeof(NroAssetHeader));

        // Make sure our asset header isn't malformed.
        if (assetHeader.magic != NROASSETHEADER_MAGIC ||
            assetHeader.version > NROASSETHEADER_VERSION ||
            assetHeader.nacp.offset == 0 ||
            assetHeader.nacp.size < sizeof(NacpStruct)) {

            file.close();
            return "0.0.0";
        }

        // Get the NACP.
        NacpStruct nacp;
        file.seekg(header.size + assetHeader.nacp.offset);
        file.read((char *) &nacp, sizeof(NacpStruct));

        file.close();

        return nacp.display_version;
    }

    std::string Version::sanitizeVersion(std::string version) {
        std::string result = "";

        for (char& c : version) {
            if ((c >= '0' && c <= '9') || c == '.') {
                result += c;
            }
        }

        return result;
    }

    bool Version::isNewerVersion(std::string currentVersion, std::string remoteVersion) {
        char currentVersionNumber = 0;
        size_t currentPos = currentVersion.find('.');
        if (currentPos != std::string::npos) {
            std::string currentSegment = currentVersion.substr(0, currentPos);
            currentVersionNumber = stoi(currentSegment);
        } else if (currentVersion.length() != 0) {
            currentVersionNumber = stoi(currentVersion);
        }

        char remoteVersionNumber = 0;
        size_t remotePos = remoteVersion.find('.');
        if (remotePos != std::string::npos) {
            std::string remoteSegment = remoteVersion.substr(0, remotePos);
            remoteVersionNumber = stoi(remoteSegment);
        } else if (remoteVersion.length() != 0) {
            remoteVersionNumber = stoi(remoteVersion);
        }

        if (remoteVersionNumber > currentVersionNumber)
            return true;
        else if (currentVersionNumber > remoteVersionNumber)
            return false;

        std::string currentRemaining = "", remoteRemaining = "";

        if (currentPos != std::string::npos) {
            currentRemaining = currentVersion.substr(currentPos + 1);
        }

        if (remotePos != std::string::npos) {
            remoteRemaining = remoteVersion.substr(remotePos + 1);
        }

        if (currentRemaining.length() + remoteRemaining.length() == 0) {
            return false;
        }
        
        return isNewerVersion(currentRemaining, remoteRemaining);
    }
}
