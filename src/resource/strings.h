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
        extern std::wstring str_oper_less_equal;
        extern std::wstring str_oper_greater;
        extern std::wstring str_oper_greater_equal;
        extern std::wstring str_oper_double_less;
        extern std::wstring str_oper_double_greater;
        extern std::wstring str_oper_triple_greater;
        extern std::wstring str_oper_ampersand;
        extern std::wstring str_oper_double_ampersand;
        extern std::wstring str_oper_vertical_bar;
        extern std::wstring str_oper_double_vertical_bar;
        extern std::wstring str_oper_caret;
        extern std::wstring str_oper_inherit;
        extern std::wstring str_oper_protect;

        extern std::wstring empty_string;
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
        extern std::wstring str_true;
        extern std::wstring str_false;
        extern std::wstring str_push;
        extern std::wstring str_decode;
        extern std::wstring str_encode;
        extern std::wstring str_file;
        extern std::wstring str_read;
        extern std::wstring str_write;
        extern std::wstring str_append;
        extern std::wstring str_close;
        extern std::wstring str_seek;
        extern std::wstring str_position;
        extern std::wstring str_eof;
        extern std::wstring str_getc;
        extern std::wstring str_split;
        extern std::wstring str_substr;
        extern std::wstring str_valueof;
        extern std::wstring str_exception;

        extern std::wstring str_Mode;
        extern std::wstring str_Origin;
        extern std::wstring str_IllegalArgument;
        extern std::wstring str_IllegalContext;
        extern std::wstring str_IllegalOperation;
        extern std::wstring str_DivisionByZero;
        extern std::wstring str_IllegalReference;

        extern std::wstring str_READ;
        extern std::wstring str_WRITE;
        extern std::wstring str_APPEND;
        extern std::wstring str_FULL;
        extern std::wstring str_BEGIN;
        extern std::wstring str_END;
        extern std::wstring str_CURRENT;
    };
};
