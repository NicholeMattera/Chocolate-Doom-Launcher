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

#include <SDL2/SDL.h>
#include <string>

#include "Image.hpp"
#include "Text.hpp"
#include "../View.hpp"

#pragma once

namespace ChocolateDoomLauncher::Views {
    class Header : public ChocolateDoomLauncher::View {
        public:
            Header(std::string title, bool showIcon);
            ~Header();
            void onTick(SDL_Rect rect, double dTime);
            void onRender(SDL_Rect rect, double dTime);

        private:
            Image * _iconImage = NULL;
            Text * _titleText = NULL;
            SDL_Texture * _icon[2] = { NULL, NULL };
            bool _showIcon;
            double _timeElapsed = 0;
            int _currentIcon = 0;
        
    };
}
