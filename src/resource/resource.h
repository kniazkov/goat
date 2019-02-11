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
        class resource
        {
        public:
            virtual ~resource() { }
            static resource *get_instance(const char *lang);

            virtual std::wstring bad_utf8() = 0;
            virtual std::wstring unknown_character(wchar_t ch) = 0;
            virtual std::wstring no_input_file() = 0;
            virtual std::wstring file_not_found(const char *file_name) = 0;
            virtual std::wstring missing_closing_quote() = 0;
            virtual std::wstring invalid_escape_sequence(wchar_t ch) = 0;
            virtual std::wstring incorrect_command_line_parameter(const char *parameter) = 0;
            virtual std::wstring unable_to_parse_token_sequence() = 0;
            virtual std::wstring invald_lvalue_expression() = 0;
            virtual std::wstring expected_an_expression_after_operator() = 0;
            virtual std::wstring expected_an_identifier() = 0;
            virtual std::wstring the_next_token_must_be_a_comma_or_a_semicolon() = 0;
            
        protected:
            static resource *get_intance_en();
            static resource *get_intance_ru();
        };
    };
};