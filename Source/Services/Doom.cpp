// Chocolate Doom Launcher
// Copyright (c) 2021 Nichole Mattera
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include <algorithm>
#include <borealis.hpp>
#include <fstream>
#include <jansson.h>
#include <string.h>
#include <switch.h>

#include "Doom.hpp"
#include "File.hpp"

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
            if (wadType == Models::PWAD) {
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
            // Make sure our filename is even long enough to be a JSON file.
            if (filename.length() <= 5) {
                continue;
            }

            // Extract the extension and make it all uppercase.
            auto extension = filename.substr(filename.length() - 4, 4);
            std::transform(extension.begin(), extension.end(), extension.begin(), ::toupper);

            // Does it not end in a JSON file extension?
            if (extension.compare(".JSON") != 0) {
                continue;
            }

            Models::Game game;
            game.isMod = true;

            // Parse the JSON file.
            json_t * root = json_loads(Services::File::openFile(cwd + "/mods/" + filename).c_str(), 0, NULL);
            if (!root || !json_is_object(root)) continue;

            // Get the name of the mod.
            json_t * name = json_object_get(root, "name");
            if (!name || !json_is_string(name)) continue;
            game.name = std::string(json_string_value(name));

            // Get the iwad of the mod.
            json_t * iwad = json_object_get(root, "iwad");
            if (!iwad || !json_is_string(iwad)) continue;
            game.iwad = std::string(json_string_value(iwad));

            // Get all the dehacked files.
            json_t * dehs = json_object_get(root, "deh");
            if (dehs && json_is_array(dehs)) {
                for(size_t i = 0; i < json_array_size(dehs); i++) {
                    json_t * deh = json_array_get(dehs, i);
                    if (!deh || !json_is_string(deh)) continue;
                    game.dehs.push_back(std::string(json_string_value(deh)));
                }
            } else if (dehs && json_is_string(dehs)) {
                game.dehs.push_back(std::string(json_string_value(dehs)));
            }

            // Get all the pwad files to load.
            json_t * files = json_object_get(root, "file");
            if (files && json_is_array(files)) {
                for(size_t i = 0; i < json_array_size(files); i++) {
                    json_t * file = json_array_get(files, i);
                    if (!file || !json_is_string(file)) continue;
                    game.files.push_back(std::string(json_string_value(file)));
                }
            } else if (files && json_is_string(files)) {
                game.files.push_back(std::string(json_string_value(files)));
            }

            // Get all the pwad files that need to be merged using deutex's behavior with the iwad.
            json_t * merges = json_object_get(root, "merge");
            if (merges && json_is_array(merges)) {
                for(size_t i = 0; i < json_array_size(merges); i++) {
                    json_t * merge = json_array_get(merges, i);
                    if (!merge || !json_is_string(merge)) continue;
                    game.merges.push_back(std::string(json_string_value(merge)));
                }
            } else if (merges && json_is_string(merges)) {
                game.merges.push_back(std::string(json_string_value(merges)));
            }

            // Get all the pwad files that need to be merged using NWT's behavior with the iwad for flats and sprites.
            json_t * aas = json_object_get(root, "aa");
            if (aas && json_is_array(aas)) {
                for(size_t i = 0; i < json_array_size(aas); i++) {
                    json_t * aa = json_array_get(aas, i);
                    if (!aa || !json_is_string(aa)) continue;
                    game.aas.push_back(std::string(json_string_value(aa)));
                }
            } else if (aas && json_is_string(aas)) {
                game.aas.push_back(std::string(json_string_value(aas)));
            }

            // Get all the pwad files that need to be merged using NWT's behavior with the iwad for flats.
            json_t * afs = json_object_get(root, "af");
            if (afs && json_is_array(afs)) {
                for(size_t i = 0; i < json_array_size(afs); i++) {
                    json_t * af = json_array_get(afs, i);
                    if (!af || !json_is_string(af)) continue;
                    game.afs.push_back(std::string(json_string_value(af)));
                }
            } else if (afs && json_is_string(afs)) {
                game.afs.push_back(std::string(json_string_value(afs)));
            }

            // Get all the pwad files that need to be merged using NWT's behavior with the iwad for sprites.
            json_t * ass = json_object_get(root, "as");
            if (ass && json_is_array(ass)) {
                for(size_t i = 0; i < json_array_size(ass); i++) {
                    json_t * as = json_array_get(ass, i);
                    if (!as || !json_is_string(as)) continue;
                    game.ass.push_back(std::string(json_string_value(as)));
                }
            } else if (ass && json_is_string(ass)) {
                game.ass.push_back(std::string(json_string_value(ass)));
            }

            // Get all the pwad files that need to be merged using NWT's behavior with the iwad.
            json_t * nwtmerges = json_object_get(root, "nwtmerge");
            if (nwtmerges && json_is_array(nwtmerges)) {
                for(size_t i = 0; i < json_array_size(nwtmerges); i++) {
                    json_t * nwtmerge = json_array_get(nwtmerges, i);
                    if (!nwtmerge || !json_is_string(nwtmerge)) continue;
                    game.nwtmerges.push_back(std::string(json_string_value(nwtmerge)));
                }
            } else if (nwtmerges && json_is_string(nwtmerges)) {
                game.nwtmerges.push_back(std::string(json_string_value(nwtmerges)));
            }

            json_decref(root);
            result.push_back(game);
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
        auto args = path + " -launcher -waddir " + cwd + "/wads -iwad " + game.iwad;
        if (game.isMod) {
            args += " -savedir " + cwd + "/savegames/" + Services::File::sanitizeDirectoryName(game.name) + "/";
        }
        
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

        brls::Application::quit();
        return true;
    }

    Models::WADType Doom::_getWADType(std::string path) {
        auto result = Models::UNKNOWN_WAD;
        std::ifstream wadStream(path, std::ios::in | std::ios::binary);
        if (!wadStream) {
            return result;
        }

        char buffer[5];
        wadStream.read(buffer, 4);
        buffer[4] = '\0';

        if (strcmp(buffer, "IWAD") == 0) {
            result = Models::IWAD;
        } else if (strcmp(buffer, "PWAD") == 0) {
            result = Models::PWAD;
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
        delete lumpNameBuffer;

        return result;
    }
}
