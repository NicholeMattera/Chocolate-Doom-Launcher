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

#include <dirent.h>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <sstream>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/unistd.h>

#include "File.hpp"

namespace ChocolateDoomLauncher::Services {
    void File::initialSetup() {
        // Make sure our folder structure is in place.
        if (!directoryExists("./dehs"))
            createDirectories("./dehs");

        if (!directoryExists("./mods"))
            createDirectories("./mods");

        if (!directoryExists("./savegames"))
            createDirectories("./savegames");

        if (!directoryExists("./wads"))
            createDirectories("./wads");

        if (!fileExists("./chocolate-doom.cfg"))
            copyFile("romfs:/chocolate-doom.cfg", "./chocolate-doom.cfg");

        if (!fileExists("./default.cfg"))
            copyFile("romfs:/default.cfg", "./default.cfg");

        // I messed up with 1.0.0 and Chocolate Doom NX had the root SD as 
        //  the current working directory. Lets clean up our mess.
        if (directoryExists("sdmc:/savegames")) {
            recursiveMove("sdmc:/savegames", "./savegames");
        }

        if (fileExists("sdmc:/chocolate-doom.cfg")) {
            deleteFile("sdmc:/chocolate-doom.cfg");
        }

        if (fileExists("sdmc:/default.cfg")) {
            deleteFile("sdmc:/default.cfg");
        }
    }
    
    std::string File::currentWorkingDirectory() {
        char path[PATH_MAX];
        getcwd(path, PATH_MAX);
        return std::string(path);
    }

    bool File::directoryExists(std::string path) {
        auto directory = opendir(path.c_str());
        if (directory) {
            closedir(directory);
            return true;
        }

        return false;
    }

    // http://stackoverflow.com/a/11366985
    bool File::createDirectories(std::string path) {
        auto bSuccess = false;
        auto nRC = ::mkdir(path.c_str(), 0775);
        if(nRC == -1)
        {
            switch(errno)
            {
                case ENOENT:
                    //parent didn't exist, try to create it
                    if(createDirectories(path.substr(0, path.find_last_of('/'))))
                        //Now, try to create again.
                        bSuccess = 0 == ::mkdir(path.c_str(), 0775);
                    else
                        bSuccess = false;
                    break;
                case EEXIST:
                    //Done!
                    bSuccess = true;
                    break;std::string getHost();
                default:
                    bSuccess = false;
                    break;
            }
        }
        else
            bSuccess = true;
        
        return bSuccess;
    }

    std::string File::sanitizeDirectoryName(std::string name) {
        std::string result = "";

        for (char& c : name) {
            if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || c == '.') {
                result += c;
            } else if (c >= 'A' && c <= 'Z') {
                result += std::tolower(c);
            } else if (result.back() != '_') {
                result += '_';
            }
        }

        return result;
    }

    std::string File::openFile(std::string source) {
        std::ifstream fileStream(source);
        if (!fileStream.is_open())
            return "";

        std::stringstream buffer;
        buffer << fileStream.rdbuf();
        fileStream.close();

        return buffer.str();
    }

    bool File::copyFile(std::string source, std::string destination) {
        std::ifstream sourceStream(source, std::ios::binary);
        if (!sourceStream.is_open())
            return false;

        std::ofstream destinationStream(destination, std::ios::binary);
        if (!destinationStream.is_open())
            return false;

        destinationStream << sourceStream.rdbuf();

        sourceStream.close();

        destinationStream.flush();
        destinationStream.close();

        return true;
    }

    bool File::deleteFile(std::string path) {
        if (fileExists(path)) {
            return remove(path.c_str()) == 0;
        }

        return false;
    }

    bool File::fileExists(std::string path) {
        struct stat buffer;
        return (stat(path.c_str(), &buffer) == 0);
    }

    std::vector<std::string> File::filenamesInDirectory(std::string path) {
        std::vector<std::string> files;

        auto directory = opendir(path.c_str());
        if (directory) {
            struct dirent * entry;

            while ((entry = readdir(directory)) != NULL) {
                auto filename = std::string(entry->d_name);
                files.push_back(filename);
            }

            closedir(directory);
        }

        return files;
    }

    void File::recursiveMove(std::string source, std::string destination) {
        struct stat info;
        stat(source.c_str(), &info);

        if (S_ISDIR(info.st_mode)) {
            if (!directoryExists(destination)) {
                createDirectories(destination);
            }
            
            auto directory = opendir(source.c_str());
            struct dirent * entry;
            while ((entry = readdir(directory)) != NULL) {
                if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
                    continue;
                }

                recursiveMove(source + "/" + entry->d_name, destination + "/" + entry->d_name);
            }

            closedir(directory);
            rmdir(source.c_str());
        } else {
            rename(source.c_str(), destination.c_str());
        }
    }
}
