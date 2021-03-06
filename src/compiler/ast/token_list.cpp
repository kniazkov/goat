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

#include "token_list.h"
#include "token.h"
#include "assert.h"
#include <utility>

namespace g0at
{
    namespace ast
    {
        token_list::token_list()
            : first(nullptr), last(nullptr)
        {
        }

        void token_list::swap(token_list *other)
        {
            lib::pointer<token> tok = first;
            while (tok)
            {
                tok->list = other;
                tok = tok->next;
            }

            tok = other->first;
            while (tok)
            {
                tok->list = this;
                tok = tok->next;
            }

            first.swap(other->first);
            std::swap(last, other->last);
        }

        void token_list::add(lib::pointer<token> item)
        {
            if (item->list != nullptr)
                item->list->remove(item.get());

            item->list = this;
            item->prev = last;
            item->next = nullptr;

            if (last)
                last->next = item;
            else
                first = item;
            last = item.get();
        }

        void token_list::add_after(lib::pointer<token> item, token *after)
        {
            if (item->list != nullptr)
                item->list->remove(item.get());

            item->prev = after;
            item->next = after->next;
            item->list = this;

            if (!after->next)
                last = item.get();
            else
                after->next->prev = item.get();
            after->next = item;
        }

        void token_list::remove(lib::pointer<token> item)
        {
            assert (item->list == this);

            if (item->prev)
                item->prev->next = item->next;
            else 
                first = item->next;

            if (item->next)
                item->next->prev = item->prev;
            else
                last = item->prev;

            item->list = nullptr;
            item->prev = nullptr;
            item->next.reset();
        }

        void token_list::replace(lib::pointer<token> begin, lib::pointer<token> end, lib::pointer<token> repl)
        {
            assert (begin->list == this && end->list == this);

            if (repl->list != nullptr)
                repl->list->remove(repl.get());
            repl->list = this;

            if (end->next)
            {
                end->next->prev = repl.get();
                repl->next = end->next;
            }
            else
            {
                last = repl.get();
                repl->next = nullptr;
            }

            if (begin->prev)
            {
                begin->prev->next = repl;
                repl->prev = begin->prev;
            }
            else
            {
                first = repl;
                repl->prev = nullptr;
            }

            token *item = end.get();
            while (item != begin.get())
            {
                item->remove_2nd();
                item->next.reset();
                item = item->prev;
            }
            
            begin->remove_2nd();
        }
        
    };
};
