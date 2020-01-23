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

#include <algorithm>
#include <fstream>
#include <string.h>
#include <switch.h>

#include "Doom.hpp"
#include "File.hpp"
#include "../Application.hpp"

namespace ChocolateDoomLauncher::Services {
    std::vector<Models::WAD> Doom::getWADSInDir(std::string path, WADType type) {
        std::vector<Models::WAD> wads;

        auto files = Services::File::filenamesInDirectory(path);
        for (auto const& file : files) {
            if (file.compare(file.length() - 4, 4, ".wad") || file.compare(file.length() - 4, 4, ".WAD")) {
                auto fileType = getWADType(path + "/" + file);

                if (fileType == type) {
                    Models::WAD wad;
                    wad.path = path;
                    wad.filename = file;
                    wad.name = (type == IWAD) ? identifyIWAD(path, file) : file;
                    wad.type = type;

                    wads.push_back(wad);
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

    std::string Doom::identifyIWAD(std::string path, std::string filename) {
        // Transform path to all uppercase.
        std::transform(filename.begin(), filename.end(), filename.begin(), ::toupper);

        if (filename.compare("DOOM.WAD") == 0) {
            auto lumpNames = getWADLumpNames(path + "/" + filename);
            auto name = std::find(lumpNames.begin(), lumpNames.end(), "E4M1");
            if (name != lumpNames.end()) {
                return "The Ultimate DOOM";
            } else {
                return "DOOM Registered";
            }
        } else if (filename.compare("DOOM1.WAD") == 0) {
            return "DOOM Shareware";
        } else if (filename.compare("DOOM2.WAD") == 0) {
            return "DOOM 2: Hell on Earth";
        } else if (filename.compare("PLUTONIA.WAD") == 0) {
            return "DOOM 2: Plutonia Experiment";
        } else if (filename.compare("TNT.WAD") == 0) {
            return "DOOM 2: TNT - Evilution";
        } else if (filename.compare("FREEDM.WAD") == 0) {
            return "FreeDM";
        } else if (filename.compare("FREEDOOM1.WAD") == 0) {
            return "Freedoom: Phase 1";
        } else if (filename.compare("FREEDOOM2.WAD") == 0) {
            return "Freedoom: Phase 2";
        }
        
        // Unknown
        return filename;
    }

    std::vector<std::string> Doom::getWADLumpNames(std::string path) {
        std::vector<std::string> result;

        // Open the WAD file.        
        std::ifstream wadStream(path, std::ios::in | std::ios::binary);
        if (!wadStream) {
            return result;
        }

        // Get size
        wadStream.seekg(0, wadStream.end);
        int size = wadStream.tellg();

        // Get Directory Location.
        int directoryLocation = 0;
        wadStream.seekg(8, wadStream.beg);
        wadStream.read((char *) &directoryLocation, 4);

        // Go to beginning of directory.
        wadStream.seekg(directoryLocation, wadStream.beg);
        
        // Parse all the lump names from the directory
        int currentPosition = directoryLocation;
        char * lumpNameBuffer = new char[9];
        lumpNameBuffer[8] = '\0';
        while (currentPosition < size) {
            wadStream.seekg(8, wadStream.cur);
            wadStream.read(lumpNameBuffer, 8);
            result.push_back(std::string(lumpNameBuffer));

            currentPosition += 16;
        }
    
        return result;
    }

    bool Doom::loadDoom(Models::WAD iwad, Models::WAD pwad) {
        auto currentWorkingDirectory = File::currentWorkingDirectory();

        auto path = currentWorkingDirectory + "/doom.nro";
        auto args = "-nogui -waddir " + currentWorkingDirectory + "/wads -iwad " + iwad.filename;
        
        if (!pwad.filename.empty()) {
            args += " -file " + pwad.filename;

            int extensionLocation = pwad.filename.find_last_of(".");
            std::string deh = pwad.filename.substr(0, extensionLocation) + ".deh";
            if (File::fileExists(pwad.path + "/wads/" + deh)) {
                args += " -deh " + deh;
            }
        }

        if (R_FAILED(envSetNextLoad(path.c_str(), args.c_str()))) {
            return false;
        }

        Application::switchScene(NULL);
        return true;
    }
}
