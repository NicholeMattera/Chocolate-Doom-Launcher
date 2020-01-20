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

#include <SDL2/SDL2_gfxPrimitives.h>

#include "Control.hpp"
#include "../Application.hpp"
#include "../Managers/Theme.hpp"

namespace ChocolateDoomLauncher::Views {
    Control::Control() {
        auto tm = Managers::Theme::Instance();

        _firstColor = tm->selected_border_1;
        _secondColor = tm->selected_border_2;
    }
    
    void Control::onTick(SDL_Rect rect, double dTime) {
        if (hasFocus) {
            if (_timeElapsed == -1) {
                _timeElapsed = dTime;
            }
            else {
                _timeElapsed += dTime;
            }

            if (_timeElapsed > 2000) {
                _timeElapsed -= 2000;
            }

            // Color1 -> Color2
            if (floor(((int) _timeElapsed) / 1000) == 0) {
                double time = (_timeElapsed / 1000) * -1;
                _borderColor.r = _firstColor.r + floor((_firstColor.r - _secondColor.r) * time);
                _borderColor.g = _firstColor.g + floor((_firstColor.g - _secondColor.g) * time);
                _borderColor.b = _firstColor.b + floor((_firstColor.b - _secondColor.b) * time);
                _borderColor.a = _firstColor.a + floor((_firstColor.a - _secondColor.a) * time);
            }
            // Color2 -> Color1
            else {
                double time = ((_timeElapsed - 1000) / 1000) * -1;
                _borderColor.r = _secondColor.r + floor((_secondColor.r - _firstColor.r) * time);
                _borderColor.g = _secondColor.g + floor((_secondColor.g - _firstColor.g) * time);
                _borderColor.b = _secondColor.b + floor((_secondColor.b - _firstColor.b) * time);
                _borderColor.a = _secondColor.a + floor((_secondColor.a - _firstColor.a) * time);
            }

            requiresRendering = true;
        }
        else if (_timeElapsed != -1) {
            _timeElapsed = -1;
            requiresRendering = true;
        }
    }

    void Control::onRender(SDL_Rect rect, double dTime) {
        if (hasFocus) {
            // Top
            thickLineRGBA(
                    Application::renderer,
                    rect.x, rect.y + 2, rect.x + rect.w, rect.y + 2,
                    4,
                    _borderColor.r, _borderColor.g, _borderColor.b, _borderColor.a);

            // Right
            thickLineRGBA(
                    Application::renderer,
                    rect.x + rect.w - 2, rect.y + 2, rect.x + rect.w - 2, rect.y + rect.h - 2,
                    4,
                    _borderColor.r, _borderColor.g, _borderColor.b, _borderColor.a);

            // Bottom
            thickLineRGBA(
                    Application::renderer,
                    rect.x + rect.w, rect.y + rect.h - 2, rect.x, rect.y + rect.h - 2,
                    4,
                    _borderColor.r, _borderColor.g, _borderColor.b, _borderColor.a);

            // Left
            thickLineRGBA(
                    Application::renderer,
                    rect.x + 2, rect.y + rect.h - 2, rect.x + 2, rect.y + 2,
                    4,
                    _borderColor.r, _borderColor.g, _borderColor.b, _borderColor.a);
        }
    }
}
