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

#include "thread.h"
#include "lib/assert.h"
#include "lib/exception.h"

namespace g0at
{
    namespace model
    {
        class unhandled_runtime_exception : public lib::exception
        {
        public:
            unhandled_runtime_exception(std::wstring str)
                : exception(global::resource->unhandled_exception(str))
            {
            }
        };

        thread::thread(thread_list *_list, thread_id _tid, context *_ctx, object_pool *_pool, variable *_ret)
            : list(_list), tid(_tid), next(nullptr), state(thread_state::pause),
              flow(thread_flow::direct), ctx(_ctx), pool(_pool), ret(_ret), lock(0)
        {
            except.set_object(pool->get_undefined_instance());
        }

        void thread::mark_all()
        {
            if (ret)
                ret->mark();

            if (ctx)
                ctx->mark();

            data.mark_all();
        }        

        void thread::raise_exception(variable &var)
        {
            flow = thread_flow::direct;
            except = var;
            if (ctx->value_type == context_value_type::catch_address)
            {
                iid = ctx->value;
                return;
            }
            else
            {
                restore_context();
                while (ctx)
                {
                    switch(ctx->value_type)
                    {
                        case context_value_type::catch_address :
                            iid = ctx->value;
                            return;
                        case context_value_type::fin_address :
                            flow = thread_flow::descent_exception;
                            iid = ctx->value;
                            return;
                        default:
                            restore_context();
                    }
                }
                throw unhandled_runtime_exception(var.to_string());
            }
        }

        thread_list::thread_list(object_pool *_pool)
            : pool(_pool), current(nullptr), last_tid(-1)
        {
        }

        thread * thread_list::create_thread(context *_ctx, variable *_ret)
        {
            last_tid = thread_id(last_tid.as_int64() + 1);
            thread_id tid = last_tid;
            thread *new_thr = new thread(this, tid, _ctx, pool, _ret);
            if (current)
            {
                new_thr->next = current->next;
                current->next = new_thr;
            }
            else
            {
                current = new_thr;
            }
            thread_by_tid[tid] = new_thr;
            
            return new_thr;
        }

        thread * thread_list::switch_thread()
        {
            assert(current != nullptr);

            if (current->lock > 0)
            {
                return current;
            }

            thread *first = current;
            thread *previous = current;
            current = current->next;
            while(current->state == thread_state::pause)
            {
                if (first == current)
                {
                    // all threads are paused
                    return nullptr;
                }
                previous = current;
                current = current->next;
            }
            if (current->state == thread_state::zombie)
            {
                if (previous == current)
                {
                    // this is a last thread
                    assert(current->stack_is_empty());
                    thread_by_tid.erase(current->tid);
                    delete current;
                    current = nullptr;
                    return nullptr;
                }
                else
                {
                    for (thread *joined : current->joined)
                    {
                        if (joined->state == thread_state::pause)
                            joined->state = thread_state::ok;
                    }
                    thread *next = current->next;
                    assert(next->state == model::thread_state::ok);
                    previous->next = next;
                    thread_by_tid.erase(current->tid);
                    delete current;
                    current = next;
                }
            }
            return current;
        }
    };
};
