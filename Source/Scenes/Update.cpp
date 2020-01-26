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

#include <SDL2/SDL_image.h>
 
#include "Update.hpp"
#include "../Application.hpp"
#include "../Managers/Font.hpp"
#include "../Scenes/GameSelection.hpp"
#include "../Services/File.hpp"
#include "../Services/Theme.hpp"
#include "../Services/Web.hpp"

namespace ChocolateDoomLauncher::Scenes {
    Update::Update() {
        auto fm = Managers::Font::Instance();

        background = Services::Theme::background;

        auto backgroundSurface = IMG_Load("romfs:/Background.png");
        _backgroundTexture = SDL_CreateTextureFromSurface(Application::renderer, backgroundSurface);
        SDL_FreeSurface(backgroundSurface);

        _background = new Views::Image(_backgroundTexture, TILE);
        _background->frame = { 0, 0, 1280, 720 };
        addSubView(_background);

        _header = new Views::Header("Chocolate Doom Launcher", true);
        _header->frame = { 0, 0, 1280, 88 };
        addSubView(_header);

        _progressBar = new Views::ProgressBar();
        _progressBar->frame = { 437, 330, 411, 10 };
        addSubView(_progressBar);

        _text = new Views::Text(fm->getFont(StandardFont, 18), "Checking for updates of Chocolate Doom Launcher...", Services::Theme::text);
        _text->frame = { 0, 370, 1280, 20 };
        _text->textAlignment = CENTER_ALIGN;
        addSubView(_text);

        _footer = new Views::Footer();
        _footer->frame = { 0, 647, 1280, 73 };
        addSubView(_footer);

        
    }

    Update::~Update() {
        if (_backgroundTexture != NULL)
            SDL_DestroyTexture(_backgroundTexture);

        if (_background != NULL)
            delete _background;

        if (_header != NULL)
            delete _header;

        if (_progressBar != NULL)
            delete _progressBar;

        if (_text != NULL)
            delete _text;

        if (_footer != NULL)
            delete _footer;
    }

    void Update::tick(SDL_Rect rect, double dTime) {
        if (!_downloadsComplete) {
            auto restartRequired = _updateLauncher();
            _updateGame();

            if (!restartRequired) {
                Application::switchScene(new Scenes::GameSelection());
            }

            _downloadsComplete = true;
        }
    }

    void Update::buttonsDown(u32 buttons, double dTime) {
        
    }

    bool Update::_updateLauncher() {
        Application::currentApplication->render();

        auto latestVersion = Services::Web::getLatestVersion(
            "NicholeMattera",
            "Chocolate-Doom-Launcher",
            std::bind(&Update::_onProgressUpdate, this, std::placeholders::_1)
        );

        // If there isn't a newer version
        if (true) {
            return false;
        }

        _text->setText("Getting latest release of Chocolate Doom Launcher...");
        Application::currentApplication->render();

        auto latestReleaseURL = Services::Web::getLatestReleaseURL(
            "NicholeMattera",
            "Chocolate-Doom-Launcher",
            ".*\\.nro",
            std::bind(&Update::_onProgressUpdate, this, std::placeholders::_1)
        );

        _text->setText("Downloading latest release of Chocolate Doom Launcher...");
        Application::currentApplication->render();

        auto launcherRelease = Services::Web::downloadFile(
            latestReleaseURL,
            std::bind(&Update::_onProgressUpdate, this, std::placeholders::_1)
        );

        // Quit RomFS.
        // Overwrite NRO.

        return true;
    }

    void Update::_updateGame() {
        Application::currentApplication->render();

        auto doomExists = Services::File::fileExists("./doom.nro");
        auto newVersionAvailable = false;

        if (doomExists) {
            _text->setText("Checking for updates of Chocolate Doom NX...");
            Application::currentApplication->render();

            auto latestVersion = Services::Web::getLatestVersion(
                "NicholeMattera",
                "chocolate-doom-nx",
                std::bind(&Update::_onProgressUpdate, this, std::placeholders::_1)
            );
        }

        // If there is a newer version or it doesn't already exists.
        if (!Services::File::fileExists("./doom.nro") || newVersionAvailable) {
            _text->setText("Getting latest release of Chocolate Doom NX...");
            Application::currentApplication->render();

            auto latestReleaseURL = Services::Web::getLatestReleaseURL(
                "NicholeMattera",
                "chocolate-doom-nx",
                ".*\\.nro",
                std::bind(&Update::_onProgressUpdate, this, std::placeholders::_1)
            );

            _text->setText("Downloading latest release of Chocolate Doom NX...");
            Application::currentApplication->render();

            auto launcherRelease = Services::Web::downloadFile(
                latestReleaseURL,
                std::bind(&Update::_onProgressUpdate, this, std::placeholders::_1)
            );

            // Overwrite NRO.
        }
    }

    void Update::_onProgressUpdate(double progress) {
        _progressBar->setProgress(progress);
        Application::currentApplication->render();
    }
}
