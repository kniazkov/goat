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

#include "utils.h"
#include <sstream>
#include <cfloat>

namespace g0at
{
    namespace lib
    {
        const char *file_name_from_full_path(const char *path)
        {
            if (!path)
                return nullptr;

            const char *begin = path;
            while (*path)
            {
                if (*path == '\\' || *path == '/')
                    begin = path + 1;
                path++;
            }
            return begin;
        }

        std::wstring double_to_wstring(double value)
        {
            return double_to_wstring(value, 13);
        }

        std::wstring double_to_wstring(double value, int precision)
        {
            // really, there must be an easier way to do it (and this is NOT std::to_wstring())
            std::stringstream buff;
            bool neg = false;
            if (value < 0)
            {
                neg = true;
                value = -value;
            }
            int64_t factor = 1;
            if (precision > DBL_DIG - 2) precision = DBL_DIG - 2;
            for (int i = 0; i < precision; i++)
            {
                factor *= 10;
            }
            int64_t ip = (long int)value;
            double dfp = value * factor - ip * factor;
            int64_t fp = (long int)dfp;
            if (fp > 0)
            {
                bool flag = false;
                while(precision > 0)
                {
                    char v = fp % 10;
                    if (v != 0)
                        flag = true;
                    if (flag)
                    {
                        char c = v + '0';
                        buff << c;
                    }
                    fp = fp / 10;
                    precision--;
                };
                buff << '.';
            }
            do
            {
                char c = ip % 10 + '0';
                buff << c;
                ip = ip / 10;
            } while(ip != 0);
            if (neg)
            {
                buff << '-';
            }
            std::string str = buff.str();
            std::wstring result;
            result.reserve(str.length());
            result.assign(str.rbegin(), str.rend());
            return result;
        }
    };

};