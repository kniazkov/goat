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

#pragma once

namespace g0at
{
    namespace pt
    {
        class variable;
        class static_string;
        class function_call;
        class statement_expression;
        class function;
        class addition;
        class integer;
        class subtraction;
        class negation;
        class value_void;
        class value_undefined;
        class value_null;
        class declare_variable;
        class assignment;
        class real;
        class declare_function;
        class statement_return;
        class node_object;
        class property;
        class value_true;
        class value_false;
        class is_equal_to;
        class is_not_equal_to;
        class statement_while;
        class method_call;
        class this_ptr;
        class node_array;
        class statement_block;
        class statement_if;
        class statement_throw;
        class statement_try;
        class inheritance;
        class character;
        class statement_for;
        class is_less_than;
        class statement_empty;
        class operator_new;
        class prefix_increment;
        class statement_lock;
        class index_access;
        class statement_for_in;
        class statement_do_while;
        class statement_break;
        class statement_continue;
        class statement_switch;
        class suffix_increment;
        class prefix_decrement;
        class suffix_decrement;
        class multiplication;
        class exponentiation;
        class division;
        class remainder;
        class unary_plus;
        class logical_not;
        class bitwise_not;
        class operator_bool;
        class left_shift;
        class signed_right_shift;
        class zero_fill_right_shift;
        class is_less_than_or_equal_to;
        class is_greater_than;
        class is_greater_than_or_equal_to;
        class bitwise_and;
        class bitwise_or;
        class bitwise_xor;
        class logical_and;
        class logical_or;
        class assignment_by_sum;
        class assignment_by_difference;
        class assignment_by_product;
        class assignment_by_quotient;
        class assignment_by_remainder;
        class assignment_by_left_shift;
        class assignment_by_signed_right_shift;
        class assignment_by_zero_fill_right_shift;
        class assignment_by_bitwise_and;
        class assignment_by_bitwise_or;
        class assignment_by_bitwise_xor;
        class ternary;
        class statement_debug;
        class protection;

        class node_visitor
        {
        public:
            node_visitor();
            virtual ~node_visitor();
            virtual void visit(variable *ref) = 0;
            virtual void visit(static_string *ref) = 0;
            virtual void visit(function_call *ref) = 0;
            virtual void visit(statement_expression *ref) = 0;
            virtual void visit(function *ref) = 0;
            virtual void visit(addition *ref) = 0;
            virtual void visit(integer *ref) = 0;
            virtual void visit(subtraction *ref) = 0;
            virtual void visit(negation *ref) = 0;
            virtual void visit(value_void *ref) = 0;
            virtual void visit(value_undefined *ref) = 0;
            virtual void visit(value_null *ref) = 0;
            virtual void visit(declare_variable *ref) = 0;
            virtual void visit(assignment *ref) = 0;
            virtual void visit(real *ref) = 0;
            virtual void visit(declare_function *ref) = 0;
            virtual void visit(statement_return *ref) = 0;
            virtual void visit(node_object *ref) = 0;
            virtual void visit(property *ref) = 0;
            virtual void visit(value_true *ref) = 0;
            virtual void visit(value_false *ref) = 0;
            virtual void visit(is_equal_to *ref) = 0;
            virtual void visit(is_not_equal_to *ref) = 0;
            virtual void visit(statement_while *ref) = 0;
            virtual void visit(method_call *ref) = 0;
            virtual void visit(this_ptr *ref) = 0;
            virtual void visit(node_array *ref) = 0;
            virtual void visit(statement_block *ref) = 0;
            virtual void visit(statement_if *ref) = 0;
            virtual void visit(statement_throw *ref) = 0;
            virtual void visit(statement_try *ref) = 0;
            virtual void visit(inheritance *ref) = 0;
            virtual void visit(character *ref) = 0;
            virtual void visit(statement_for *ref) = 0;
            virtual void visit(is_less_than *ref) = 0;
            virtual void visit(statement_empty *ref) = 0;
            virtual void visit(operator_new *ref) = 0;
            virtual void visit(prefix_increment *ref) = 0;
            virtual void visit(statement_lock *ref) = 0;
            virtual void visit(index_access *ref) = 0;
            virtual void visit(statement_for_in *ref) = 0;
            virtual void visit(statement_do_while *ref) = 0;
            virtual void visit(statement_break *ref) = 0;
            virtual void visit(statement_continue *ref) = 0;
            virtual void visit(statement_switch *ref) = 0;
            virtual void visit(suffix_increment *ref) = 0;
            virtual void visit(prefix_decrement *ref) = 0;
            virtual void visit(suffix_decrement *ref) = 0;
            virtual void visit(multiplication *ref) = 0;
            virtual void visit(exponentiation *ref) = 0;
            virtual void visit(division *ref) = 0;
            virtual void visit(remainder *ref) = 0;
            virtual void visit(unary_plus *ref) = 0;
            virtual void visit(logical_not *ref) = 0;
            virtual void visit(bitwise_not *ref) = 0;
            virtual void visit(operator_bool *ref) = 0;
            virtual void visit(left_shift *ref) = 0;
            virtual void visit(signed_right_shift *ref) = 0;
            virtual void visit(zero_fill_right_shift *ref) = 0;
            virtual void visit(is_less_than_or_equal_to *ref) = 0;
            virtual void visit(is_greater_than *ref) = 0;
            virtual void visit(is_greater_than_or_equal_to *ref) = 0;
            virtual void visit(bitwise_and *ref) = 0;
            virtual void visit(bitwise_or *ref) = 0;
            virtual void visit(bitwise_xor *ref) = 0;
            virtual void visit(logical_and *ref) = 0;
            virtual void visit(logical_or *ref) = 0;
            virtual void visit(assignment_by_sum *ref) = 0;
            virtual void visit(assignment_by_difference *ref) = 0;
            virtual void visit(assignment_by_product *ref) = 0;
            virtual void visit(assignment_by_quotient *ref) = 0;
            virtual void visit(assignment_by_remainder *ref) = 0;
            virtual void visit(assignment_by_left_shift *ref) = 0;
            virtual void visit(assignment_by_signed_right_shift *ref) = 0;
            virtual void visit(assignment_by_zero_fill_right_shift *ref) = 0;
            virtual void visit(assignment_by_bitwise_and *ref) = 0;
            virtual void visit(assignment_by_bitwise_or *ref) = 0;
            virtual void visit(assignment_by_bitwise_xor *ref) = 0;
            virtual void visit(ternary *ref) = 0;
            virtual void visit(statement_debug *ref) = 0;
            virtual void visit(protection *ref) = 0;
        };
    };
};
