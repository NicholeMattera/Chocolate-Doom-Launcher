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

#include <switch.h>

#include "Web.hpp"

namespace ChocolateDoomLauncher::Services {
    std::string Web::getLatestVersion(std::string user, std::string repo, std::function<void(double)> onProgressChanged) {
        return "";
    }

    std::string Web::getLatestReleaseURL(std::string user, std::string repo, std::string pattern, std::function<void(double)> onProgressChanged) {
        return "";
    }

    std::vector<char> Web::downloadFile(std::string url, std::function<void(double)> onProgressChanged) {
        std::vector<char> result;
        return result;
    }

    bool Web::hasInternetConnection() {
        NifmInternetConnectionType connectionType;
        u32 wifiStrength = 0;
        NifmInternetConnectionStatus connectionStatus;

        if (R_FAILED(nifmGetInternetConnectionStatus(&connectionType, &wifiStrength, &connectionStatus))) {
            return false;
        }

        return (
            (connectionType == NifmInternetConnectionType_Ethernet && connectionStatus == NifmInternetConnectionStatus_Connected) || 
            (connectionType == NifmInternetConnectionType_WiFi && wifiStrength > 0)
        );
    }
}
