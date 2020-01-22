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

#include <map>
#include <string>
#include <vector>

#include "ListRow.hpp"
#include "../View.hpp"

#pragma once

namespace ChocolateDoomLauncher::Views {
    class List;

    class ListDelegate {
        public:
            virtual int numberOfRows(List * list) { return 0; };
            virtual int getRowHeight() { return 70; };
            virtual ListRow * getRow(Views::List * list, int index) { return NULL; };
    };

    class List : public View {
        public:
            List(ListDelegate * delegate);
            ~List();
            ListRow * getReusableRow();
            void reload();
            int getSelectedRowIndex();
            void selectRow(int index);

        private:
            void _clearCache();

            ListDelegate * _delegate = NULL;
            int _numberOfRows = 0;
            int _maximumNumberOfRows = 0;
            int _rowSelected = 0;
            int _yOffset = 0;
            std::vector<ListRow *> _rowCache;
        
    };
}
