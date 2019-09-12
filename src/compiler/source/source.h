/*

Copyright (C) 17..19 Ivan Kniazkov

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

#include "position.h"
#include "lib/pointer.h"
#include <string>

namespace g0at
{
    class source
    {
    public:
        source();
        virtual ~source();
        virtual wchar_t get_char();
        virtual wchar_t get_char(int offset) = 0;
        virtual wchar_t next() = 0;
        virtual lib::pointer<position> get_position() = 0;
        virtual std::wstring get_data() = 0;
    };
};
