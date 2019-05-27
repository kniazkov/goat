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

#include "token.h"
#include "lib/assert.h"

namespace g0at
{
    namespace ast
    {
        token::token()
            : pos(nullptr), 
            list(nullptr), prev(nullptr), next(nullptr), list_2(nullptr), prev_2(nullptr), next_2(nullptr)
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
    }
};
