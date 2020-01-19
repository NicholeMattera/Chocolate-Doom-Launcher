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

#include "Scene.hpp"

namespace ChocolateDoomLauncher {
    void Scene::render(SDL_Rect rect, double dTime) {
        for (auto const& view : subviews) {
            if (!view->isHidden) {
                SDL_Rect subviewFrame = view->frame;
                view->render({ rect.x + subviewFrame.x, rect.y + subviewFrame.y, subviewFrame.w, subviewFrame.h }, dTime);
            }
        }
    }

    void Scene::addSubView(View * view) {
        view->superview = NULL;
        subviews.push_back(view);
    }
    void Scene::removeSubView(View * view) {
        view->superview = NULL;
        subviews.push_back(view);
    }
}
