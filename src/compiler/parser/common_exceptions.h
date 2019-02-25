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

#include "compiler/common/compilation_error.h"
#include "global/global.h"

namespace g0at
{
    namespace parser
    {
        class unable_to_parse_token_sequence : public compilation_error
        {
        public:
            unable_to_parse_token_sequence(lib::pointer<position> pos)
                : compilation_error(pos, global::resource->unable_to_parse_token_sequence())
            {
            }
        };

        class expected_an_expression_after_operator : public compilation_error
        {
        public:
            expected_an_expression_after_operator(lib::pointer<position> pos)
                : compilation_error(pos, global::resource->expected_an_expression_after_operator())
            {
            }
        };

        class expected_an_identifier : public compilation_error
        {
        public:
            expected_an_identifier(lib::pointer<position> pos)
                : compilation_error(pos, global::resource->expected_an_identifier())
            {
            }
        };

        class the_next_token_must_be_a_comma_or_a_semicolon : public compilation_error
        {
        public:
            the_next_token_must_be_a_comma_or_a_semicolon(lib::pointer<position> pos)
                : compilation_error(pos, global::resource->the_next_token_must_be_a_comma_or_a_semicolon())
            {
            }
        };

        class expected_an_expression : public compilation_error
        {
        public:
            expected_an_expression(lib::pointer<position> pos)
                : compilation_error(pos, global::resource->expected_an_expression())
            {
            }
        };

        class function_arguments_must_be_separated_by_commas : public compilation_error
        {
        public:
            function_arguments_must_be_separated_by_commas(lib::pointer<position> pos)
                : compilation_error(pos, global::resource->function_arguments_must_be_separated_by_commas())
            {
            }
        };

        class expected_an_argument_list : public compilation_error
        {
        public:
            expected_an_argument_list(lib::pointer<position> pos)
                : compilation_error(pos, global::resource->expected_an_argument_list())
            {
            }
        };

        class expected_a_function_body : public compilation_error
        {
        public:
            expected_a_function_body(lib::pointer<position> pos)
                : compilation_error(pos, global::resource->expected_a_function_body())
            {
            }
        };

        class the_next_token_must_be_a_semicolon : public compilation_error
        {
        public:
            the_next_token_must_be_a_semicolon(lib::pointer<position> pos)
                : compilation_error(pos, global::resource->the_next_token_must_be_a_semicolon())
            {
            }
        };

        class the_next_token_must_be_a_colon : public compilation_error
        {
        public:
            the_next_token_must_be_a_colon(lib::pointer<position> pos)
                : compilation_error(pos, global::resource->the_next_token_must_be_a_colon())
            {
            }
        };
    };
};
