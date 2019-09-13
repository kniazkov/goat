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

#include "all_source_data.h"
#include "lib/assert.h"
#include <sstream>

namespace g0at
{
    all_source_data::all_source_data()
        : last_offset(0)
    {
    }

    void all_source_data::add_data(std::wstring data)
    {
        item i;
        i.data = data;
        i.offset = last_offset;
        items.push_back(i);
        last_offset += (int)data.size();
    }

    std::wstring all_source_data::get_fragment(int begin, int end)
    {
        int k, n, a, b;
        if (begin >= last_offset)
            begin = last_offset - 1;
        assert(begin >= 0);
        if (end < begin)
            end = begin;
        for (k = 1, n = items.size(); k < n; k++)
        {
            if (items[k].offset > begin)
                break;
        }
        k--;
        std::wstring &data = items[k].data;
        a = begin - items[k].offset; // real offset
        n = end - begin;
        if (n > 80)
            n = 80;
        b = a; // end
        while(b - a < n)
        {
            if (data[b] == '\r' || data[b] == '\n')
                break;
            b++;
        }
        return data.substr(a, b - a);
    }

    std::wstring all_source_data::get_fragment_by_index(int index)
    {
        int j, k, n, p, a, b;
        if (index >= last_offset)
            index = last_offset - 1;
        assert(index >= 0);
        for (k = 1, n = items.size(); k < n; k++)
        {
            if (items[k].offset > index)
                break;
        }
        k--;
        std::wstring &data = items[k].data;
        p = index - items[k].offset; // real offset
        n = (int)data.size();
        a = p; // begin
        while(a != 0 && p - a < 60)
        {
            if (data[a - 1] == '\r' || data[a - 1] == '\n')
                break;
            a--;
        }
        b = p; // end
        while(b != n && b - a < 80)
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
