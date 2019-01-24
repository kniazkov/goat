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
#include "lib/pointer.h"

namespace g0at
{
    class scanner;

    namespace ast
    {
        extern int __tok_count;

        class nonterminal;
        class expression;
        class statement;
        class token_with_list;
        class function;
        class root;
        class identifier;
        class bracket;
        class static_string;
        class semicolon;
        class brackets_pair;
        class function_call;
        class statement_expression;

        class token
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
            virtual statement *to_statement();
            virtual token_with_list *to_token_with_list();
            virtual function *to_function();
            virtual root *to_root();
            virtual identifier *to_identifier();
            virtual bracket *to_bracket();
            virtual static_string *to_static_string();
            virtual semicolon *to_semicolon();
            virtual brackets_pair *to_brackets_pair();
            virtual function_call *to_function_call();
            virtual statement_expression *to_statement_expression();

            lib::pointer<position> get_position() { return pos; }

            int refs;

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
