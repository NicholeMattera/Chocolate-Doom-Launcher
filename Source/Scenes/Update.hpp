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
#include "../Views/Image.hpp"
#include "../Views/ProgressBar.hpp"
#include "../Views/Text.hpp"

#pragma once

namespace ChocolateDoomLauncher::Scenes {
    class Update : public Scene {
        public:
            Update();
            ~Update();
            void tick(SDL_Rect rect, double dTime);
            void buttonsDown(u32 buttons, double dTime);

        private:
            SDL_Texture * _backgroundTexture = NULL;

            Views::Image * _background = NULL;
            Views::Header * _header = NULL;
            Views::ProgressBar * _progressBar = NULL;
            Views::Text * _text = NULL;
            Views::Footer * _footer = NULL;

            bool _downloadsComplete = false;
        
            bool _updateLauncher();
            void _updateGame();
            void _onProgressUpdate(double progress);
    };
}
