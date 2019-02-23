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

#include "grammar_factory.h"

namespace g0at
{
    namespace parser
    {
        grammar_factory::grammar_factory(parser_data *_data)
            : data(_data)
        {
        }

        lib::pointer<grammar> grammar_factory::create_grammar()
        {
            grammar *gr = new grammar();
            gr->vector.push_back(create_pattern_function_decl());
            gr->vector.push_back(create_pattern_variable());
            gr->vector.push_back(create_pattern_object());
            gr->vector.push_back(create_pattern_function_call());
            gr->vector.push_back(create_pattern_unary_prefix(&data->opers_plus_minus));
            gr->vector.push_back(create_pattern_binary(&data->opers_plus_minus));
            gr->vector.push_back(create_pattern_assignment());
            gr->vector.push_back(create_pattern_declare_variable());
            gr->vector.push_back(create_pattern_statement_return());
            gr->vector.push_back(create_pattern_statement_expression());
            return gr;
        }
    };
};
