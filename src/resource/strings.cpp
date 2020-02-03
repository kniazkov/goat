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

#include "strings.h"

namespace g0at
{
    namespace resource
    {
        std::wstring str_oper_plus = L"+";
        std::wstring str_oper_minus = L"-";
        std::wstring str_oper_plus_plus = L"++";
        std::wstring str_oper_minus_minus = L"--";
        std::wstring str_oper_asterisk = L"*";
        std::wstring str_oper_double_asterisk = L"**";
        std::wstring str_oper_slash = L"/";
        std::wstring str_oper_percent = L"%";
        std::wstring str_oper_exclamation = L"!";
        std::wstring str_oper_double_exclamation = L"!!";
        std::wstring str_oper_tilde = L"~";
        std::wstring str_oper_equals = L"==";
        std::wstring str_oper_not_equal = L"!=";
        std::wstring str_oper_less = L"<";
        std::wstring str_oper_less_equal = L"<=";
        std::wstring str_oper_greater = L">";
        std::wstring str_oper_greater_equal = L">=";
        std::wstring str_oper_double_less = L"<<";
        std::wstring str_oper_double_greater = L">>";
        std::wstring str_oper_triple_greater = L">>>";
        std::wstring str_oper_ampersand = L"&";
        std::wstring str_oper_double_ampersand = L"&&";
        std::wstring str_oper_vertical_bar = L"|";
        std::wstring str_oper_double_vertical_bar = L"||";
        std::wstring str_oper_caret = L"^";
        std::wstring str_oper_inherit = L"->";
        std::wstring str_oper_protect = L"#";

        std::wstring empty_string = L"";
        std::wstring str_instanceof = L"instanceof";
        std::wstring str_flat = L"flat";
        std::wstring str_get = L"get";
        std::wstring str_set = L"set";
        std::wstring str_iterator = L"iterator";
        std::wstring str_keys = L"keys";
        std::wstring str_next = L"next";
        std::wstring str_valid = L"valid";
        std::wstring str_clone = L"clone";
        std::wstring str_length = L"length";
        std::wstring str_run = L"run";
        std::wstring str_delay = L"delay";
        std::wstring str_current = L"current";
        std::wstring str_contains = L"contains";
        std::wstring str_void = L"void";
        std::wstring str_undefined = L"undefined";
        std::wstring str_null = L"null";
        std::wstring str_data = L"data";
        std::wstring str_init = L"init";
        std::wstring str_add = L"add";
        std::wstring str_true = L"true";
        std::wstring str_false = L"false";
        std::wstring str_push = L"push";
        std::wstring str_decode = L"decode";
        std::wstring str_encode = L"encode";
        std::wstring str_file = L"file";
        std::wstring str_read = L"read";
        std::wstring str_write = L"write";
        std::wstring str_flush = L"flush";
        std::wstring str_append = L"append";
        std::wstring str_close = L"close";
        std::wstring str_seek = L"seek";
        std::wstring str_position = L"position";
        std::wstring str_eof = L"eof";
        std::wstring str_getc = L"getc";
        std::wstring str_split = L"split";
        std::wstring str_trim = L"trim";
        std::wstring str_substr = L"substr";
        std::wstring str_valueof = L"valueof";
        std::wstring str_exception = L"exception";
        std::wstring str_string = L"string";
        std::wstring str_lock = L"lock";
        std::wstring str_fill = L"fill";
        std::wstring str_bitwidth = L"bitwidth";
        std::wstring str_inc = L"inc";
        std::wstring str_pulse = L"pulse";

        std::wstring str_Mode = L"Mode";
        std::wstring str_Origin = L"Origin";
        std::wstring str_IllegalArgument = L"IllegalArgument";
        std::wstring str_IllegalContext = L"IllegalContext";
        std::wstring str_IllegalOperation = L"IllegalOperation";
        std::wstring str_DivisionByZero = L"DivisionByZero";
        std::wstring str_IllegalReference = L"IllegalReference";
        std::wstring str_IllegalType = L"IllegalType";
        std::wstring str_IsNotAFunction = L"IsNotAFunction";
        std::wstring str_IsNotAMethod = L"IsNotAMethod";
        std::wstring str_OperatorNotFound = L"OperatorNotFound";
        std::wstring str_UndeclaredVariable = L"UndeclaredVariable";

        std::wstring str_READ = L"READ";
        std::wstring str_WRITE = L"WRITE";
        std::wstring str_APPEND = L"APPEND";
        std::wstring str_FULL = L"FULL";
        std::wstring str_BEGIN = L"BEGIN";
        std::wstring str_END = L"END";
        std::wstring str_CURRENT = L"CURRENT";
    };
};
