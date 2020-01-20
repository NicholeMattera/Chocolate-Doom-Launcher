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

#include "Image.hpp"
#include "../Application.hpp"

namespace ChocolateDoomLauncher::Views {
    Image::Image(SDL_Texture * image, ImageMode mode) {
        _image = image;
        _mode = mode;
        
        SDL_QueryTexture(_image, NULL, NULL, &_width, &_height);
    }

    void Image::onRender(SDL_Rect rect, double dTime) {
        if (_mode == CENTERED) {
            SDL_Rect textureFrame = { rect.x + ((rect.w - _width) / 2), rect.y + ((rect.h - _height) / 2), _width, _height };
            SDL_RenderCopy(Application::renderer, _image, NULL, &textureFrame);
        }
        else if (_mode == TILE) {
            for (int x = rect.x; x <= (rect.w / _width + 1) * _width; x += _width) {
                for (int y = rect.y; y <= (rect.h / _height + 1) * _height; y += _height) {
                    SDL_Rect textureFrame = { x, y, _width, _height };
                    SDL_RenderCopy(Application::renderer, _image, NULL, &textureFrame);
                }
            }
        }
    }

    void Image::setImage(SDL_Texture * image) {
        _image = image;
        SDL_QueryTexture(_image, NULL, NULL, &_width, &_height);
        requiresRendering = true;
    }

    void Image::setMode(ImageMode mode) {
        _mode = mode;
        requiresRendering = true;
    }
}
