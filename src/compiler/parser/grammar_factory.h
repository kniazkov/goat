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

#include "grammar.h"

namespace g0at
{
    namespace parser
    {
        class grammar_factory
        {
        public:
            grammar_factory(parser_data *_data);
            lib::pointer<grammar> create_grammar();
        
        protected:
            lib::pointer<pattern> create_pattern_variable();
            lib::pointer<pattern> create_pattern_function_call();
            lib::pointer<pattern> create_pattern_statement_expression();
            lib::pointer<pattern> create_pattern_binary(ast::token_2nd_list *_list);
            lib::pointer<pattern> create_pattern_function_body();
            lib::pointer<pattern> create_pattern_unary_prefix(ast::token_2nd_list *_list);

            parser_data *data;
        };
    };
};
