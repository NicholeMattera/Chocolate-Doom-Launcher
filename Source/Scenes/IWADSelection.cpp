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

#include "IWADSelection.hpp"
#include "PWADSelection.hpp"
#include "../Application.hpp"
#include "../Models/FooterAction.hpp"
#include "../Managers/Theme.hpp"
#include "../Services/Doom.hpp"

namespace ChocolateDoomLauncher::Scenes {
    IWADSelection::IWADSelection() {
        auto tm = Managers::Theme::Instance();

        background = tm->background;

        auto backgroundSurface = IMG_Load("romfs:/Background.png");
        _background = SDL_CreateTextureFromSurface(Application::renderer, backgroundSurface);
        _backgroundWidth = backgroundSurface->w;
        _backgroundHeight = backgroundSurface->h;
        SDL_FreeSurface(backgroundSurface);

        _headerView = new Views::Header("Choose a IWAD", true);
        _headerView->frame = { 0, 0, 1280, 88 };
        addSubView(_headerView);

        _footerView = new Views::Footer();
        _footerView->frame = { 0, 647, 1280, 73 };
        addSubView(_footerView);

        auto openIWADAction = new Models::FooterAction();
        openIWADAction->button = A_BUTTON;
        openIWADAction->text = "Open IWAD";
        _footerView->addAction(openIWADAction);

        auto openPWADAction = new Models::FooterAction();
        openPWADAction->button = X_BUTTON;
        openPWADAction->text = "Open IWAD with PWAD";
        _footerView->addAction(openPWADAction);

        auto quitAction = new Models::FooterAction();
        quitAction->button = B_BUTTON;
        quitAction->text = "Quit";
        _footerView->addAction(quitAction);
    }

    IWADSelection::~IWADSelection() {
        if (_headerView != NULL)
            delete _headerView;

        if (_background != NULL)
            SDL_DestroyTexture(_background);

        if (_footerView != NULL)
            delete _footerView;
    }

    void IWADSelection::buttonsDown(u32 buttons, double dTime) {
        if (buttons & KEY_A) {
            Services::Doom::loadDoom("DOOM2.WAD");
        }
        else if (buttons & KEY_X) {
            Application::switchScene(new PWADSelection());
        }
        else if (buttons & KEY_B) {
            Application::switchScene(NULL);
        }
    }

    void IWADSelection::render(SDL_Rect rect, double dTime) {
        for (int x = 0; x <= (rect.w / _backgroundWidth + 1) * _backgroundWidth; x += _backgroundWidth) {
            for (int y = 0; y <= (rect.h / _backgroundHeight + 1) * _backgroundHeight; y += _backgroundHeight) {
                SDL_Rect textureFrame = { x, y, _backgroundWidth, _backgroundHeight };
                SDL_RenderCopy(Application::renderer, _background, NULL, &textureFrame);
            }
        }

        Scene::render(rect, dTime);
    }
}
