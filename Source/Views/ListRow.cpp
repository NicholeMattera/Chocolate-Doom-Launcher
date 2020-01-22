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

#include <stdlib.h> 

#include "ListRow.hpp"
#include "../Application.hpp"
#include "../Managers/Font.hpp"
#include "../Managers/Theme.hpp"

namespace ChocolateDoomLauncher::Views {
    ListRow::ListRow() {
        auto tm = Managers::Theme::Instance();
        auto fm = Managers::Font::Instance();

        _titleText = new Text(fm->getFont(StandardFont, 23), "", tm->text);
        _titleText->frame = { 20, 23, 840, 24 };
        addSubView(_titleText);
    }

    void ListRow::onRender(SDL_Rect rect, double dTime) {
        auto tm = Managers::Theme::Instance();

        if (index != 0) {
            // Divider
            auto divider = tm->list_divider;
            SDL_SetRenderDrawColor(Application::renderer, divider.r, divider.g, divider.b, divider.a);
            SDL_RenderDrawLine(Application::renderer, rect.x + 5, 0, rect.x + rect.w - 10, 0);
        }

        Control::onRender(rect, dTime);
    }

    void ListRow::setTitle(std::string text) {
        _titleText->setText(text);
    }

    std::string ListRow::getTitle() {
        return _titleText->text;
    }
}
