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
    namespace ast
    {
        class function;
        class identifier;
        class variable;
        class bracket;
        class static_string;
        class semicolon;
        class brackets_pair;
        class function_call;
        class statement_expression;
        class plus;
        class custom_operator;
        class addition;
        class integer;
        class minus;
        class subtraction;
        class negation;
        class value_void;
        class value_undefined;
        class value_null;
        class keyword_var;
        class declare_variable;
        class assign;
        class assignment;
        class comma;
        class real;
        class keyword_function;
        class declare_function;
        class keyword_return;
        class statement_return;
        class token_object;
        class colon;
        class dot;
        class property;
        class value_true;
        class value_false;
        class equals;
        class not_equal;
        class is_equal_to;
        class is_not_equal_to;
        class keyword_while;
        class statement_while;
        class method_call;
        class this_ptr;
        class token_array;
        class statement_block;
        class keyword_if;
        class keyword_else;
        class statement_if;
        class keyword_throw;
        class statement_throw;
        class keyword_try;
        class keyword_catch;
        class keyword_finally;
        class statement_try;
        class inherit;
        class inheritance;
        class character;
        class keyword_for;
        class statement_for;
        class less;
        class is_less_than;
        class statement_empty;
        class keyword_new;
        class operator_new;
        class increment;
        class prefix_increment;
        class keyword_thread;
        class keyword_lock;
        class statement_lock;
        class index_access;
        class keyword_in;
        class variable_in;
        class statement_for_in;
        class keyword_do;
        class statement_do_while;
        class keyword_break;
        class keyword_continue;
        class statement_break;
        class statement_continue;
        class keyword_switch;
        class keyword_case;
        class keyword_default;
        class statement_switch;
        class suffix_increment;
        class decrement;
        class prefix_decrement;
        class suffix_decrement;
        class keyword_import;
        class asterisk;
        class double_asterisk;
        class slash;
        class percent;
        class multiplication;
        class exponentiation;
        class division;
        class remainder;
        class unary_plus;
        class exclamation;
        class tilde;
        class logical_not;
        class bitwise_not;
        class double_exclamation;
        class operator_bool;
        class left_shift;
        class signed_right_shift;
        class zero_fill_right_shift;
        class double_less;
        class double_greater;
        class triple_greater;
        class is_less_than_or_equal_to;
        class is_greater_than;
        class is_greater_than_or_equal_to;
        class less_or_equal;
        class greater;
        class greater_or_equal;
        class bitwise_and;
        class bitwise_or;
        class bitwise_xor;
        class logical_and;
        class logical_or;
 
        class token_visitor
        {
        public:
            token_visitor();
            virtual ~token_visitor();
            virtual void visit(function *ref);
            virtual void visit(identifier *ref);
            virtual void visit(variable *ref);
            virtual void visit(bracket *ref);
            virtual void visit(static_string *ref);
            virtual void visit(semicolon *ref);
            virtual void visit(brackets_pair *ref);
            virtual void visit(function_call *ref);
            virtual void visit(statement_expression *ref);
            virtual void visit(plus *ref);
            virtual void visit(custom_operator *ref);
            virtual void visit(addition *ref);
            virtual void visit(integer *ref);
            virtual void visit(minus *ref);
            virtual void visit(subtraction *ref);
            virtual void visit(negation *ref);
            virtual void visit(value_void *ref);
            virtual void visit(value_undefined *ref);
            virtual void visit(value_null *ref);
            virtual void visit(keyword_var *ref);
            virtual void visit(declare_variable *ref);
            virtual void visit(assign *ref);
            virtual void visit(assignment *ref);
            virtual void visit(comma *ref);
            virtual void visit(real *ref);
            virtual void visit(keyword_function *ref);
            virtual void visit(declare_function *ref);
            virtual void visit(keyword_return *ref);
            virtual void visit(statement_return *ref);
            virtual void visit(token_object *ref);
            virtual void visit(colon *ref);
            virtual void visit(dot *ref);
            virtual void visit(property *ref);
            virtual void visit(value_true *ref);
            virtual void visit(value_false *ref);
            virtual void visit(equals *ref);
            virtual void visit(not_equal *ref);
            virtual void visit(is_equal_to *ref);
            virtual void visit(is_not_equal_to *ref);
            virtual void visit(keyword_while *ref);
            virtual void visit(statement_while *ref);
            virtual void visit(method_call *ref);
            virtual void visit(this_ptr *ref);
            virtual void visit(token_array *ref);
            virtual void visit(statement_block *ref);
            virtual void visit(keyword_if *ref);
            virtual void visit(keyword_else *ref);
            virtual void visit(statement_if *ref);
            virtual void visit(keyword_throw *ref);
            virtual void visit(statement_throw *ref);
            virtual void visit(keyword_try *ref);
            virtual void visit(keyword_catch *ref);
            virtual void visit(keyword_finally *ref);
            virtual void visit(statement_try *ref);
            virtual void visit(inherit *ref);
            virtual void visit(inheritance *ref);
            virtual void visit(character *ref);
            virtual void visit(keyword_for *ref);
            virtual void visit(statement_for *ref);
            virtual void visit(less *ref);
            virtual void visit(is_less_than *ref);
            virtual void visit(statement_empty *ref);
            virtual void visit(keyword_new *ref);
            virtual void visit(operator_new *ref);
            virtual void visit(increment *ref);
            virtual void visit(prefix_increment *ref);
            virtual void visit(keyword_thread *ref);
            virtual void visit(keyword_lock *ref);
            virtual void visit(statement_lock *ref);
            virtual void visit(index_access *ref);
            virtual void visit(keyword_in *ref);
            virtual void visit(variable_in *ref);
            virtual void visit(statement_for_in *ref);
            virtual void visit(keyword_do *ref);
            virtual void visit(statement_do_while *ref);
            virtual void visit(keyword_break *ref);
            virtual void visit(keyword_continue *ref);
            virtual void visit(statement_break *ref);
            virtual void visit(statement_continue *ref);
            virtual void visit(keyword_switch *ref);
            virtual void visit(keyword_case *ref);
            virtual void visit(keyword_default *ref);
            virtual void visit(statement_switch *ref);
            virtual void visit(suffix_increment *ref);
            virtual void visit(decrement *ref);
            virtual void visit(prefix_decrement *ref);
            virtual void visit(suffix_decrement *ref);
            virtual void visit(keyword_import *ref);
            virtual void visit(asterisk *ref);
            virtual void visit(double_asterisk *ref);
            virtual void visit(slash *ref);
            virtual void visit(percent *ref);
            virtual void visit(multiplication *ref);
            virtual void visit(exponentiation *ref);
            virtual void visit(division *ref);
            virtual void visit(remainder *ref);
            virtual void visit(unary_plus *ref);
            virtual void visit(exclamation *ref);
            virtual void visit(tilde *ref);
            virtual void visit(logical_not *ref);
            virtual void visit(bitwise_not *ref);
            virtual void visit(double_exclamation *ref);
            virtual void visit(operator_bool *ref);
            virtual void visit(left_shift *ref);
            virtual void visit(signed_right_shift *ref);
            virtual void visit(zero_fill_right_shift *ref);
            virtual void visit(double_less *ref);
            virtual void visit(double_greater *ref);
            virtual void visit(triple_greater *ref);
            virtual void visit(is_less_than_or_equal_to *ref);
            virtual void visit(is_greater_than *ref);
            virtual void visit(is_greater_than_or_equal_to *ref);
            virtual void visit(less_or_equal *ref);
            virtual void visit(greater *ref);
            virtual void visit(greater_or_equal *ref);
            virtual void visit(bitwise_and *ref);
            virtual void visit(bitwise_or *ref);
            virtual void visit(bitwise_xor *ref);
            virtual void visit(logical_and *ref);
            virtual void visit(logical_or *ref);
         };
    };
};
