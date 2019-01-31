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

#include "source_file.h"
#include "global/global.h"
#include <string>
#include <fstream>
#include <streambuf>
#include <assert.h>

namespace g0at
{
    class source_file_position : public position
    {
    public:
        source_file_position(int _index)
            : index(_index)
        {
        }

        int get_index() override
        {
            return index;
        }

        void print(std::wostream &stream) override
        {
            stream << index;
        }

    protected:
        int index;
    };

    source_file::source_file(char *_file_name)
    {
        // TODO: exceptions
        assert(_file_name != nullptr);
        std::ifstream stream(_file_name);
        std::string temp;
        stream.seekg(0, std::ios::end);
        temp.reserve(stream.tellg());
        stream.seekg(0, std::ios::beg);
        temp.assign((std::istreambuf_iterator<char>(stream)),
                    std::istreambuf_iterator<char>());
        data = global::char_encoder->decode(temp);
        file_name = _file_name;
        index = 0;
        max_index = data.size();
    }

    wchar_t source_file::get_char()
    {
        return index < max_index ? data[index] : L'\0';
    }

    wchar_t source_file::next()
    {
        if (index < max_index)
        {
            index++;
            return index < max_index ? data[index] : L'\0';
        }
        return L'\0';
    }

    lib::pointer<position> source_file::get_position()
    {
        return new source_file_position(index);
    }
};
