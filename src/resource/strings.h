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

namespace g0at
{
    namespace resource
    {
        extern std::wstring str_oper_plus;
        extern std::wstring str_oper_minus;
        extern std::wstring str_oper_plus_plus;
        extern std::wstring str_oper_minus_minus;
        extern std::wstring str_oper_asterisk;
        extern std::wstring str_oper_double_asterisk;
        extern std::wstring str_oper_slash;
        extern std::wstring str_oper_percent;
        extern std::wstring str_oper_exclamation;
        extern std::wstring str_oper_double_exclamation;
        extern std::wstring str_oper_tilde;
        extern std::wstring str_oper_equals;
        extern std::wstring str_oper_not_equal;
        extern std::wstring str_oper_less;
        extern std::wstring str_oper_double_less;
        extern std::wstring str_oper_double_greater;
        extern std::wstring str_oper_triple_greater;

        extern std::wstring str_instanceof;
        extern std::wstring str_flat;
        extern std::wstring str_get;
        extern std::wstring str_set;
        extern std::wstring str_iterator;
        extern std::wstring str_next;
        extern std::wstring str_valid;
        extern std::wstring str_clone;
        extern std::wstring str_length;
        extern std::wstring str_run;
        extern std::wstring str_current;
        extern std::wstring str_contains;
        extern std::wstring str_void;
        extern std::wstring str_undefined;
        extern std::wstring str_null;
        extern std::wstring str_data;
        extern std::wstring str_init;
        extern std::wstring str_add;
    };
};
