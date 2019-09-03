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
#pragma warning( disable : 4250 )

#include "token_visitor.h"
#include "token_list.h"
#include "token_2nd_list.h"
#include "compiler/source/position.h"
#include "lib/ref_counter.h"
#include "lib/pointer.h"

namespace g0at
{
    class scanner;

    namespace ast
    {
        class nonterminal;
        class expression;
        class left_expression;
        class statement;
        class token_with_list;
        class function;
        class root;
        class identifier;
        class variable;
        class bracket;
        class static_string;
        class semicolon;
        class brackets_pair;
        class function_call;
        class statement_expression;
        class token_operator;
        class plus;
        class custom_operator;
        class binary;
        class addition;
        class integer;
        class minus;
        class subtraction;
        class unary_prefix;
        class negation;
        class value_void;
        class value_undefined;
        class value_null;
        class keyword;
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
        class unary_suffix;
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

        class token : public lib::ref_counter
        {
        friend class g0at::scanner;
        public:
            token();
            virtual ~token();
            virtual void accept(token_visitor *visitor) = 0;
            void replace(lib::pointer<token> repl);
            void replace(token *end, lib::pointer<token> repl);
            void remove();
            void remove_2nd();
            virtual nonterminal *to_nonterminal();
            virtual expression *to_expression();
            virtual left_expression *to_left_expression();
            virtual statement *to_statement();
            virtual token_with_list *to_token_with_list();
            virtual function *to_function();
            virtual root *to_root();
            virtual identifier *to_identifier();
            virtual variable *to_variable();
            virtual bracket *to_bracket();
            virtual static_string *to_static_string();
            virtual semicolon *to_semicolon();
            virtual brackets_pair *to_brackets_pair();
            virtual function_call *to_function_call();
            virtual statement_expression *to_statement_expression();
            virtual token_operator *to_token_operator();
            virtual plus *to_plus();
            virtual custom_operator *to_custom_operator();
            virtual binary *to_binary();
            virtual addition *to_addition();
            virtual integer *to_integer();
            virtual minus *to_minus();
            virtual subtraction *to_subtraction();
            virtual unary_prefix *to_unary_prefix();
            virtual negation *to_negation();
            virtual value_void *to_value_void();
            virtual value_undefined *to_value_undefined();
            virtual value_null *to_value_null();
            virtual keyword *to_keyword();
            virtual keyword_var *to_keyword_var();
            virtual declare_variable *to_declare_variable();
            virtual assign *to_assign();
            virtual assignment *to_assignment();
            virtual comma *to_comma();
            virtual real *to_real();
            virtual keyword_function *to_keyword_function();
            virtual declare_function *to_declare_function();
            virtual keyword_return *to_keyword_return();
            virtual statement_return *to_statement_return();
            virtual token_object *to_token_object();
            virtual colon *to_colon();
            virtual dot *to_dot();
            virtual property *to_property();
            virtual value_true *to_value_true();
            virtual value_false *to_value_false();
            virtual equals *to_equals();
            virtual not_equal *to_not_equal();
            virtual is_equal_to *to_is_equal_to();
            virtual is_not_equal_to *to_is_not_equal_to();
            virtual keyword_while *to_keyword_while();
            virtual statement_while *to_statement_while();
            virtual method_call *to_method_call();
            virtual this_ptr *to_this_ptr();
            virtual token_array *to_token_array();
            virtual statement_block *to_statement_block();
            virtual keyword_if *to_keyword_if();
            virtual keyword_else *to_keyword_else();
            virtual statement_if *to_statement_if();
            virtual keyword_throw *to_keyword_throw();
            virtual statement_throw *to_statement_throw();
            virtual keyword_try *to_keyword_try();
            virtual keyword_catch *to_keyword_catch();
            virtual keyword_finally *to_keyword_finally();
            virtual statement_try *to_statement_try();
            virtual inherit *to_inherit();
            virtual inheritance *to_inheritance();
            virtual character *to_character();
            virtual keyword_for *to_keyword_for();
            virtual statement_for *to_statement_for();
            virtual less *to_less();
            virtual is_less_than *to_is_less_than();
            virtual statement_empty *to_statement_empty();
            virtual keyword_new *to_keyword_new();
            virtual operator_new *to_operator_new();
            virtual increment *to_increment();
            virtual prefix_increment *to_prefix_increment();
            virtual keyword_thread *to_keyword_thread();
            virtual keyword_lock *to_keyword_lock();
            virtual statement_lock *to_statement_lock();
            virtual index_access *to_index_access();
            virtual keyword_in *to_keyword_in();
            virtual variable_in *to_variable_in();
            virtual statement_for_in *to_statement_for_in();
            virtual keyword_do *to_keyword_do();
            virtual statement_do_while *to_statement_do_while();
            virtual keyword_break *to_keyword_break();
            virtual keyword_continue *to_keyword_continue();
            virtual statement_break *to_statement_break();
            virtual statement_continue *to_statement_continue();
            virtual keyword_switch *to_keyword_switch();
            virtual keyword_case *to_keyword_case();
            virtual keyword_default *to_keyword_default();
            virtual statement_switch *to_statement_switch();
            virtual unary_suffix *to_unary_suffix();
            virtual suffix_increment *to_suffix_increment();
            virtual decrement *to_decrement();
            virtual prefix_decrement *to_prefix_decrement();
            virtual suffix_decrement *to_suffix_decrement();
            virtual keyword_import *to_keyword_import();
            virtual asterisk *to_asterisk();
            virtual double_asterisk *to_double_asterisk();
            virtual slash *to_slash();
            virtual percent *to_percent();
            virtual multiplication *to_multiplication();
            virtual exponentiation *to_exponentiation();
            virtual division *to_division();
            virtual remainder *to_remainder();
            virtual unary_plus *to_unary_plus();
            virtual exclamation *to_exclamation();
            virtual tilde *to_tilde();
            virtual logical_not *to_logical_not();
            virtual bitwise_not *to_bitwise_not();
            virtual double_exclamation *to_double_exclamation();
            virtual operator_bool *to_operator_bool();
            virtual left_shift *to_left_shift();
            virtual signed_right_shift *to_signed_right_shift();
            virtual zero_fill_right_shift *to_zero_fill_right_shift();
            virtual double_less *to_double_less();
            virtual double_greater *to_double_greater();
            virtual triple_greater *to_triple_greater();

            lib::pointer<position> get_position() { return pos; }

            token_list *list;
            token *prev;
            lib::pointer<token> next;

            token_2nd_list *list_2;
            token *prev_2;
            token *next_2;
        
        protected:
            lib::pointer<position> pos;
        };
    };
};
