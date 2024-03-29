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

#include "node_visitor.h"
#include "scope.h"
#include "compiler/source/fragment.h"
#include "lib/ref_counter.h"
#include "lib/pointer.h"

namespace g0at
{
    namespace pt
    {
        class expression;
        class statement;
        class variable;
        class static_string;
        class function_call;
        class statement_expression;
        class function;
        class binary;
        class addition;
        class integer;
        class subtraction;
        class unary_prefix;
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
        class unary_suffix;
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

        class node : public lib::ref_counter
        {
        public:
            node(fragment _frag);
            virtual ~node();
            virtual void accept(node_visitor *visitor) = 0;
            virtual expression *to_expression();
            virtual statement *to_statement();
            virtual variable *to_variable();
            virtual static_string *to_static_string();
            virtual function_call *to_function_call();
            virtual statement_expression *to_statement_expression();
            virtual function *to_function();
            virtual binary *to_binary();
            virtual addition *to_addition();
            virtual integer *to_integer();
            virtual subtraction *to_subtraction();
            virtual unary_prefix *to_unary_prefix();
            virtual negation *to_negation();
            virtual value_void *to_value_void();
            virtual value_undefined *to_value_undefined();
            virtual value_null *to_value_null();
            virtual declare_variable *to_declare_variable();
            virtual assignment *to_assignment();
            virtual real *to_real();
            virtual declare_function *to_declare_function();
            virtual statement_return *to_statement_return();
            virtual node_object *to_node_object();
            virtual property *to_property();
            virtual value_true *to_value_true();
            virtual value_false *to_value_false();
            virtual is_equal_to *to_is_equal_to();
            virtual is_not_equal_to *to_is_not_equal_to();
            virtual statement_while *to_statement_while();
            virtual method_call *to_method_call();
            virtual this_ptr *to_this_ptr();
            virtual node_array *to_node_array();
            virtual statement_block *to_statement_block();
            virtual statement_if *to_statement_if();
            virtual statement_throw *to_statement_throw();
            virtual statement_try *to_statement_try();
            virtual inheritance *to_inheritance();
            virtual character *to_character();
            virtual statement_for *to_statement_for();
            virtual is_less_than *to_is_less_than();
            virtual statement_empty *to_statement_empty();
            virtual operator_new *to_operator_new();
            virtual prefix_increment *to_prefix_increment();
            virtual statement_lock *to_statement_lock();
            virtual index_access *to_index_access();
            virtual statement_for_in *to_statement_for_in();
            virtual statement_do_while *to_statement_do_while();
            virtual statement_break *to_statement_break();
            virtual statement_continue *to_statement_continue();
            virtual statement_switch *to_statement_switch();
            virtual unary_suffix *to_unary_suffix();
            virtual suffix_increment *to_suffix_increment();
            virtual prefix_decrement *to_prefix_decrement();
            virtual suffix_decrement *to_suffix_decrement();
            virtual multiplication *to_multiplication();
            virtual exponentiation *to_exponentiation();
            virtual division *to_division();
            virtual remainder *to_remainder();
            virtual unary_plus *to_unary_plus();
            virtual logical_not *to_logical_not();
            virtual bitwise_not *to_bitwise_not();
            virtual operator_bool *to_operator_bool();
            virtual left_shift *to_left_shift();
            virtual signed_right_shift *to_signed_right_shift();
            virtual zero_fill_right_shift *to_zero_fill_right_shift();
            virtual is_less_than_or_equal_to *to_is_less_than_or_equal_to();
            virtual is_greater_than *to_is_greater_than();
            virtual is_greater_than_or_equal_to *to_is_greater_than_or_equal_to();
            virtual bitwise_and *to_bitwise_and();
            virtual bitwise_or *to_bitwise_or();
            virtual bitwise_xor *to_bitwise_xor();
            virtual logical_and *to_logical_and();
            virtual logical_or *to_logical_or();
            virtual assignment_by_sum *to_assignment_by_sum();
            virtual assignment_by_difference *to_assignment_by_difference();
            virtual assignment_by_product *to_assignment_by_product();
            virtual assignment_by_quotient *to_assignment_by_quotient();
            virtual assignment_by_remainder *to_assignment_by_remainder();
            virtual assignment_by_left_shift *to_assignment_by_left_shift();
            virtual assignment_by_signed_right_shift *to_assignment_by_signed_right_shift();
            virtual assignment_by_zero_fill_right_shift *to_assignment_by_zero_fill_right_shift();
            virtual assignment_by_bitwise_and *to_assignment_by_bitwise_and();
            virtual assignment_by_bitwise_or *to_assignment_by_bitwise_or();
            virtual assignment_by_bitwise_xor *to_assignment_by_bitwise_xor();
            virtual ternary *to_ternary();
            virtual statement_debug *to_statement_debug();
            virtual protection *to_protection();
            
            fragment get_fragment() { return frag; }

        private:
            fragment frag;
        };
    };
};
