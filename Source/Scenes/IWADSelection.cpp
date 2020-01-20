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
 
#include <limits.h>
#include <SDL2/SDL_image.h>

#include "Error.hpp"
#include "IWADSelection.hpp"
#include "PWADSelection.hpp"
#include "../Application.hpp"
#include "../Constants.hpp"
#include "../Models/FooterAction.hpp"
#include "../Managers/Theme.hpp"
#include "../Services/Doom.hpp"
#include "../Services/File.hpp"

namespace ChocolateDoomLauncher::Scenes {
    IWADSelection::IWADSelection() {
        auto tm = Managers::Theme::Instance();

        background = tm->background;
        _wads = Services::Doom::getWADSInDir(Services::File::currentWorkingDirectory() + "/wads", IWAD);

        auto backgroundSurface = IMG_Load("romfs:/Background.png");
        _backgroundTexture = SDL_CreateTextureFromSurface(Application::renderer, backgroundSurface);
        SDL_FreeSurface(backgroundSurface);

        _background = new Views::Image(_backgroundTexture, TILE);
        _background->frame = { 0, 0, 1280, 720 };
        addSubView(_background);

        _header = new Views::Header("Choose a IWAD", true);
        _header->frame = { 0, 0, 1280, 88 };
        addSubView(_header);

        _list = new Views::List(this);
        _list->frame = { 0, 88, 1280, 559 };
        _list->hasFocus = true;
        _list->reload();
        addSubView(_list);

        _footer = new Views::Footer();
        _footer->frame = { 0, 647, 1280, 73 };
        addSubView(_footer);

        auto openIWADAction = new Models::FooterAction();
        openIWADAction->button = A_BUTTON;
        openIWADAction->text = "Open IWAD";
        _footer->addAction(openIWADAction);

        auto openPWADAction = new Models::FooterAction();
        openPWADAction->button = X_BUTTON;
        openPWADAction->text = "Open IWAD with PWAD";
        _footer->addAction(openPWADAction);

        auto quitAction = new Models::FooterAction();
        quitAction->button = B_BUTTON;
        quitAction->text = "Quit";
        _footer->addAction(quitAction);
    }

    IWADSelection::~IWADSelection() {
        if (_backgroundTexture != NULL)
            SDL_DestroyTexture(_backgroundTexture);

        if (_background != NULL)
            delete _background;

        if (_header != NULL)
            delete _header;

        if (_list != NULL)
            delete _list;

        if (_footer != NULL)
            delete _footer;
    }

    void IWADSelection::buttonsDown(u32 buttons, double dTime) {
        if (buttons & KEY_UP) {
            int rowSelected = _list->getSelectedRowIndex();
            if (rowSelected != 0) {
                _list->selectRow(rowSelected - 1);
            }
        }
        else if (buttons & KEY_DOWN) {
            int rowSelected = _list->getSelectedRowIndex();
            if (rowSelected != (int) _wads.size() - 1) {
                _list->selectRow(rowSelected + 1);
            }
        }
        else if (buttons & KEY_A) {
            if (!Services::Doom::loadDoom(_wads.at(_list->getSelectedRowIndex()))) {
                Application::switchScene(new Error("Unable to start Chocolate Doom."));
            }
        }
        else if (buttons & KEY_X) {
            Application::switchScene(new PWADSelection(_wads.at(_list->getSelectedRowIndex())));
        }
        else if (buttons & KEY_B) {
            Application::switchScene(NULL);
        }
    }

    // Views::ListDelegate Methods

    int IWADSelection::numberOfRows(Views::List * list) {
        return _wads.size();
    }

    Views::ListRow * IWADSelection::getRow(Views::List * list, int index) {
        auto row = list->getReusableRow("WadRow");
        if (row == NULL) {
            row = new Views::ListRow("WadRow");
        }

        return row;
    }
}
