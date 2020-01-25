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

#include "Theme.hpp"

namespace ChocolateDoomLauncher::Managers {
    Theme * Theme::Instance() {
        if (_instance == NULL) {
            _instance = new Theme;
        }

        return _instance;
    }

    Theme::Theme() {
        background = { 0, 0, 0, 255 };
        header_footer_divider = { 255, 255, 255, 255 };
        list_divider = { 77, 77, 77, 255 };
        selected_border_1 = { 255, 0, 0, 255 };
        selected_border_2 = { 67, 0, 0, 255 };
        text = { 255, 255, 255, 255 };
        active_text = { 0, 255, 196, 255 };
        disabled_text = { 128, 128, 128, 255 };
    }
}
