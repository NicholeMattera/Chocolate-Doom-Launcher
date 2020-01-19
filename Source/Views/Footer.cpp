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

#include "Footer.hpp"
#include "../Application.hpp"
#include "../Constants.hpp"
#include "../Managers/Font.hpp"
#include "../Managers/Theme.hpp"

namespace ChocolateDoomLauncher::Views {
    Footer::Footer() {
        this->background = { 0, 0, 0, 0 };
    }

    Footer::~Footer() {
        for (auto const& action : actions) {
            if (action->buttonTexture != NULL)
                SDL_DestroyTexture(action->buttonTexture);

            if (action->textTexture != NULL)
                SDL_DestroyTexture(action->textTexture);
            
            delete action;
        }

        actions.clear();
    }

    void Footer::addAction(Models::FooterAction * action) {
        auto fm = Managers::Font::Instance();
        auto tm = Managers::Theme::Instance();

        if (action->buttonTexture == NULL) {
            Uint16 button;

            switch(action->button) {
                case B_BUTTON:
                    button = 0xE0E1;
                    break;

                case X_BUTTON:
                    button = 0xE0E2;
                    break;

                case Y_BUTTON:
                    button = 0xE0E3;
                    break;
                
                default:
                    button = 0xE0E0;
                    break;
            }

            auto buttonSurface = TTF_RenderGlyph_Blended(fm->getFont(NintendoExtFont, 25), button, tm->text);
            action->buttonTexture = SDL_CreateTextureFromSurface(Application::renderer, buttonSurface);
            SDL_FreeSurface(buttonSurface);
        }

        if (action->textTexture == NULL) {
            auto textSurface = TTF_RenderText_Blended(fm->getFont(StandardFont, 23), action->text.c_str(), tm->text);
            action->textTexture = SDL_CreateTextureFromSurface(Application::renderer, textSurface);
            action->textWidth = textSurface->w;
            action->textHeight = textSurface->h;
            SDL_FreeSurface(textSurface);
        }

        actions.push_back(action);
        requiresRendering = true;
    }

    void Footer::onRender(SDL_Rect rect, double dTime) {
        auto tm = Managers::Theme::Instance();

        // Divider
        auto divider = tm->header_footer_divider;
        SDL_SetRenderDrawColor(Application::renderer, divider.r, divider.g, divider.b, divider.a);
        SDL_RenderDrawLine(Application::renderer, rect.x + 30, rect.y, rect.w - 30, rect.y);

        int x = rect.x + rect.w - 60;
        for (auto const & action : actions) {
            x -= action->textWidth;
            SDL_Rect textFrame = { x, rect.y + 25, action->textWidth, action->textHeight };
            SDL_RenderCopy(Application::renderer, action->textTexture, NULL, &textFrame);
            
            x -= 37;
            SDL_Rect iconFrame = { x, rect.y + 25, 25, 25 };
            SDL_RenderCopy(Application::renderer, action->buttonTexture, NULL, &iconFrame);

            x -= 40;
        }
    }
}
