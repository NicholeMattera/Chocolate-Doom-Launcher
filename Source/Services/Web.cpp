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
        std::vector<char> data = _makeRequest(
            "https://kosmos-builder.teamatlasnx.com/github/" + user + "/" + repo + "/version",
            onProgressChanged
        );

        if (data.size() == 0) {
            return NULL;
        }

        return std::string(data.begin(), data.end());
    }

    std::string Web::getLatestReleaseURL(std::string user, std::string repo, std::string pattern, std::function<void(double)> onProgressChanged) {
        std::vector<char> data = _makeRequest(
            "https://kosmos-builder.teamatlasnx.com/github/" + user + "/" + repo + "/release?pattern=" + pattern,
            onProgressChanged
        );

        if (data.size() == 0) {
            return NULL;
        }

        return std::string(data.begin(), data.end());
    }

    std::vector<char> Web::downloadFile(std::string url, std::function<void(double)> onProgressChanged) {
        return _makeRequest(url, onProgressChanged);
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

    std::vector<char> Web::_makeRequest(std::string url, std::function<void(double)> onProgressChanged) {
        std::vector<char> buffer;

        CURL * curl = curl_easy_init();
        if (!curl) {
            return buffer;
        }

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _write);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &buffer);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, _progress);
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, (void *) &onProgressChanged);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            buffer.clear();
            return buffer;
        }

        curl_easy_cleanup(curl);

        return buffer;
    }

    size_t Web::_write(const char * in, size_t size, size_t num, std::vector<char> * buffer) {
        size_t i = 0;
        while (i < size * num) {
            buffer->push_back(*in);
            ++in;
            i++;
        }

        return i;
    }

    size_t Web::_progress(std::function<void(double)> * onProgressChanged, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
        if (onProgressChanged != NULL) {
            auto progress = (double) dlnow / (double) dltotal;
            (*onProgressChanged)(progress);
        }

        return 0;
    }
}
