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

#include "Font.hpp"
#include "../Constants.hpp"

namespace ChocolateDoomLauncher::Managers {
    Font * Font::Instance() {
        if (_instance == NULL) {
            _instance = new Font;
        }

        return _instance;
    }

    TTF_Font * Font::getFont(std::string name, u8 size) {
        auto fontIt = _fontCache.find(name + "_" + std::to_string(size));
        if (fontIt != _fontCache.end())
            return fontIt->second;

        PlFontData fontData;

        auto fontDataIt = _fontData.find(name);
        if (fontDataIt == _fontData.end()) {
            fontData = _getFontData(name);
            _fontData.insert(std::pair<std::string, PlFontData>(name, fontData));
        } else
            fontData = fontDataIt->second;

        auto font = TTF_OpenFontRW(SDL_RWFromMem(fontData.address, fontData.size), 1, size);
        _fontCache.insert(std::pair<std::string, TTF_Font *>(name + "_" + std::to_string(size), font));

        return font;
    }

    void Font::dealloc() {
        for (auto it = _fontCache.begin(); it != _fontCache.end(); ++it) {
            TTF_CloseFont(it->second);
        }

        _fontCache.clear();
        _fontData.clear();
    }

    PlFontData Font::_getFontData(std::string name) {
        PlFontData data;
        if (name == ChineseSimplifiedFont) {
            plGetSharedFontByType(&data, PlSharedFontType_ChineseSimplified);
        }
        else if (name == ExtChineseSimplifiedFont) {
            plGetSharedFontByType(&data, PlSharedFontType_ExtChineseSimplified);
        }
        else if (name == ChineseTraditionalFont) {
            plGetSharedFontByType(&data, PlSharedFontType_ChineseTraditional);
        }
        else if (name == KOFont) {
            plGetSharedFontByType(&data, PlSharedFontType_KO);
        }
        else if (name == NintendoExtFont) {
            plGetSharedFontByType(&data, PlSharedFontType_NintendoExt);
        }
        else if (name == TotalFont) {
            plGetSharedFontByType(&data, PlSharedFontType_Total);
        }
        else {
            plGetSharedFontByType(&data, PlSharedFontType_Standard);
        }

        return data;
    }
}
