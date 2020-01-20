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

#include <fstream>
#include <string.h>
#include <switch.h>

#include "Doom.hpp"
#include "File.hpp"
#include "../Application.hpp"

namespace ChocolateDoomLauncher::Services {
    std::vector<std::string> Doom::getWADSInDir(std::string path, WADType type) {
        std::vector<std::string> wads;

        auto files = Services::File::filenamesInDirectory(path);
        for (auto const& file : files) {
            if (file.compare(file.length() - 4, 4, ".wad") || file.compare(file.length() - 4, 4, ".WAD")) {
                auto fileType = getWADType(path + "/" + file);

                if (fileType == type) {
                    wads.push_back(file);
                }
            }
        }

        return wads;
    }

    WADType Doom::getWADType(std::string path) {
        auto result = UNKNOWN_WAD;
        std::ifstream wadStream(path, std::ios::in | std::ios::binary);
        if (!wadStream) {
            return result;
        }

        char buffer[5];
        wadStream.read(buffer, 4);
        buffer[4] = '\0';

        if (strcmp(buffer, "IWAD") == 0) {
            result = IWAD;
        } else if (strcmp(buffer, "PWAD") == 0) {
            result = PWAD;
        }

        wadStream.close();

        return result;
    }

    bool Doom::loadDoom(std::string iwad, std::string pwad) {
        auto currentWorkingDirectory = File::currentWorkingDirectory();

        auto path = currentWorkingDirectory + "/doom.nro";
        auto args = "-nogui -waddir " + currentWorkingDirectory + "/iwads -iwad " + iwad;
        
        if (!pwad.empty()) {
            args += " -file " + pwad;
        }

        if (R_FAILED(envSetNextLoad(path.c_str(), args.c_str()))) {
            return false;
        }

        Application::switchScene(NULL);
        return true;
    }
}
