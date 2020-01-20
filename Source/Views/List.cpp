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
        for (auto it = _rowCache.begin(); it != _rowCache.end(); ++it) {
            auto rows = it->second;
            for (auto const& row : rows) {
                delete row;
            }
            rows.clear();
        }
        _rowCache.clear();
    }

    ListRow * List::getReusableRow(std::string identifier) {
        return NULL;
    }

    void List::reload() {
        _rowSelected = 0;

        // Get number of rows.
        _numberOfRows = _delegate->numberOfRows(this);

        // Clear our cached rows.
        for (auto it = _rowCache.begin(); it != _rowCache.end(); ++it) {
            auto rows = it->second;
            for (auto const& row : rows) {
                delete row;
            }
            rows.clear();
        }
        _rowCache.clear();

        auto rowHeight = _delegate->getRowHeight();
        int maximumNumberOfRows = ceil(frame.h / rowHeight) + 4;

        int y = 50;
        // Generate our rows.
        for (int i = 0; i < _numberOfRows && i < maximumNumberOfRows; i++) {
            auto row = _delegate->getRow(this, i);
            row->index = i;
            row->frame = { 200, y, 880, rowHeight };
            row->hasFocus = hasFocus && i == _rowSelected;
            addSubView(row);

            auto rowCache = _rowCache.find(row->getIdentifier());
            if (rowCache == _rowCache.end()) {
                std::vector<ListRow *> rows;
                rows.push_back(row);
                _rowCache.insert(std::pair<std::string, std::vector<ListRow *>>(row->getIdentifier(), rows));
            } else {
                rowCache->second.push_back(row);
            }

            y += rowHeight;
        }
    }

    int List::getSelectedRowIndex() {
        return _rowSelected;
    }

    void List::selectRow(int index) {
        _rowSelected = index;
        for (auto it = _rowCache.begin(); it != _rowCache.end(); ++it) {
            auto rows = it->second;
            for (auto const& row : rows) {
                row->hasFocus = (row->index == _rowSelected);
            }
        }
    }
}
