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
        std::wstring str_oper_tilde = L"~";
        std::wstring str_oper_equals = L"==";
        std::wstring str_oper_not_equal = L"!=";

        std::wstring str_instanceof = L"instanceof";
        std::wstring str_flat = L"flat";
        std::wstring str_get = L"get";
        std::wstring str_set = L"set";
        std::wstring str_iterator = L"iterator";
        std::wstring str_next = L"next";
        std::wstring str_valid = L"valid";
        std::wstring str_clone = L"clone";
    };
};
