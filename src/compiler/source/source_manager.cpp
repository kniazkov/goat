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

#include "source_manager.h"
#include "source_string.h"
#include "source_file.h"
#include "lib/assert.h"
#include <sstream>

namespace g0at
{
    source_manager::source_manager()
        : last_offset(0)
    {
    }

    source * source_manager::create_source_from_string(std::wstring data)
    {
        lib::pointer<source> src = new source_string(data, last_offset);
        add_source(src);
        return src.get();
    }

    source * source_manager::create_source_from_file(std::string file_name)
    {
        lib::pointer<source> src = new source_file(file_name, last_offset);
        add_source(src);
        return src.get();
    }

    void source_manager::add_source(lib::pointer<source> src)
    {
        item it;
        it.src = src;
        it.offset = last_offset;
        list.push_back(it);
        last_offset += (int)src->get_data().size();
    }

    source_manager::item * source_manager::find_item(int index)
    {
        int k, n;
        for (k = 1, n = list.size(); k < n; k++)
        {
            if (list[k].offset > index)
                break;
        }
        return &list[k - 1];
    }

    std::wstring source_manager::get_fragment(int begin, int end)
    {
        int n, a, b;
        if (begin >= last_offset)
            begin = last_offset - 1;
        assert(begin >= 0);
        if (end < begin)
            end = begin;
        item *it = find_item(begin);
        std::wstring &data = it->src->get_data();
        a = begin - it->offset; // real offset
        n = end - begin;
        if (n > max_width)
            n = max_width;
        b = a; // end
        while(b - a < n)
        {
            if (data[b] == '\r' || data[b] == '\n')
                break;
            b++;
        }
        return data.substr(a, b - a);
    }

    std::wstring source_manager::get_fragment_by_index(int index)
    {
        int j, n, p, a, b;
        if (index >= last_offset)
            index = last_offset - 1;
        assert(index >= 0);
        item *it = find_item(index);
        std::wstring &data = it->src->get_data();
        p = index - it->offset; // real offset
        n = (int)data.size();
        a = p; // begin
        while(a != 0 && p - a < max_width * 3 / 4)
        {
            if (data[a - 1] == '\r' || data[a - 1] == '\n')
                break;
            a--;
        }
        b = p; // end
        while(b != n && b - a < max_width)
        {
            if (data[b] == '\r' || data[b] == '\n')
                break;
            b++;
        }
        std::wstringstream buff;
        buff << data.substr(a, b - a) << '\n';
        for (j = 0; j < p - a; j++)
            buff << L' ';
        buff << L'^';
        return buff.str();
    }
};
