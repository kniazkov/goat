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

#include "token.h"
#include "lib/assert.h"

namespace g0at
{
    namespace ast
    {
        token::token()
            : list(nullptr), prev(nullptr), next(nullptr), list_2(nullptr), prev_2(nullptr), next_2(nullptr)
        {
        }

        token::~token()
        {
        }

        void token::replace(lib::pointer<token> repl)
        {
            assert(list != nullptr);
            list->replace(this, this, repl);
        }

        void token::replace(token *end, lib::pointer<token> repl)
        {
            assert(list != nullptr);
            list->replace(this, end, repl);
        }

        void token::remove()
        {
            assert(list != nullptr);
            list->remove(this);
        }

        void token::remove_2nd()
        {
            if(list_2 != nullptr)
            {
                list_2->remove(this);
            }
        }

        nonterminal *token::to_nonterminal()
        {
            return nullptr;
        }

        expression *token::to_expression()
        {
            return nullptr;
        }

        left_expression *token::to_left_expression()
        {
            return nullptr;
        }

        statement *token::to_statement()
        {
            return nullptr;
        }

        token_with_list *token::to_token_with_list()
        {
            return nullptr;
        }

        function *token::to_function()
        {
            return nullptr;
        }

        root *token::to_root()
        {
            return nullptr;
        }

        identifier *token::to_identifier()
        {
            return nullptr;
        }

        variable *token::to_variable()
        {
            return nullptr;
        }

        bracket *token::to_bracket()
        {
            return nullptr;
        }

        static_string *token::to_static_string()
        {
            return nullptr;
        }

        semicolon *token::to_semicolon()
        {
            return nullptr;
        }

        brackets_pair *token::to_brackets_pair()
        {
            return nullptr;
        }

        function_call *token::to_function_call()
        {
            return nullptr;
        }

        statement_expression *token::to_statement_expression()
        {
            return nullptr;
        }

        token_operator *token::to_token_operator()
        {
            return nullptr;
        }

        plus *token::to_plus()
        {
            return nullptr;
        }

        custom_operator *token::to_custom_operator()
        {
            return nullptr;
        }

        binary *token::to_binary()
        {
            return nullptr;
        }

        addition *token::to_addition()
        {
            return nullptr;
        }

        integer *token::to_integer()
        {
            return nullptr;
        }

        minus *token::to_minus()
        {
            return nullptr;
        }

        subtraction *token::to_subtraction()
        {
            return nullptr;
        }

        unary_prefix *token::to_unary_prefix()
        {
            return nullptr;
        }

        negation *token::to_negation()
        {
            return nullptr;
        }

        value_void *token::to_value_void()
        {
            return nullptr;
        }

        value_undefined *token::to_value_undefined()
        {
            return nullptr;
        }

        value_null *token::to_value_null()
        {
            return nullptr;
        }

        keyword *token::to_keyword()
        {
            return nullptr;
        }

        keyword_var *token::to_keyword_var()
        {
            return nullptr;
        }

        declare_variable *token::to_declare_variable()
        {
            return nullptr;
        }

        assign *token::to_assign()
        {
            return nullptr;
        }

        assignment *token::to_assignment()
        {
            return nullptr;
        }

        comma *token::to_comma()
        {
            return nullptr;
        }

        real *token::to_real()
        {
            return nullptr;
        }

        keyword_function *token::to_keyword_function()
        {
            return nullptr;
        }

        declare_function *token::to_declare_function()
        {
            return nullptr;
        }

        keyword_return *token::to_keyword_return()
        {
            return nullptr;
        }

        statement_return *token::to_statement_return()
        {
            return nullptr;
        }

        token_object *token::to_token_object()
        {
            return nullptr;
        }

        colon *token::to_colon()
        {
            return nullptr;
        }

        dot *token::to_dot()
        {
            return nullptr;
        }

        property *token::to_property()
        {
            return nullptr;
        }

        value_true *token::to_value_true()
        {
            return nullptr;
        }

        value_false *token::to_value_false()
        {
            return nullptr;
        }

        equals *token::to_equals()
        {
            return nullptr;
        }

        not_equal *token::to_not_equal()
        {
            return nullptr;
        }

        is_equal_to *token::to_is_equal_to()
        {
            return nullptr;
        }

        is_not_equal_to *token::to_is_not_equal_to()
        {
            return nullptr;
        }

        keyword_while *token::to_keyword_while()
        {
            return nullptr;
        }

        statement_while *token::to_statement_while()
        {
            return nullptr;
        }

        method_call *token::to_method_call()
        {
            return nullptr;
        }

        this_ptr *token::to_this_ptr()
        {
            return nullptr;
        }

        token_array *token::to_token_array()
        {
            return nullptr;
        }

