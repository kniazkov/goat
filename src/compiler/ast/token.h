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
