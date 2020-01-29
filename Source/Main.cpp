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

#include "Application.hpp"
#include "Scenes/Error.hpp"
#include "Scenes/GameSelection.hpp"
#include "Scenes/Update.hpp"
#include "Services/File.hpp"
#include "Services/Web.hpp"

int main(int argc, char *argv[]) {
    auto * app = new ChocolateDoomLauncher::Application(argc, argv);
    ChocolateDoomLauncher::Services::File::initialSetup();

    // Start our first scene.
    int val;
    if (ChocolateDoomLauncher::Services::File::fileExists("./doom.nro")) {
        val = app->start(new ChocolateDoomLauncher::Scenes::GameSelection());
    } else {
        val = app->start(new ChocolateDoomLauncher::Scenes::Error("Chocolate Doom NX is missing, please download Chocolate Doom NX and install it into:\n \n" + ChocolateDoomLauncher::Services::File::currentWorkingDirectory()));
    }

    // Clean up.
    delete app;
    return val;
}
