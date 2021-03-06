/*

Copyright (C) 2017-2021 Ivan Kniazkov

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

#include "object_thread.h"
#include "object_function_built_in.h"
#include "object_string.h"
#include "object_runner.h"
#include "object_exception.h"
#include "process.h"
#include "thread.h"
#include "lib/assert.h"
#include "lib/utils.h"
#include "resource/strings.h"

namespace g0at
{
    namespace model
    {
        object_thread::object_thread(object_pool *pool, code::iid_t _first_iid, context *_proto_ctx)
            : object_function_user_defined(pool, _first_iid, _proto_ctx)
        {
            proto = pool->get_thread_proto_instance();
        }

        object_thread * object_thread::to_object_thread()
        {
            return this;
        }

        std::wstring object_thread::to_string() const
        {
            return L"thread";
        }

        goat_value * object_thread::get_value(const goat_allocator *allocator)
        {
            return create_goat_thread(allocator, this);
        }

        /* 
            Prototype
        */

        /**
         * @brief Built-in method 'run()' for threads
         * 
         * The 'run()' method runs a Goat thread and returns instance of Runner.
         */
        class object_thread_run : public object_function_built_in
        {
        public:
            object_thread_run(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(new object_exception_illegal_context(thr->pool));
                    return;
                }
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);
                object_thread *obj_thread = this_ptr->to_object_thread();
                if (!obj_thread)
                {
                    thr->raise_exception(new object_exception_illegal_context(thr->pool));
                    return;
                }
                context *ctx = thr->pool->create_context(obj_thread->get_proto_ctx(), thr->ctx);
                ctx->address_type = context_address_type::stop;
                int decl_arg_count = obj_thread->get_arg_names_count();
                for (int i = 0; i < decl_arg_count; i++)
                {
                    object *key = obj_thread->get_arg_name(i);
                    if (i < arg_count)
                    {
                        variable arg = thr->pop();
                        ctx->add_object(key, arg);
                    }
                    else
                        ctx->add_object(key, thr->pool->get_undefined_instance());
                }
                if (arg_count > decl_arg_count)
                {
                    thr->pop(arg_count - decl_arg_count);
                }

                thread *new_thr = thr->get_process()->active_threads->create_thread(ctx, nullptr, thr->pool);
                new_thr->state = thread_state::ok;
                new_thr->iid = obj_thread->get_first_iid();
                
                variable runner;
                runner.set_object(new object_runner(thr->pool, new_thr->get_id()));
                thr->push(runner);
            }
        };

        /**
         * @brief Built-in method 'delay()' for threads
         * 
         * The 'delay()' method runs a Goat thread after a time and returns instance of Runner.
         */
        class object_thread_delay : public object_function_built_in
        {
        public:
            object_thread_delay(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(new object_exception_illegal_context(thr->pool));
                    return;
                }
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);
                object_thread *obj_thread = this_ptr->to_object_thread();
                if (!obj_thread)
                {
                    thr->raise_exception(new object_exception_illegal_context(thr->pool));
                    return;
                }
                context *ctx = thr->pool->create_context(obj_thread->get_proto_ctx(), thr->ctx);
                ctx->address_type = context_address_type::stop;
                if (arg_count < 0)
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return;
                }
                variable arg_delay = thr->peek(0);
                int64_t delay;
                if (!arg_delay.get_integer(&delay))
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return;
                }
                if (delay < 0)
                    delay = 0;

                thr->pop(arg_count);

                thread *new_thr = thr->get_process()->active_threads->
                    create_delayed_thread(ctx, delay + lib::get_time_ns(), thr->pool);
                new_thr->iid = obj_thread->get_first_iid();
                
                variable runner;
                runner.set_object(new object_runner(thr->pool, new_thr->get_id()));
                thr->push(runner);
            }
        };

        /**
         * @brief Built-in method 'current()' for threads
         * 
         * The 'current()' method returns current thread
         */
        class current_thread : public object_function_built_in
        {
        public:
            current_thread(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode == call_mode::as_method)
                    thr->pop();
                thr->pop(arg_count);
                if (!thr->runner)
                    thr->runner = new object_runner(thr->pool, thr->get_id());
                variable result;
                result.set_object(thr->runner);
                thr->push(result);
            }
        };

        object_thread_proto::object_thread_proto(object_pool *pool)
            : object(pool, pool->get_function_proto_instance())
        {
        }

        void object_thread_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(resource::str_run), new object_thread_run(pool));
            add_object(pool->get_static_string(resource::str_delay), new object_thread_delay(pool));
            add_object(pool->get_static_string(resource::str_current), new current_thread(pool));
            lock();
        }

        void object_thread_proto::op_new(thread *thr, int arg_count)
        {
            thr->raise_exception(new object_exception_illegal_operation(thr->pool));
        }
    };
};
