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

#include "context.h"
#include "object_cache.h"
#include <deque>

namespace g0at
{
    namespace model
    {
        enum class thread_state
        {
            pause,
            ok,
            zombie
        };

        class thread
        {
        public:
            thread(context *_ctx, object_list *_o_list, object_cache *_cache);

            void push(variable var) { data.push_front(var); }
            variable pop()
            {
                variable var = data.front();
                data.pop_front();
                return var;
            }
            void pop(int n)
            {
                while (n > 0)
                {
                    data.pop_front();
                    n--;
                }
            }
            variable &peek() { return *(data.begin()); }
            variable &peek(int idx) { return data.begin()[idx]; }
            bool stack_is_empty() { return data.empty(); }

            uint32_t iid;
            thread_state state;
            context *ctx;
            object_list *o_list;
            object_cache  *cache;
            variable ret;

        protected:
            thread(const thread &) { }
            void operator=(const thread &) { }

            std::deque<variable> data;
        };
    };
};