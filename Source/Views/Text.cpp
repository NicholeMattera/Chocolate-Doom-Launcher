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

#include <sstream>

#include "Text.hpp"
#include "../Application.hpp"

namespace ChocolateDoomLauncher::Views {
    Text::Text(TTF_Font * theFont, std::string theText, SDL_Color theTextColor) : View() {
        this->lineHeight = TTF_FontHeight(theFont);
        
        this->font = theFont;
        this->text = theText;
        this->textColor = theTextColor;
        _reset();
    }

    Text::~Text() {
        for (auto const& textLine : this->_textLines) {
            SDL_DestroyTexture(textLine->texture);
        }
        this->_textLines.clear();
    }

    void Text::onRender(SDL_Rect rect, double dTime) {
        int y = rect.y;
        for (auto const& textLine : this->_textLines) {
            int x = 0;
            int width = std::max(textLine->width, rect.w);
            switch (textAlignment) {
                case LEFT_ALIGN:
                    x = rect.x;
                    break;

                case CENTER_ALIGN:
                    x = rect.x + (width - textLine->width) / 2;
                    break;

                case RIGHT_ALIGN:
                    x = rect.x + width - textLine->width;
                    break;
            }

            SDL_Rect textFrame = { x, y, textLine->width, textLine->height };
            SDL_RenderCopy(Application::renderer, textLine->texture, NULL, &textFrame);

            y += lineHeight;
        }
    }

    int Text::getTextHeight() {
        return _textLines.size() * lineHeight;
    }

    void Text::setFont(TTF_Font * theFont) {
        font = theFont;
        _reset();
    }

    void Text::setText(std::string theText) {
        text = theText;
        _reset();
    }

    void Text::setTextColor(SDL_Color theTextColor) {
        textColor = theTextColor;
        _reset();
    }

    void Text::_reset() {
        for (auto const& textLine : this->_textLines) {
            SDL_DestroyTexture(textLine->texture);
        }
        this->_textLines.clear();

        auto textStream = std::stringstream(text);
        std::string text;

        while(getline(textStream, text, '\n')) {
            auto textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
            if (textSurface != NULL) {
                auto textLine = new Models::TextLine();
                textLine->texture = SDL_CreateTextureFromSurface(Application::renderer, textSurface);
                textLine->width = textSurface->w;
                textLine->height = textSurface->h;

                SDL_FreeSurface(textSurface);

                this->_textLines.push_back(textLine);
            }
        }
        
        requiresRendering = true;
    }
}