        statement_block *token::to_statement_block()
        {
            return nullptr;
        }

        keyword_if *token::to_keyword_if()
        {
            return nullptr;
        }

        keyword_else *token::to_keyword_else()
        {
            return nullptr;
        }

        statement_if *token::to_statement_if()
        {
            return nullptr;
        }

        keyword_throw *token::to_keyword_throw()
        {
            return nullptr;
        }

        statement_throw *token::to_statement_throw()
        {
            return nullptr;
        }

        keyword_try *token::to_keyword_try()
        {
            return nullptr;
        }

        keyword_catch *token::to_keyword_catch()
        {
            return nullptr;
        }

        keyword_finally *token::to_keyword_finally()
        {
            return nullptr;
        }

        statement_try *token::to_statement_try()
        {
            return nullptr;
        }

        inherit *token::to_inherit()
        {
            return nullptr;
        }

        inheritance *token::to_inheritance()
        {
            return nullptr;
        }

        character *token::to_character()
        {
            return nullptr;
        }

        keyword_for *token::to_keyword_for()
        {
            return nullptr;
        }

        statement_for *token::to_statement_for()
        {
            return nullptr;
        }

        less *token::to_less()
        {
            return nullptr;
        }

        is_less_than *token::to_is_less_than()
        {
            return nullptr;
        }

        statement_empty *token::to_statement_empty()
        {
            return nullptr;
        }

        keyword_new *token::to_keyword_new()
        {
            return nullptr;
        }

        operator_new *token::to_operator_new()
        {
            return nullptr;
        }

        increment *token::to_increment()
        {
            return nullptr;
        }

        prefix_increment *token::to_prefix_increment()
        {
            return nullptr;
        }

        keyword_thread *token::to_keyword_thread()
        {
            return nullptr;
        }

        keyword_lock *token::to_keyword_lock()
        {
            return nullptr;
        }

        statement_lock *token::to_statement_lock()
        {
            return nullptr;
        }

        index_access *token::to_index_access()
        {
            return nullptr;
        }

        keyword_in *token::to_keyword_in()
        {
            return nullptr;
        }

        variable_in *token::to_variable_in()
        {
            return nullptr;
        }

        statement_for_in *token::to_statement_for_in()
        {
            return nullptr;
        }

        keyword_do *token::to_keyword_do()
        {
            return nullptr;
        }

        statement_do_while *token::to_statement_do_while()
        {
            return nullptr;
        }

        keyword_break *token::to_keyword_break()
        {
            return nullptr;
        }

        keyword_continue *token::to_keyword_continue()
        {
            return nullptr;
        }

        statement_break *token::to_statement_break()
        {
            return nullptr;
        }

        statement_continue *token::to_statement_continue()
        {
            return nullptr;
        }

        keyword_switch *token::to_keyword_switch()
        {
            return nullptr;
        }

        keyword_case *token::to_keyword_case()
        {
            return nullptr;
        }

        keyword_default *token::to_keyword_default()
        {
            return nullptr;
        }

        statement_switch *token::to_statement_switch()
        {
            return nullptr;
        }

        unary_suffix *token::to_unary_suffix()
        {
            return nullptr;
        }

        suffix_increment *token::to_suffix_increment()
        {
            return nullptr;
        }

        decrement *token::to_decrement()
        {
            return nullptr;
        }

        prefix_decrement *token::to_prefix_decrement()
        {
            return nullptr;
        }

        suffix_decrement *token::to_suffix_decrement()
        {
            return nullptr;
        }

        keyword_import *token::to_keyword_import()
        {
            return nullptr;
        }

        asterisk *token::to_asterisk()
        {
            return nullptr;
        }

        double_asterisk *token::to_double_asterisk()
        {
            return nullptr;
        }

        slash *token::to_slash()
        {
            return nullptr;
        }

        percent *token::to_percent()
        {
            return nullptr;
        }

        multiplication *token::to_multiplication()
        {
            return nullptr;
        }

        exponentiation *token::to_exponentiation()
        {
            return nullptr;
        }

        division *token::to_division()
        {
            return nullptr;
        }

        remainder *token::to_remainder()
        {
            return nullptr;
        }

        unary_plus *token::to_unary_plus()
        {
            return nullptr;
        }

        exclamation *token::to_exclamation()
        {
            return nullptr;
        }

        tilde *token::to_tilde()
        {
            return nullptr;
        }

        logical_not *token::to_logical_not()
        {
            return nullptr;
        }

        bitwise_not *token::to_bitwise_not()
        {
            return nullptr;
        }

        double_exclamation *token::to_double_exclamation()
        {
            return nullptr;
        }

        operator_bool *token::to_operator_bool()
        {
            return nullptr;
        }

