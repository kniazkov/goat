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
#include <map>

namespace g0at
{
    namespace model
    {
        class thread_list;

        enum class thread_state
        {
            pause,
            ok,
            zombie
        };

        class thread
        {
            friend class thread_list;
        protected:
            thread(thread_list *_list, int64_t _tid, context *_ctx, object_pool *_pool, variable *_ret);

        public:
            void raise_exception(variable &var);
            void mark_all();

            thread_list *get_thread_list() { return list; }
            int64_t get_id() { return tid; }
            variable *push(variable var) { return data.push(var); }
            variable pop() { return data.pop(); }
            void pop(int n) { data.pop(n); }
            variable &peek() { return data.peek(); }
            variable &peek(int n) { return data.peek(n); }
            bool stack_is_empty() { return data.empty(); }

            variable *push_undefined()
            {
                variable var;
                var.set_object(pool->get_undefined_instance());
                return data.push(var);
            }

            void raise_exception(object *obj)
            {
                variable var;
                var.set_object(obj);
                raise_exception(var);
            }

            void set_context(context *_ctx)
            {
                ctx = _ctx;
                ctx->stack_size = data.size();
            }

            void restore_context()
            {
                data.restore_size(ctx ? ctx->stack_size : 0);
                ctx = ctx->prev;
            }

            thread *next;
            uint32_t iid;
            thread_state state;
            context *ctx;
            object_pool *pool;
            variable *ret;

        protected:
            thread(const thread &) { }
            void operator=(const thread &) { }

            stack data;
            thread_list *list;
            int64_t tid;
        };

        class thread_list
        {
        public:
            thread_list(object_pool *_pool);
            thread *create_thread(context *_ctx, variable *_ret);
            thread *switch_thread();

            thread *get_current_thread() { return current; }
            thread *get_thread_by_tid(int64_t tid)
            {
                auto iter = thread_by_tid.find(tid);
                return iter != thread_by_tid.end() ? iter->second : nullptr;
            }

        protected:
            thread_list(const thread_list &) { }
            void operator=(const thread_list &) { }

            object_pool *pool;
            std::map<int64_t, thread*> thread_by_tid;
            thread *current;
            int64_t last_tid;
        };
    };
};