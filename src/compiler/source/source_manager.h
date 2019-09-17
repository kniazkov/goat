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

#include "source.h"
#include <vector>
#include <string>

namespace g0at
{
    class source_manager
    {
    public:
        source_manager();
        source * create_source_from_string(std::wstring data);
        source * create_source_from_file(std::string file_name);
        std::wstring get_fragment(int begin, int end);
        std::wstring get_fragment_by_index(int index);

    protected:
        struct item
        {
            lib::pointer<source> src;
            int offset;
        };

        void add_source(lib::pointer<source> src);
        item * find_item(int index);

        std::vector<item> list;
        int last_offset;

    private:
        source_manager(const source_manager &) { }
        void operator=(const source_manager &) { }
    };
};
