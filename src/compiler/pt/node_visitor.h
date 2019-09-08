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

        class node_visitor
        {
        public:
            node_visitor();
            virtual ~node_visitor();
            virtual void visit(variable *ref);
            virtual void visit(static_string *ref);
            virtual void visit(function_call *ref);
            virtual void visit(statement_expression *ref);
            virtual void visit(function *ref);
            virtual void visit(addition *ref);
            virtual void visit(integer *ref);
            virtual void visit(subtraction *ref);
            virtual void visit(negation *ref);
            virtual void visit(value_void *ref);
            virtual void visit(value_undefined *ref);
            virtual void visit(value_null *ref);
            virtual void visit(declare_variable *ref);
            virtual void visit(assignment *ref);
            virtual void visit(real *ref);
            virtual void visit(declare_function *ref);
            virtual void visit(statement_return *ref);
            virtual void visit(node_object *ref);
            virtual void visit(property *ref);
            virtual void visit(value_true *ref);
            virtual void visit(value_false *ref);
            virtual void visit(is_equal_to *ref);
            virtual void visit(is_not_equal_to *ref);
            virtual void visit(statement_while *ref);
            virtual void visit(method_call *ref);
            virtual void visit(this_ptr *ref);
            virtual void visit(node_array *ref);
            virtual void visit(statement_block *ref);
            virtual void visit(statement_if *ref);
            virtual void visit(statement_throw *ref);
            virtual void visit(statement_try *ref);
            virtual void visit(inheritance *ref);
            virtual void visit(character *ref);
            virtual void visit(statement_for *ref);
            virtual void visit(is_less_than *ref);
            virtual void visit(statement_empty *ref);
            virtual void visit(operator_new *ref);
            virtual void visit(prefix_increment *ref);
            virtual void visit(statement_lock *ref);
            virtual void visit(index_access *ref);
            virtual void visit(statement_for_in *ref);
            virtual void visit(statement_do_while *ref);
            virtual void visit(statement_break *ref);
            virtual void visit(statement_continue *ref);
            virtual void visit(statement_switch *ref);
            virtual void visit(suffix_increment *ref);
            virtual void visit(prefix_decrement *ref);
            virtual void visit(suffix_decrement *ref);
            virtual void visit(multiplication *ref);
            virtual void visit(exponentiation *ref);
            virtual void visit(division *ref);
            virtual void visit(remainder *ref);
            virtual void visit(unary_plus *ref);
            virtual void visit(logical_not *ref);
            virtual void visit(bitwise_not *ref);
            virtual void visit(operator_bool *ref);
            virtual void visit(left_shift *ref);
            virtual void visit(signed_right_shift *ref);
            virtual void visit(zero_fill_right_shift *ref);
            virtual void visit(is_less_than_or_equal_to *ref);
            virtual void visit(is_greater_than *ref);
            virtual void visit(is_greater_than_or_equal_to *ref);
            virtual void visit(bitwise_and *ref);
            virtual void visit(bitwise_or *ref);
            virtual void visit(bitwise_xor *ref);
            virtual void visit(logical_and *ref);
            virtual void visit(logical_or *ref);
            virtual void visit(assignment_by_sum *ref);
            virtual void visit(assignment_by_difference *ref);
            virtual void visit(assignment_by_product *ref);
            virtual void visit(assignment_by_quotient *ref);
            virtual void visit(assignment_by_remainder *ref);
            virtual void visit(assignment_by_left_shift *ref);
            virtual void visit(assignment_by_signed_right_shift *ref);
            virtual void visit(assignment_by_zero_fill_right_shift *ref);
            virtual void visit(assignment_by_bitwise_and *ref);
            virtual void visit(assignment_by_bitwise_or *ref);
            virtual void visit(assignment_by_bitwise_xor *ref);
            virtual void visit(ternary *ref);
        };
    };
};
