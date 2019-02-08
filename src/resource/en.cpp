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

#include "resource.h"
#include <sstream>

namespace g0at
{
    namespace resource
    {
        class en : public resource
        {
        public:
            static resource *get_instance()
            {
                static en instance;
                return &instance;
            }

            std::wstring bad_utf8() override
            {
                return L"input data is not utf-8 encoded";
            }

            std::wstring unknown_character(wchar_t ch) override
            {
                std::wstringstream wss;
                wss << L"unknown character '" << ch << L'\'';
                return wss.str();
            }

            std::wstring no_input_file() override
            {
                return L"no input file";
            }

            std::wstring file_not_found(const char *file_name) override
            {
                std::wstringstream wss;
                wss << L"file \'" << file_name << L"\' not found";
                return wss.str();
            }

            std::wstring missing_closing_quote() override
            {
                return L"missing a closing quote";
            }

            std::wstring invalid_escape_sequence(wchar_t ch) override
            {
                std::wstringstream wss;
                wss << L"invalid escape sequence '\\" << ch << L'\'';
                return wss.str();
            }

            std::wstring incorrect_command_line_parameter(const char *parameter) override
            {
                std::wstringstream wss;
                wss << L"incorrect command line parameter: '" << parameter << L'\'';
                return wss.str();
            }

            std::wstring unable_to_parse_token_sequence() override
            {
                return L"unable to parse token sequence";
            }

            std::wstring invald_lvalue_expression() override
            {
                return L"invalid lvalue expression for the assingment operator";
            }

            std::wstring expected_an_expression_after_operator() override
            {
                return L"expected an expression after operator";
            }
        };

        resource *resource::get_intance_en()
        {
            return en::get_instance();
        }
    };
};