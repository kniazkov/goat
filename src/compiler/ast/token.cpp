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
#include <assert.h>
//#include <iostream>

namespace g0at
{
    namespace ast
    {
        int __tok_count;

        token::token()
            : list(nullptr), prev(nullptr), next(nullptr)
        {
            __tok_count++;
            //std::cout << "*";
        }

        token::~token()
        {
            __tok_count--;
            //std::cout << "#";
        }

        void token::replace(std::shared_ptr<token> repl)
        {
            assert(list != nullptr);
            list->replace(this, this, repl);
        }

        void token::replace(token *end, std::shared_ptr<token> repl)
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
            assert(list_2 != nullptr);
            list_2->remove(this);
        }

        nonterminal *token::to_nonterminal()
        {
            return nullptr;
        }

        expression *token::to_expression()
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
    }
};
