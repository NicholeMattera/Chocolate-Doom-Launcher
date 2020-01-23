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

#include <math.h>

#include "List.hpp"

namespace ChocolateDoomLauncher::Views {
    List::List(ListDelegate * delegate) {
        _delegate = delegate;
    }

    List::~List() {
        _clearCache();
    }

    ListRow * List::getReusableRow() {
        // We haven't hit the maximum number of rows yet.
        if ((int) _rowCache.size() < _maximumNumberOfRows) {
            return NULL;
        }

        ListRow * farthestRow = NULL;
        int farthestOffscreenDistance = 0;
        for (auto const& row : _rowCache) {
            int distance = -1;

            // Is it offscreen and figure out it's distance offscreen.
            if (row->frame.y + row->frame.h < 0) {
                distance = (row->frame.y + row->frame.h) * -1;
            } else if (row->frame.y > frame.h) {
                distance = row->frame.y - frame.h;
            }

            // Lets check if this distance is the farthest.
            if (distance > farthestOffscreenDistance) {
                farthestOffscreenDistance = distance;
                farthestRow = row;
            }
        }

        return farthestRow;
    }

    void List::reload() {
        _rowSelected = 0;

        // Get number of rows.
        _numberOfRows = _delegate->numberOfRows(this);

        // Clear our cached rows.
        _clearCache();

        auto rowHeight = _delegate->getRowHeight();
        _maximumNumberOfRows = ceil(frame.h / rowHeight) + 2;

        // Generate our rows.
        for (int i = 0; i < _numberOfRows; i++) {
            // No need to generate additional rows right now.
            if (rowHeight * i > frame.h) {
                break;
            }

            auto row = _delegate->getRow(this, i);
            row->index = i;
            row->frame = { 200, rowHeight * i, 880, rowHeight };
            row->hasFocus = (hasFocus && i == _rowSelected);
            addSubView(row);

            _rowCache.push_back(row);
        }
    }

    int List::getSelectedRowIndex() {
        return _rowSelected;
    }

    void List::selectRow(int index) {
        int direction = index > _rowSelected ? 1 : -1;
        SDL_Rect bounds = { 0, 0, frame.w, frame.h };
        SDL_Rect intersectedBounds;

        // Change our focus.
        for (auto const& row : _rowCache) {
            row->hasFocus = (hasFocus && row->index == index);

            if (row->index == index) {
                _rowSelected = index;

                // Is it out of bounds?
                SDL_IntersectRect(&row->frame, &bounds, &intersectedBounds);
                if (row->frame.h != intersectedBounds.h) {
                    _yOffset = _yOffset + (row->frame.h - intersectedBounds.h) * direction;
                }
            }
        }

        // See if we didn't find the row in our cache.
        if (_rowSelected != index) {
            auto row = _delegate->getRow(this, index);
            if (row->index == -1) {
                _rowCache.push_back(row);
                addSubView(row);
            }
            row->index = index;
            row->frame = { 200, 0, 880, _delegate->getRowHeight() };
            row->hasFocus = hasFocus;

            _yOffset = _yOffset + row->frame.h * direction;
            _rowSelected = index;
        }

        // Adjust our y coordinates for scrolling.
        for (auto const& row : _rowCache) {
            row->frame.y = row->index * _delegate->getRowHeight() - _yOffset;
        }
    }

    void List::_clearCache() {
        for (auto const& row : _rowCache) {
            delete row;
        }
        _rowCache.clear();
    }
}
