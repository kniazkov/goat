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

#include <vector>
#include <string>

namespace g0at
{
    class all_source_data
    {
    public:
        all_source_data();
        void add_data(std::wstring data);
        std::wstring get_fragment_by_index(int index);

        int get_last_offset() { return last_offset; };

    protected:

        struct item
        {
            std::wstring data;
            int offset;
        };

        std::vector<item> items;
        int last_offset;

    private:
        all_source_data(const all_source_data &) { }
        void operator=(const all_source_data &) { }
    };
};
