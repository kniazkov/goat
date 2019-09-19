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

#include "token_2nd_list.h"
#include "token.h"
#include "lib/assert.h"

namespace g0at
{
    namespace ast
    {
        token_2nd_list::token_2nd_list()
            : first(nullptr), last(nullptr)
        {
        }

        void token_2nd_list::add(token *item)
        {
            if (item->list_2 != nullptr)
                item->list_2->remove(item);

            item->list_2 = this;
            item->prev_2 = last;
            item->next_2 = nullptr;

            if (last)
                last->next_2 = item;
            else
                first = item;
            last = item;
        }

        void token_2nd_list::remove(token *item)
        {
            assert(item->list_2 == this);

            if (item->prev_2)
                item->prev_2->next_2 = item->next_2;
            else 
                first = item->next_2;

            if (item->next_2)
                item->next_2->prev_2 = item->prev_2;
            else
                last = item->prev_2;

            item->list_2 = nullptr;
            item->prev_2 = nullptr;
            item->next_2 = nullptr;
        }
    };
};
