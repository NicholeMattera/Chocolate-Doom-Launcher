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
#include <switch.h>

#pragma once

namespace ChocolateDoomLauncher::Managers {
    class Theme {
        public:
            SDL_Color background;
            SDL_Color header_footer_divider;
            SDL_Color list_divider;
            SDL_Color selected_background;
            SDL_Color selected_border_1;
            SDL_Color selected_border_2;
            SDL_Color text;
            SDL_Color active_text;

            static Theme * Instance();
        
        private:
            static inline Theme * _instance = NULL;

            Theme();
            Theme(Theme const&){};

    };
}
