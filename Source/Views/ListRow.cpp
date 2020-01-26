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
#include "../Services/Theme.hpp"

namespace ChocolateDoomLauncher::Views {
    ListRow::ListRow(ListRowStyle style) {
        auto fm = Managers::Font::Instance();

        _primaryText = new Text(fm->getFont(StandardFont, 23), "", Services::Theme::text);
        if (style != SUBTITLE) {
            _primaryText->frame = { 20, 23, 840, 24 };
        } else {
            _primaryText->frame = { 20, 10, 840, 24 };
        }
        addSubView(_primaryText);

        if (style != DEFAULT) {
            if (style == BOOLEAN) {
                _secondaryText = new Text(fm->getFont(StandardFont, 18), "Off", Services::Theme::disabled_text);
            } else {
                _secondaryText = new Text(fm->getFont(StandardFont, 18), "", (style == SUBTITLE) ? Services::Theme::disabled_text : Services::Theme::active_text);
            }

            if (style != SUBTITLE) {
                _secondaryText->textAlignment = RIGHT_ALIGN;
                _secondaryText->frame = { 20, 29, 840, 19 };
            } else {
                _secondaryText->textAlignment = LEFT_ALIGN;
                _secondaryText->frame = { 20, 41, 840, 19 };
            }

            addSubView(_secondaryText);
        }
    }

    ListRow::~ListRow() {
        if (_primaryText != NULL) {
            delete _primaryText;
        }

        if (_secondaryText != NULL) {
            delete _secondaryText;
        }
    }

    void ListRow::onRender(SDL_Rect rect, double dTime) {
        if (index != 0) {
            // Divider
            auto divider = Services::Theme::list_divider;
            SDL_SetRenderDrawColor(Application::renderer, divider.r, divider.g, divider.b, divider.a);
            SDL_RenderDrawLine(Application::renderer, rect.x + 5, 0, rect.x + rect.w - 10, 0);
        }

        Control::onRender(rect, dTime);
    }

    void ListRow::setPrimaryText(std::string text) {
        if (_primaryText != NULL) {
            _primaryText->setText(text);
        }
    }

    std::string ListRow::getPrimaryText() {
        return (_primaryText == NULL) ? "" : _primaryText->text;
    }

    void ListRow::setSecondaryText(std::string text) {
        if (_secondaryText != NULL) {
            _secondaryText->setText(text);
        }
    }

    std::string ListRow::getSecondaryText() {
        return (_secondaryText == NULL) ? "" : _secondaryText->text;
    }
}