        left_shift *token::to_left_shift()
        {
            return nullptr;
        }

        signed_right_shift *token::to_signed_right_shift()
        {
            return nullptr;
        }

        zero_fill_right_shift *token::to_zero_fill_right_shift()
        {
            return nullptr;
        }

        double_less *token::to_double_less()
        {
            return nullptr;
        }

        double_greater *token::to_double_greater()
        {
            return nullptr;
        }

        triple_greater *token::to_triple_greater()
        {
            return nullptr;
        }

        is_less_than_or_equal_to *token::to_is_less_than_or_equal_to()
        {
            return nullptr;
        }

        is_greater_than *token::to_is_greater_than()
        {
            return nullptr;
        }

        is_greater_than_or_equal_to *token::to_is_greater_than_or_equal_to()
        {
            return nullptr;
        }

        less_or_equal *token::to_less_or_equal()
        {
            return nullptr;
        }

        greater *token::to_greater()
        {
            return nullptr;
        }

        greater_or_equal *token::to_greater_or_equal()
        {
            return nullptr;
        }

        bitwise_and *token::to_bitwise_and()
        {
            return nullptr;
        }

        bitwise_or *token::to_bitwise_or()
        {
            return nullptr;
        }

        bitwise_xor *token::to_bitwise_xor()
        {
            return nullptr;
        }

        logical_and *token::to_logical_and()
        {
            return nullptr;
        }

        logical_or *token::to_logical_or()
        {
            return nullptr;
        }

        ampersand *token::to_ampersand()
        {
            return nullptr;
        }

        double_ampersand *token::to_double_ampersand()
        {
            return nullptr;
        }

        vertical_bar *token::to_vertical_bar()
        {
            return nullptr;
        }

        double_vertical_bar *token::to_double_vertical_bar()
        {
            return nullptr;
        }

        caret *token::to_caret()
        {
            return nullptr;
        }

        assignment_by_sum *token::to_assignment_by_sum()
        {
            return nullptr;
        }

        assignment_by_difference *token::to_assignment_by_difference()
        {
            return nullptr;
        }

        assignment_by_product *token::to_assignment_by_product()
        {
            return nullptr;
        }

        assignment_by_quotient *token::to_assignment_by_quotient()
        {
            return nullptr;
        }

        assignment_by_remainder *token::to_assignment_by_remainder()
        {
            return nullptr;
        }

        assignment_by_left_shift *token::to_assignment_by_left_shift()
        {
            return nullptr;
        }

        assignment_by_signed_right_shift *token::to_assignment_by_signed_right_shift()
        {
            return nullptr;
        }

        assignment_by_zero_fill_right_shift *token::to_assignment_by_zero_fill_right_shift()
        {
            return nullptr;
        }

        assignment_by_bitwise_and *token::to_assignment_by_bitwise_and()
        {
            return nullptr;
        }

        assignment_by_bitwise_or *token::to_assignment_by_bitwise_or()
        {
            return nullptr;
        }

        assignment_by_bitwise_xor *token::to_assignment_by_bitwise_xor()
        {
            return nullptr;
        }

        plus_assign *token::to_plus_assign()
        {
            return nullptr;
        }

        minus_assign *token::to_minus_assign()
        {
            return nullptr;
        }

        asterisk_assign *token::to_asterisk_assign()
        {
            return nullptr;
        }

        slash_assign *token::to_slash_assign()
        {
            return nullptr;
        }

        percent_assign *token::to_percent_assign()
        {
            return nullptr;
        }

        double_less_assign *token::to_double_less_assign()
        {
            return nullptr;
        }

        double_greater_assign *token::to_double_greater_assign()
        {
            return nullptr;
        }

        triple_greater_assign *token::to_triple_greater_assign()
        {
            return nullptr;
        }

        ampersand_assign *token::to_ampersand_assign()
        {
            return nullptr;
        }

        vertical_bar_assign *token::to_vertical_bar_assign()
        {
            return nullptr;
        }

        caret_assign *token::to_caret_assign()
        {
            return nullptr;
        }

        question_mark *token::to_question_mark()
        {
            return nullptr;
        }

        ternary *token::to_ternary()
        {
            return nullptr;
        }

        parenthesized_expression *token::to_parenthesized_expression()
        {
            return nullptr;
        }

        keyword_debug *token::to_keyword_debug()
        {
            return nullptr;
        }

        statement_debug *token::to_statement_debug()
        {
            return nullptr;
        }

        protect *token::to_protect()
        {
            return nullptr;
        }

        protection *token::to_protection()
        {
            return nullptr;
        }

        question_with_dot *token::to_question_with_dot()
        {
            return nullptr;
        }
    }
};
