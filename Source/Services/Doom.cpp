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
#include <SimpleIniParser.hpp>
#include <string.h>
#include <switch.h>

#include "Doom.hpp"
#include "File.hpp"
#include "../Application.hpp"

namespace ChocolateDoomLauncher::Services {
    std::vector<Models::Game> Doom::getGames() {
        std::vector<Models::Game> result;
        auto cwd = Services::File::currentWorkingDirectory();

        // Get base games.
        auto wadFilenames = Services::File::filenamesInDirectory(cwd + "/wads");
        for (auto const& filename : wadFilenames) {
            // Make sure our filename is even long enough to be a WAD file.
            if (filename.length() <= 4) {
                continue;
            }

            // Extract the extension and make it all uppercase.
            auto extension = filename.substr(filename.length() - 4, 4);
            std::transform(extension.begin(), extension.end(), extension.begin(), ::toupper);

            // Does it not end in a WAD file extension?
            if (extension.compare(".WAD") != 0) {
                continue;
            }

            // Make sure it's an IWAD.
            auto wadType = _getWADType(cwd + "/wads/" + filename);
            if (wadType == PWAD) {
                continue;
            }

            Models::Game game;
            game.isMod = false;
            game.name = _identifyIWAD(filename);
            game.iwad = filename;

            result.push_back(game);
        }

        // Get mods.
        auto modFilenames = Services::File::filenamesInDirectory(cwd + "/mods");
        for (auto const& filename : modFilenames) {
            // Make sure our filename is even long enough to be a INI file.
            if (filename.length() <= 4) {
                continue;
            }

            // Extract the extension and make it all uppercase.
            auto extension = filename.substr(filename.length() - 4, 4);
            std::transform(extension.begin(), extension.end(), extension.begin(), ::toupper);

            // Does it not end in a INI file extension?
            if (extension.compare(".INI") != 0) {
                continue;
            }

            // Parse the INI file.
            auto config = simpleIniParser::Ini::parseFile(cwd + "/mods/" + filename);
            if (config == nullptr) {
                continue;
            }

            // Get the name and iwad of the mod.
            auto nameOption = config->findFirstOption("name");
            auto iwadOption = config->findFirstOption("iwad");
            if (nameOption == nullptr || iwadOption == nullptr) {
                delete config;
                continue;
            }

            Models::Game game;
            game.isMod = true;
            game.name = nameOption->value;
            game.iwad = iwadOption->value;

            // Get all the dehacked files.
            auto dehOptions = config->findAllOptions("deh");
            for (auto const& dehOption : dehOptions) {
                game.dehs.push_back(dehOption->value);
            }
            
            // Get all the pwad files to load.
            auto fileOptions = config->findAllOptions("file");
            for (auto const& fileOption : fileOptions) {
                game.files.push_back(fileOption->value);
            }

            // Get all the pwad files that need to be merged using deutex's behavior with the iwad.
            auto mergeOptions = config->findAllOptions("merge");
            for (auto const& mergeOption : mergeOptions) {
                game.merges.push_back(mergeOption->value);
            }

            // Get all the pwad files that need to be merged using NWT's behavior with the iwad for flats and sprites.
            auto aaOptions = config->findAllOptions("aa");
            for (auto const& aaOption : aaOptions) {
                game.aas.push_back(aaOption->value);
            }

            // Get all the pwad files that need to be merged using NWT's behavior with the iwad for flats.
            auto afOptions = config->findAllOptions("af");
            for (auto const& afOption : afOptions) {
                game.afs.push_back(afOption->value);
            }

            // Get all the pwad files that need to be merged using NWT's behavior with the iwad for sprites.
            auto asOptions = config->findAllOptions("as");
            for (auto const& asOption : asOptions) {
                game.ass.push_back(asOption->value);
            }

            // Get all the pwad files that need to be merged using NWT's behavior with the iwad.
            auto nwtMergeOptions = config->findAllOptions("nwtmerge");
            for (auto const& nwtMergeOption : nwtMergeOptions) {
                game.nwtmerges.push_back(nwtMergeOption->value);
            }

            result.push_back(game);
            delete config;
        }

        // Sort games.
        std::sort(
            result.begin(),
            result.end(),
            [](const Models::Game & a, const Models::Game & b) -> bool {
                return a.name < b.name;
            });

        return result;
    }

    bool Doom::loadDoom(Models::Game game) {
        auto cwd = File::currentWorkingDirectory();

        auto path = cwd + "/doom.nro";
        auto args = "-nogui -waddir " + cwd + "/wads -iwad " + game.iwad;
        if (game.isMod) {
            args += " -savedir " + cwd + "/savegames/" + Services::File::sanitizeDirectoryName(game.name) + "/";
        } else {
            args += " -savedir " + cwd + "/savegames/" + Services::File::sanitizeDirectoryName(game.iwad) + "/";
        }
        args += " -config " + cwd + "/default.cfg -extraconfig " + cwd + "/chocolate-doom.cfg";
        
        if (game.dehs.size() > 0) {
            args += " -deh";
            for (auto const& deh : game.dehs) {
                args += " " + cwd + "/dehs/" + deh;
            }
        }

        if (game.files.size() > 0) {
            args += " -file";
            for (auto const& file : game.files) {
                args += " " + file;
            }
        }

        if (game.merges.size() > 0) {
            args += " -merge";
            for (auto const& merge : game.merges) {
                args += " " + merge;
            }
        }

        if (game.aas.size() > 0) {
            args += " -aa";
            for (auto const& aa : game.aas) {
                args += " " + aa;
            }
        }

        if (game.afs.size() > 0) {
            args += " -af";
            for (auto const& af : game.afs) {
                args += " " + af;
            }
        }

        if (game.ass.size() > 0) {
            args += " -as";
            for (auto const& as : game.ass) {
                args += " " + as;
            }
        }

        if (game.nwtmerges.size() > 0) {
            args += " -nwtmerge";
            for (auto const& nwtmerge : game.nwtmerges) {
                args += " " + nwtmerge;
            }
        }

        if (R_FAILED(envSetNextLoad(path.c_str(), args.c_str()))) {
            return false;
        }

        Application::switchScene(NULL);
        return true;
    }

    WADType Doom::_getWADType(std::string path) {
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

    std::string Doom::_identifyIWAD(std::string filename) {
        // Transform path to all uppercase.
        std::transform(filename.begin(), filename.end(), filename.begin(), ::toupper);

        if (filename.compare("DOOM.WAD") == 0) {
            auto cwd = Services::File::currentWorkingDirectory();
            auto lumpNames = _getWADLumpNames(cwd + "/wads/" + filename);
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
        } else if (filename.compare("HACX.WAD") == 0) {
            return "Hacx";
        }
        
        // Unknown
        return filename;
    }

    std::vector<std::string> Doom::_getWADLumpNames(std::string path) {
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
}
