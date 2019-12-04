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
            gr->vector.push_back(create_pattern_variable_in());
            gr->vector.push_back(create_pattern_method_lock());
            gr->vector.push_back(create_pattern_variable());
            gr->vector.push_back(create_pattern_statement_block());
            gr->vector.push_back(create_pattern_index_access());
            gr->vector.push_back(create_pattern_object());
            gr->vector.push_back(create_pattern_array());
            gr->vector.push_back(create_pattern_property());
            gr->vector.push_back(create_pattern_operator_new());
            gr->vector.push_back(create_pattern_method_call());
            gr->vector.push_back(create_pattern_function_call());
            gr->vector.push_back(create_pattern_parenthesized_expression());
            gr->vector.push_back(create_pattern_unary_suffix(&data->opers_incr_decr));
            gr->vector.push_back(create_pattern_unary_prefix(&data->opers_incr_decr));
            gr->vector.push_back(create_pattern_unary_prefix(&data->opers_plus_minus));
            gr->vector.push_back(create_pattern_unary_prefix(&data->opers_protect));
            gr->vector.push_back(create_pattern_unary_prefix(&data->opers_excl_tildes));
            gr->vector.push_back(create_pattern_binary(&data->opers_inherit));
            gr->vector.push_back(create_pattern_binary(&data->opers_exp));
            gr->vector.push_back(create_pattern_binary(&data->opers_mul_div_mod));
            gr->vector.push_back(create_pattern_binary(&data->opers_plus_minus));
            gr->vector.push_back(create_pattern_binary(&data->opers_shift));
            gr->vector.push_back(create_pattern_binary(&data->opers_less_greater));
            gr->vector.push_back(create_pattern_binary(&data->opers_equals_and_not));
            gr->vector.push_back(create_pattern_binary(&data->opers_ampersand));
            gr->vector.push_back(create_pattern_binary(&data->opers_caret));
            gr->vector.push_back(create_pattern_binary(&data->opers_vertical_bar));
            gr->vector.push_back(create_pattern_binary(&data->opers_dbl_ampersand));
            gr->vector.push_back(create_pattern_binary(&data->opers_dbl_vertical_bar));
            gr->vector.push_back(create_pattern_ternary());
            gr->vector.push_back(create_pattern_assignment());
            gr->vector.push_back(create_pattern_declare_variable());
            gr->vector.push_back(create_pattern_statement_debug());
            gr->vector.push_back(create_pattern_statement_return());
            gr->vector.push_back(create_pattern_statement_break());
            gr->vector.push_back(create_pattern_statement_continue());
            gr->vector.push_back(create_pattern_statement_throw());
            gr->vector.push_back(create_pattern_statement_expression());
            gr->vector.push_back(create_pattern_statement_empty());
            gr->vector.push_back(create_pattern_statement_switch());
            gr->vector.push_back(create_pattern_statement_if());
            gr->vector.push_back(create_pattern_statement_for());
            gr->vector.push_back(create_pattern_statement_do_while());
            gr->vector.push_back(create_pattern_statement_while());
            gr->vector.push_back(create_pattern_statement_try());
            gr->vector.push_back(create_pattern_statement_lock());
            return gr;
        }
    };
};
