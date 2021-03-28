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

#include <borealis.hpp>
#include <switch.h>

#include "Activities/Main.hpp"
#include "Activities/Error.hpp"
#include "Services/File.hpp"

using namespace brls::literals;
using namespace ChocolateDoomLauncher;

int main(int argc, char* argv[]) {
    brls::Logger::setLogLevel(brls::LogLevel::INFO);

    if (!brls::Application::init()) {
        brls::Logger::error("Unable to init Borealis application");
        return EXIT_FAILURE;
    }

    brls::Application::createWindow("main/title"_i18n);

    Services::File::initialSetup();
    if (Services::File::fileExists("./doom.nro")) {
        brls::Application::pushActivity(new Activities::Main());
    } else {
        brls::Application::pushActivity(new Activities::Error());
    }

    while (brls::Application::mainLoop())
        ;

    return EXIT_SUCCESS;
}