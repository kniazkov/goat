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

#include "source_file.h"
#include "lib/utils.h"
#include "lib/exception.h"
#include <string>
#include <sstream>
#include <fstream>
#include <streambuf>
#include "lib/assert.h"

namespace g0at
{
    class source_file_position : public position
    {
    public:
        source_file_position(std::string _file_name, int _absolute_position, int _row, int _column)
            : file_name(_file_name), absolute_position(_absolute_position), row(_row), column(_column)
        {
        }

        int get_absolute_position() override
        {
            return absolute_position;
        }

        std::wstring to_string() override
        {
            std::wstringstream wss;
            wss << lib::file_name_from_full_path(file_name).c_str() << L", " << row << L"." << column;
            return wss.str();
        }

    protected:
        std::string file_name;
        int absolute_position;
        int row;
        int column;
    };

    class source_file_breakpoint : public breakpoint
    {
    public:
        source_file_breakpoint(std::string _file_name, int _line, int _begin, int _end)
            : file_name(_file_name), line(_line), begin(_begin), end(_end)
        {
        }

        bool triggered(int absolute_position) override
        {
            return absolute_position >= begin && absolute_position < end;
        }

        std::wstring to_string() override
        {
            std::wstringstream wss;
            wss << lib::file_name_from_full_path(file_name).c_str() << L", " << line;
            return wss.str();
        }

    protected:
        std::string file_name;
        int line;
        int begin;
        int end;
    };


    source_file::source_file(std::string _file_name, int _offset)
    {
        std::string temp;
        std::ifstream stream(_file_name);
        if (stream.good() == false)
        {
            throw lib::file_not_found(_file_name);            
        }
        stream.seekg(0, std::ios::end);
        temp.reserve((size_t)stream.tellg());
        stream.seekg(0, std::ios::beg);
        temp.assign((std::istreambuf_iterator<char>(stream)),
                    std::istreambuf_iterator<char>());
        data = global::char_encoder->decode(temp);
        file_name = _file_name;
        index = 0;
        max_index = (int)data.size();
        row  = 1;
        column = 1;
        cached_position_index = -1;
        offset = _offset;
    }

    wchar_t source_file::get_char()
    {
        return index < max_index ? data[index] : L'\0';
    }

    wchar_t source_file::get_char(int offset)
    {
        return index + offset < max_index ? data[index + offset] : L'\0';
    }

    wchar_t source_file::next()
    {
        if (index < max_index)
        {
            if (data[index] == L'\n')
            {
                row++;
                column = 1;
                row_index.push_back(index + 1);
            }
            else
            {
                column++;
            }
            index++;
            return index < max_index ? data[index] : L'\0';
        }
        return L'\0';
    }

    lib::pointer<position> source_file::get_position()
    {
        if (index == cached_position_index)
            return cached_position;

        cached_position_index = index;
        cached_position = new source_file_position(file_name, index + offset, row, column);
        return cached_position;
    }

    lib::pointer<position> source_file::get_position(int absolute_position)
    {
        int relative_position = absolute_position - offset;
        assert(relative_position >= 0 && relative_position < (int)data.size());
        int k, n;
        for (k = 0, n = (int)row_index.size(); k < n; k++)
        {
            if (row_index[k] > relative_position)
                break;
        }
        k--;
        return new source_file_position(file_name, absolute_position, k + 2,
            k < 0 ? relative_position + 1 : relative_position - row_index[k] + 1);
    }

    std::wstring &source_file::get_data()
    {
        return data;
    }

    lib::pointer<breakpoint> source_file::set_breakpoint(std::string request)
    {
        size_t colon = request.find(':');
        if (colon == std::string::npos)
            return nullptr;

        std::string bp_file_name = lib::trim(request.substr(0, colon));

        if (!lib::ends_with(file_name, bp_file_name))
            return nullptr;

        std::string bp_row = lib::trim(request.substr(colon + 1));
        int line = std::atoi(bp_row.c_str());
        if (line < 1 || line - 2 >= (int)row_index.size())
            return nullptr;

        int begin = (line - 2 < 0 ? 0 : row_index.at(line - 2)) + offset;
        int end = (line - 1 == (int)row_index.size() ? max_index : row_index.at(line - 1)) + offset;

        return new source_file_breakpoint(file_name, line, begin, end);
    }
};
