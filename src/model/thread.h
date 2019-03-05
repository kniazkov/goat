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
#include "stack.h"

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
            thread(context *_ctx, object_pool *_o_pool, object_cache *_cache);

            variable *push(variable var) { return data.push(var); }
            variable *push_undefined()
            {
                variable var;
                var.set_object(o_pool->get_undefined_instance());
                return data.push(var);
            }
            variable pop() { return data.pop(); }
            void pop(int n) { data.pop(n); }
            variable &peek() { return data.peek(); }
            variable &peek(int n) { return data.peek(n); }
            bool stack_is_empty() { return data.empty(); }
            int get_stack_size() { return data.size(); }

            thread *next;
            uint32_t iid;
            thread_state state;
            context *ctx;
            object_pool *o_pool;
            object_cache  *cache;

        protected:
            thread(const thread &) { }
            void operator=(const thread &) { }

            stack data;
        };
    };
};