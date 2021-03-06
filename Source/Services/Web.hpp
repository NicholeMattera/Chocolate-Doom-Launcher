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

#include <curl/curl.h>
#include <fstream> 
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#pragma once

namespace ChocolateDoomLauncher::Services {
    class Web {
        public:
            static std::string getLatestVersion(std::string user, std::string repo, std::function<void(double)> onProgressChanged);
            static std::string getLatestReleaseURL(std::string user, std::string repo, std::string pattern, std::function<void(double)> onProgressChanged);
            static std::vector<char> downloadFile(std::string url, std::string path, std::function<void(double)> onProgressChanged);
            static bool hasInternetConnection();

        private:
            static std::vector<char> _makeRequest(std::string url, std::string path, std::function<void(double)> onProgressChanged);
            static size_t _write(const char * in, size_t size, size_t num, std::vector<char> * buffer);
            static size_t _writeToFile(const char * in, size_t size, size_t num, std::ofstream * file);
            static size_t _progress(std::function<void(double)> * onProgressChanged, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);

    };
}
