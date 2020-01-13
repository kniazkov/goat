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

#include "thread.h"
#include "lib/assert.h"
#include "lib/exception.h"
#include "lib/utils.h"
#include "process.h"

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

        thread::thread(process *_proc, thread_id _tid, context *_ctx, object_pool *_pool, variable *_ret)
            : proc(_proc), tid(_tid), is_active(true), prev(nullptr), next(nullptr), state(thread_state::pause),
              flow(thread_flow::direct), ctx(_ctx), pool(_pool), ret(_ret), runner(nullptr), lock(0),
              debug_level(0), debug_state(thread_debug_state::do_not_stop)
        {
            except.set_object(pool->get_undefined_instance());
        }

        void thread::mark_all()
        {
            if (ret)
                ret->mark();

            except.mark();

            if (ctx)
                ctx->mark();

            if (runner)
                runner->mark();

            data.mark_all();
        }        

        void thread::mark_all_parallel()
        {
            if (ret)
                ret->mark_parallel(pool);

            except.mark_parallel(pool);

            if (ctx)
                ctx->mark_parallel(pool);

            if (runner)
                runner->mark_parallel(pool);

            data.mark_all_parallel(pool);
        }

        void thread::resume() {
            if (state == thread_state::pause) {
                state = thread_state::ok;
                if (!is_active) {

                    proc->suspended_threads->remove_thread(this);
                    proc->active_threads->add_thread(this);
                    is_active = true;
                }
            }
        }

        void thread::kill() {
            if (state != thread_state::zombie) {
                state = thread_state::zombie;
                if (!is_active) {

                    proc->suspended_threads->remove_thread(this);
                    proc->active_threads->add_thread(this);
                    is_active = true;
                }
            }
        }

        void thread::raise_exception(variable &var)
        {
            flow = thread_flow::direct;
            except = var;
            if (ctx->address_type == context_address_type::catch_address)
            {
                iid = ctx->address[0];
                return;
            }
            else
            {
                restore_context();
                while (ctx)
                {
                    switch(ctx->address_type)
                    {
                        case context_address_type::catch_address :
                            iid = ctx->address[0];
                            return;
                        case context_address_type::fin_address :
                            flow = thread_flow::descent_exception;
                            iid = ctx->address[0];
                            return;
                        default:
                            restore_context();
                    }
                }
                throw unhandled_runtime_exception(var.to_string());
            }
        }

        thread_list::thread_list(process *_proc)
            : proc(_proc), current(nullptr)
        {
        }

        thread_list::~thread_list()
        {
        }

        void thread_list::add_thread(thread *new_thr)
        {
            if (current)
            {
                // insert before current
                new_thr->next = current;
                new_thr->prev = current->prev;
                current->prev->next = new_thr;
                current->prev = new_thr;
            }
            else
            {
                current = new_thr->next = new_thr->prev = new_thr;
            }
        }

        void thread_list::remove_current_thread()
        {
            if (current->prev == current) // all other threads are paused or deleted
                current = nullptr;
            else
            {
                current->prev->next = current->next;
                current->next->prev = current->prev;
                current = current->next;
            }
        }

        void thread_list::remove_thread(thread *thr)
        {
            if (thr == current)
                remove_current_thread();
            else 
                thr->prev->next = thr->next;
                thr->next->prev = thr->prev;
        }

        thread_list_ext::thread_list_ext(process *_proc, thread_list *_aux_list, object_pool *_pool)
            : thread_list(_proc), aux_list(_aux_list), pool(_pool), last_tid(-1)
        {
        }

        thread * thread_list_ext::create_thread(context *_ctx, variable *_ret)
        {
            last_tid = thread_id(last_tid.as_int64() + 1);
            thread_id tid = last_tid;
            thread *new_thr = new thread(proc, tid, _ctx, pool, _ret);
            add_thread(new_thr);
            thread_by_tid[tid] = new_thr;
            return new_thr;
        }

        thread * thread_list_ext::create_delayed_thread(context *_ctx, int64_t delay)
        {
            last_tid = thread_id(last_tid.as_int64() + 1);
            thread_id tid = last_tid;
            thread *new_thr = new thread(proc, tid, _ctx, pool, nullptr);
            aux_list->add_thread(new_thr);
            new_thr->is_active = false;
            thread_by_tid[tid] = new_thr;
            delayed_threads[delay] = tid;
            return new_thr;
        }

        thread * thread_list_ext::switch_thread(bool *stop)
        {
            if (current == nullptr) // no active threads
                return nullptr;

            if (current->lock > 0)
                return current; // don't switch

            current = current->next;
            while(current != nullptr) {
                switch(current->state) {
                    case thread_state::ok:
                        return current;
                    case thread_state::pause: // move thread to the aux list
                    {
                        thread *moved = current;
                        remove_current_thread();
                        moved->is_active = false;
                        aux_list->add_thread(moved);
                        break;
                    }
                    case thread_state::zombie: // need to delete this thread
                    {
                        thread *deleted = current;
                        remove_current_thread();
                        for (thread_id joined_id : deleted->joined)
                        {
                            thread* joined = get_thread_by_tid(joined_id);
                            if (joined)
                                joined->resume();
                        }
                        if (current == nullptr && aux_list->current == nullptr) // no more threads
                        {
                            *stop = true;
                            assert(deleted->stack_is_empty());
                        }
                        thread_by_tid.erase(deleted->tid);
                        delete deleted;
                        break;
                    }
                }
            }
            return nullptr;
        }

        thread * thread_list_ext::process_delayed_threads()
        {
            if (delayed_threads.empty())
                return nullptr;
            
            int64_t time = lib::get_time_ns();
            auto pair = delayed_threads.begin();
            if (pair->first > time)
                return nullptr;

            thread *delayed = get_thread_by_tid(pair->second);
            delayed_threads.erase(pair);
            if (delayed != nullptr)
                delayed->resume();
            return delayed;
        }
    };
};
