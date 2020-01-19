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

#include "../Scene.hpp"
#include "../Views/Footer.hpp"
#include "../Views/Header.hpp"

#pragma once

namespace ChocolateDoomLauncher::Scenes {
    class IWADSelection : public ChocolateDoomLauncher::Scene {
        public:
            IWADSelection();
            ~IWADSelection();

            void buttonsDown(u32 buttons, double dTime);
            void render(SDL_Rect rect, double dTime);

        private:
            SDL_Texture * _background = NULL;
            int _backgroundWidth = 0;
            int _backgroundHeight = 0;

            Views::Header * _headerView = NULL;
            Views::Footer * _footerView = NULL;
        
    };
}
