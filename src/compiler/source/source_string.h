/*

Copyright (C) 2017-2020 Ivan Kniazkov

This file is part of interpreter of programming language
codenamed "Goat" ("Goat interpreter").

Goat interpreter is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Goat interpreter is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with Goat interpreter.  If not, see <http://www.gnu.org/licenses/>.

*/

#pragma once

#include "source.h"
#include <string>

namespace g0at
{
    class source_manager;

    class source_string : public source
    {
        friend class source_manager;
    public:
        wchar_t get_char() override;
        wchar_t get_char(int offset) override;
        wchar_t next() override;
        lib::pointer<position> get_position() override;
        lib::pointer<position> get_position(int absolute_position) override;
        std::wstring &get_data() override;
        lib::pointer<breakpoint> set_breakpoint(std::string request) override;

    protected:
        source_string(std::wstring _data, int _offset);
        std::wstring data;
        int index;
        int max_index;
        lib::pointer<position> cached_position;
        int cached_position_index;
        int offset;
    };
};
