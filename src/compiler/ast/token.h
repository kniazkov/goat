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
#include "../source/position.h"
#include <memory>

namespace g0at
{
    namespace ast
    {
        extern int __tok_count;

        class nonterminal;
        class expression;
        class token_with_list;
        class function;
        class root;
        class identifier;
        class bracket;
        class static_string;
        class semicolon;
        class brackets_pair;
        class function_call;

        class token
        {
        public:
            token();
            virtual ~token();
            virtual void accept(token_visitor *visitor) = 0;
            virtual nonterminal *to_nonterminal();
            virtual expression *to_expression();
            virtual token_with_list *to_token_with_list();
            virtual function *to_function();
            virtual root *to_root();
            virtual identifier *to_identifier();
            virtual bracket *to_bracket();
            virtual static_string *to_static_string();
            virtual semicolon *to_semicolon();
            virtual brackets_pair *to_brackets_pair();
            virtual function_call *to_function_call();

            std::shared_ptr<position> pos;

            token_list *list;
            token *prev;
            std::shared_ptr<token> next;

            token_2nd_list *list_2;
            token *prev_2;
            token *next_2;
        };
    };
};
