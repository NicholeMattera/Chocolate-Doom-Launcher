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
 
#include "Error.hpp"
#include "../Application.hpp"
#include "../Managers/Font.hpp"
#include "../Services/Theme.hpp"

namespace ChocolateDoomLauncher::Scenes {
    Error::Error(std::string errorMessage) {
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

        _error = new Views::Text(fm->getFont(StandardFont, 23), errorMessage, Services::Theme::text);
        _error->frame = { 0, 320, 1280, 70 };
        _error->textAlignment = CENTER_ALIGN;
        addSubView(_error);

        _footer = new Views::Footer();
        _footer->frame = { 0, 647, 1280, 73 };
        addSubView(_footer);

        auto quitAction = new Models::FooterAction();
        quitAction->button = B_BUTTON;
        quitAction->text = "Quit";
        _footer->addAction(quitAction);
    }

    Error::~Error() {
        if (_backgroundTexture != NULL)
            SDL_DestroyTexture(_backgroundTexture);

        if (_background != NULL)
            delete _background;

        if (_header != NULL)
            delete _header;

        if (_error != NULL)
            delete _error;

        if (_footer != NULL)
            delete _footer;
    }

    void Error::buttonsDown(u32 buttons, double dTime) {
        if (buttons & KEY_B) {
            Application::switchScene(NULL);
        }
    }
}
