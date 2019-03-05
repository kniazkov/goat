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

#if 1
#include "global/global.h"
#include <iostream> 
#include <string> 
namespace g0at
{
    void dbg_print()
    {
        std::cout << std::endl;
    }

    void dbg_print(std::wstring value)
    {
        std::cout << global::char_encoder->encode(value);
    }

    void dbg_print(const wchar_t *value)
    {
        dbg_print(std::wstring(value));
    }

    void dbg_print(int value)
    {
        dbg_print(std::to_wstring(value));
    }

    void dbg_print(long int value)
    {
        dbg_print(std::to_wstring(value));
    }

    void dbg_print(long long int value)
    {
        dbg_print(std::to_wstring(value));
    }

    void dbg_print(unsigned int value)
    {
        dbg_print(std::to_wstring(value));
    }

    void dbg_print(unsigned long int value)
    {
        dbg_print(std::to_wstring(value));
    }

    void dbg_print(unsigned long long int value)
    {
        dbg_print(std::to_wstring(value));
    }

    void dbg_print(float value)
    {
        dbg_print(std::to_wstring(value));
    }

    void dbg_print(double value)
    {
        dbg_print(std::to_wstring(value));
    }

    template <typename Head, typename... Tail> void dbg_print(Head head, Tail... tail)
    {
        dbg_print(head);
        dbg_print(tail...);
    }
};
#else
#define dbg_print(...) ;
#endif
