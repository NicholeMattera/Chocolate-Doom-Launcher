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

#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/unistd.h>

#include "File.hpp"

namespace ChocolateDoomLauncher::Services {
    std::string File::currentWorkingDirectory() {
        char path[PATH_MAX];
        getcwd(path, PATH_MAX);
        return std::string(path);
    }

    bool File::directoryExists(std::string path) {
        auto dir = opendir(path.c_str());
        if (dir) {
            closedir(dir);
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

    bool File::fileExists(std::string path) {
        struct stat buffer;
        return (stat(path.c_str(), &buffer) == 0);
    }

    std::vector<std::string> File::filenamesInDirectory(std::string path) {
        std::vector<std::string> files;

        return files;
    }
}
