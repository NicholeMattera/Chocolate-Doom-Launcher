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

#include "Header.hpp"
#include "../Application.hpp"
#include "../Constants.hpp"
#include "../Managers/Theme.hpp"
#include "../Managers/Font.hpp"

namespace ChocolateDoomLauncher::Views {
    Header::Header(std::string title, bool showIcon) {
        auto fm = Managers::Font::Instance();
        auto tm = Managers::Theme::Instance();

        this->background = { 0, 0, 0, 0 };

        _showIcon = showIcon;

        if (showIcon) {
            auto icon1Surface = IMG_Load("romfs:/Icon1.png");
            _icon[0] = SDL_CreateTextureFromSurface(Application::renderer, icon1Surface);
            SDL_FreeSurface(icon1Surface);

            auto icon2Surface = IMG_Load("romfs:/Icon2.png");
            _icon[1] = SDL_CreateTextureFromSurface(Application::renderer, icon2Surface);
            SDL_FreeSurface(icon2Surface);

            _iconImage = new Image(_icon[0]);
            _iconImage->frame = { 69, 32, 40, 38 };
            addSubView(_iconImage);
        }

        _titleText = new Text(fm->getFont(StandardFont, 28), title, tm->text);
        _titleText->frame = {
            (showIcon) ? 132 : 74,
            36,
            (showIcon) ? 1074 : 1132,
            33
        };
        addSubView(_titleText);
    }

    Header::~Header() {
        if (_titleText != NULL)
            delete _titleText;

        if (_icon[0] != NULL)
            SDL_DestroyTexture(_icon[0]);

        if (_icon[1] != NULL)
            SDL_DestroyTexture(_icon[1]);
            
        if (_iconImage != NULL)
            delete _iconImage;
    }

    void Header::onTick(SDL_Rect rect, double dTime) {
        _timeElapsed += dTime;
        
        if ((_timeElapsed <= 500 || _timeElapsed >= 1000) && _currentIcon != 0) {
            _iconImage->setImage(_icon[0]);
            _currentIcon = 0;
            requiresRendering = true;

            if (_timeElapsed >= 1000) {
                _timeElapsed -= 1000;
            }
        } else if (_timeElapsed > 500 && _currentIcon != 1) {
            _iconImage->setImage(_icon[1]);
            _currentIcon = 1;
            requiresRendering = true;
        }
    }

    void Header::onRender(SDL_Rect rect, double dTime) {
        auto tm = Managers::Theme::Instance();

        // Divider
        auto divider = tm->header_footer_divider;
        SDL_SetRenderDrawColor(Application::renderer, divider.r, divider.g, divider.b, divider.a);
        SDL_RenderDrawLine(Application::renderer, 30, rect.y + rect.h - 1, rect.x + rect.w - 30, rect.y + rect.h - 1);
    }
}
