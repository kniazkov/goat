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

#include <string>
#include <cstdint>

namespace g0at
{
    namespace lib
    {
        union uint16_converter
        {
            uint8_t buff[2];
            uint16_t val;
        };

        union int32_converter
        {
            uint8_t buff[4];
            uint32_t val;
        };

        union int64_converter
        {
            uint8_t buff[8];
            uint64_t val;
        };

        union double_converter
        {
            uint8_t buff[8];
            double val;
        };

        union wchar_converter
        {
            uint8_t buff[4];
            wchar_t val;
        };

        std::string file_name_from_full_path(std::string path);
        std::wstring double_to_wstring(double value);
        std::wstring double_to_wstring(double value, int precision);
        bool wstring_to_double(std::wstring wstr, double *result);
        bool wstring_to_int64(std::wstring wstr, int64_t *result, int radix);
        std::wstring int64_to_wstring(int64_t value, int radix);
        std::wstring escape_special_chars(std::wstring src);
        std::wstring escape_special_chars(const wchar_t *src, size_t len);
        std::wstring escape_html_entities(std::wstring src);
        std::wstring escape_html_entities(const wchar_t *src, size_t len);
        char * file_name_postfix(const char *head, const char *tail);
        void dump_file(const char *file_name, const char *postfix, std::wstring data);
        bool file_exists(const char *file_name);
        std::string wstring_to_ascii_string(std::wstring src, bool *okay);
        std::string normalize_file_path(std::string path);
        std::string trim(std::string str);
        std::string get_line();
        bool ends_with(std::string const & value, std::string const & ending);
        uint8_t generate_random_number();
        int64_t get_time_ns();
    };
};