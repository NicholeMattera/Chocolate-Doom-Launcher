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

#include <string>
#include <vector>

#pragma once

namespace ChocolateDoomLauncher::Services {
    class File {
        public:
            static std::string currentWorkingDirectory();
            static bool directoryExists(std::string path);
            static bool createDirectories(std::string path);
            static std::string sanitizeDirectoryName(std::string name);

            static bool fileExists(std::string path);
            static std::vector<std::string> filenamesInDirectory(std::string path);

        private:

    };
}
