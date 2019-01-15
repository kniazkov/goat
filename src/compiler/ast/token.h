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
#include "../source/position.h"
#include <memory>

namespace g0at
{
    namespace ast
    {
        extern int __tok_count;

        class nonterminal;
        class token_with_list;
        class function;
        class root;
        class identifier;
        class bracket;
        class static_string;
        class semicolon;
        class brackets_pair;

        class token
        {
        public:
            token();
            virtual ~token();
            virtual void accept(token_visitor *visitor) = 0;
            virtual nonterminal *to_nonterminal();
            virtual token_with_list *to_token_with_list();
            virtual function *to_function();
            virtual root *to_root();
            virtual identifier *to_identifier();
            virtual bracket *to_bracket();
            virtual static_string *to_static_string();
            virtual semicolon *to_semicolon();
            virtual brackets_pair *to_brackets_pair();

            std::shared_ptr<position> pos;
            token_list *list;
            token *prev;
            std::shared_ptr<token> next;
        };
    };
};
