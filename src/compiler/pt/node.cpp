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

#include "node.h"

namespace g0at
{
    namespace pt
    {
        node::node(lib::pointer<position> _pos)
            : pos(_pos)
        {
        }

        node::~node()
        {
        }

        expression *node::to_expression()
        {
            return nullptr;
        }

        statement *node::to_statement()
        {
            return nullptr;
        }

        variable *node::to_variable()
        {
            return nullptr;
        }

        static_string *node::to_static_string()
        {
            return nullptr;
        }

        function_call *node::to_function_call()
        {
            return nullptr;
        }

        statement_expression *node::to_statement_expression()
        {
            return nullptr;
        }

        function *node::to_function()
        {
            return nullptr;
        }

        binary *node::to_binary()
        {
            return nullptr;
        }

        addition *node::to_addition()
        {
            return nullptr;
        }

        integer *node::to_integer()
        {
            return nullptr;
        }

        subtraction *node::to_subtraction()
        {
            return nullptr;
        }

        unary_prefix *node::to_unary_prefix()
        {
            return nullptr;
        }

        negation *node::to_negation()
        {
            return nullptr;
        }

        value_void *node::to_value_void()
        {
            return nullptr;
        }

        value_undefined *node::to_value_undefined()
        {
            return nullptr;
        }

        value_null *node::to_value_null()
        {
            return nullptr;
        }

        declare_variable *node::to_declare_variable()
        {
            return nullptr;
        }

        assignment *node::to_assignment()
        {
            return nullptr;
        }

        real *node::to_real()
        {
            return nullptr;
        }

        declare_function *node::to_declare_function()
        {
            return nullptr;
        }

        statement_return *node::to_statement_return()
        {
            return nullptr;
        }

        node_object *node::to_node_object()
        {
            return nullptr;
        }

        property *node::to_property()
        {
            return nullptr;
        }

        value_true *node::to_value_true()
        {
            return nullptr;
        }

        value_false *node::to_value_false()
        {
            return nullptr;
        }

        is_equal_to *node::to_is_equal_to()
        {
            return nullptr;
        }
        
        is_not_equal_to *node::to_is_not_equal_to()
        {
            return nullptr;
        }
        
        statement_while *node::to_statement_while()
        {
            return nullptr;
        }
        
        method_call *node::to_method_call()
        {
            return nullptr;
        }
        
        this_ptr *node::to_this_ptr()
        {
            return nullptr;
        }
        
        node_array *node::to_node_array()
        {
            return nullptr;
        }
        
        statement_block *node::to_statement_block()
        {
            return nullptr;
        }
        
        statement_if *node::to_statement_if()
        {
            return nullptr;
        }
        
        statement_throw *node::to_statement_throw()
        {
            return nullptr;
        }
        
        statement_try *node::to_statement_try()
        {
            return nullptr;
        }
        
        inheritance *node::to_inheritance()
        {
            return nullptr;
        }
        
        character *node::to_character()
        {
            return nullptr;
        }

        statement_for *node::to_statement_for()
        {
            return nullptr;
        }

        is_less_than *node::to_is_less_than()
        {
            return nullptr;
        }

        statement_empty *node::to_statement_empty()
        {
            return nullptr;
        }

        operator_new *node::to_operator_new()
        {
            return nullptr;
        }

        prefix_increment *node::to_prefix_increment()
        {
            return nullptr;
        }

        statement_lock *node::to_statement_lock()
        {
            return nullptr;
        }

        index_access *node::to_index_access()
        {
            return nullptr;
        }

        statement_for_in *node::to_statement_for_in()
        {
            return nullptr;
        }

        statement_do_while *node::to_statement_do_while()
        {
            return nullptr;
        }

        statement_break *node::to_statement_break()
        {
            return nullptr;
        }

        statement_continue *node::to_statement_continue()
        {
            return nullptr;
        }

        statement_switch *node::to_statement_switch()
        {
            return nullptr;
        }

        unary_suffix *node::to_unary_suffix()
        {
            return nullptr;
        }

        suffix_increment *node::to_suffix_increment()
        {
            return nullptr;
        }

        prefix_decrement *node::to_prefix_decrement()
        {
            return nullptr;
        }

        suffix_decrement *node::to_suffix_decrement()
        {
            return nullptr;
        }

        multiplication *node::to_multiplication()
        {
            return nullptr;
        }

        exponentiation *node::to_exponentiation()
        {
            return nullptr;
        }

        division *node::to_division()
        {
            return nullptr;
        }

        remainder *node::to_remainder()
        {
            return nullptr;
        }

        unary_plus *node::to_unary_plus()
        {
            return nullptr;
        }

        logical_not *node::to_logical_not()
        {
            return nullptr;
        }

        bitwise_not *node::to_bitwise_not()
        {
            return nullptr;
        }

        operator_bool *node::to_operator_bool()
        {
            return nullptr;
        }

        left_shift *node::to_left_shift()
        {
            return nullptr;
        }

        signed_right_shift *node::to_signed_right_shift()
        {
            return nullptr;
        }

        zero_fill_right_shift *node::to_zero_fill_right_shift()
        {
            return nullptr;
        }

        is_less_than_or_equal_to *node::to_is_less_than_or_equal_to()
        {
            return nullptr;
        }

        is_greater_than *node::to_is_greater_than()
        {
            return nullptr;
        }

        is_greater_than_or_equal_to *node::to_is_greater_than_or_equal_to()
        {
            return nullptr;
        }

        bitwise_and *node::to_bitwise_and()
        {
            return nullptr;
        }

        bitwise_or *node::to_bitwise_or()
        {
            return nullptr;
        }

        bitwise_xor *node::to_bitwise_xor()
        {
            return nullptr;
        }

        logical_and *node::to_logical_and()
        {
            return nullptr;
        }

        logical_or *node::to_logical_or()
        {
            return nullptr;
        }

        assignment_by_sum *node::to_assignment_by_sum()
        {
            return nullptr;
        }

        assignment_by_difference *node::to_assignment_by_difference()
        {
            return nullptr;
        }

        assignment_by_product *node::to_assignment_by_product()
        {
            return nullptr;
        }

        assignment_by_quotient *node::to_assignment_by_quotient()
        {
            return nullptr;
        }

        assignment_by_remainder *node::to_assignment_by_remainder()
        {
            return nullptr;
        }

        assignment_by_left_shift *node::to_assignment_by_left_shift()
        {
            return nullptr;
        }

        assignment_by_signed_right_shift *node::to_assignment_by_signed_right_shift()
        {
            return nullptr;
        }

        assignment_by_zero_fill_right_shift *node::to_assignment_by_zero_fill_right_shift()
        {
            return nullptr;
        }

        assignment_by_bitwise_and *node::to_assignment_by_bitwise_and()
        {
            return nullptr;
        }

        assignment_by_bitwise_or *node::to_assignment_by_bitwise_or()
        {
            return nullptr;
        }

        assignment_by_bitwise_xor *node::to_assignment_by_bitwise_xor()
        {
            return nullptr;
        }
    }
};
