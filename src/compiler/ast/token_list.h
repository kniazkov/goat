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

#include "lib/pointer.h"

namespace g0at
{
    namespace ast
    {
        class token;

        class token_list
        {
        public:
            token_list();
            void swap(token_list *other);
            void add(lib::pointer<token> item);
            void add_after(lib::pointer<token> item, token *after);
            void remove(lib::pointer<token> item);
            void replace(lib::pointer<token> begin, lib::pointer<token> end, lib::pointer<token> repl);

            bool is_empty() { return first == nullptr; }
            void clear()
            {
                last = nullptr;
                first.reset();
            }
            bool has_only_one_item() { return first != nullptr && first == last; }

            lib::pointer<token> first;
            token *last;

        private:
            token_list(const token_list &) { }
            void operator=(const token_list &) { }
        };
    };
};
