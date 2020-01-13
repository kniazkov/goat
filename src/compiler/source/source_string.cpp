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

#include "source_string.h"
#include "lib/assert.h"

namespace g0at
{
    class source_string_position : public position
    {
    public:
        source_string_position(int _relative_position, int _absolute_position)
            : relative_position(_relative_position), absolute_position(_absolute_position)
        {
        }

        int get_absolute_position() override
        {
            return absolute_position;
        }

        std::wstring to_string() override
        {
            return std::to_wstring(relative_position);
        }

    protected:
        int relative_position;
        int absolute_position;
    };

    source_string::source_string(std::wstring _data, int _offset)
    {
        data = _data;
        index = 0;
        max_index = (int)data.size();
        cached_position_index = -1;
        offset = _offset;
    }

    wchar_t source_string::get_char()
    {
        return index < max_index ? data[index] : L'\0';
    }

    wchar_t source_string::get_char(int offset)
    {
        return index + offset < max_index ? data[index + offset] : L'\0';
    }

    wchar_t source_string::next()
    {
        if (index < max_index)
        {
            index++;
            return index < max_index ? data[index] : L'\0';
        }
        return L'\0';
    }

    lib::pointer<position> source_string::get_position()
    {
        if (index == cached_position_index)
            return cached_position;

        cached_position_index = index;
        cached_position = new source_string_position(index, index + offset);
        return cached_position;
    }

    lib::pointer<position> source_string::get_position(int absolute_position)
    {
        int relative_position = absolute_position - offset;
        assert(relative_position >= 0 && relative_position < (int)data.size());
        return new source_string_position(relative_position, absolute_position);
    }

    std::wstring &source_string::get_data()
    {
        return data;
    }

    lib::pointer<breakpoint> source_string::set_breakpoint(std::string request)
    {
        // we can't set breakpoints on string sources
        return nullptr;
    }
};
