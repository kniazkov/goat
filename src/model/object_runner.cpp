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

#include "object_runner.h"
#include "object_function_built_in.h"
#include "object_string.h"
#include "lib/assert.h"
#include <sstream>

namespace g0at
{
    namespace model
    {
        object_runner::object_runner(object_pool *pool, thread_id _tid)
            : object(pool, pool->get_runner_proto_instance()), tid(_tid)
        {
        }

        object_runner * object_runner::to_object_runner()
        {
            return this;
        }

        std::wstring object_runner::to_string() const
        {
            return L"runner";
        }

        /* 
            Prototype
        */

       /*
          TODO: move this code to separated method

            if (mode != call_mode::as_method)
            {
                thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                return;
            }
            object *this_ptr = thr->pop().get_object();
            assert(this_ptr != nullptr);
            object_runner *runner = this_ptr->to_object_runner();
            if (!runner)
            {
                thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                return;
            }
            thr->pop(arg_count);
        */

        class object_runner_get_id : public object_function_built_in
        {
        public:
            object_runner_get_id(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);
                object_runner *runner = this_ptr->to_object_runner();
                if (!runner)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                thr->pop(arg_count);

                variable var;
                var.set_integer(runner->get_thread_id().as_int64());
                thr->push(var);
            }
        };

        class object_runner_alive : public object_function_built_in
        {
        public:
            object_runner_alive(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);
                object_runner *runner = this_ptr->to_object_runner();
                if (!runner)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                thr->pop(arg_count);

                variable var;
                thread_id tid = runner->get_thread_id();
                thread *thr_by_tid = thr->get_thread_list()->get_thread_by_tid(tid);
                if (thr_by_tid)
                    var.set_boolean(thr_by_tid->state != thread_state::zombie);
                else
                    var.set_boolean(false);
                thr->push(var);
            }
        };

        class object_runner_join : public object_function_built_in
        {
        public:
            object_runner_join(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);
                object_runner *runner = this_ptr->to_object_runner();
                if (!runner)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                thr->pop(arg_count);

                variable var;
                thread_id tid = runner->get_thread_id();
                thread *thr_by_tid = thr->get_thread_list()->get_thread_by_tid(tid);
                if (thr_by_tid && thr_by_tid != thr)
                {
                    var.set_boolean(true);
                    thr_by_tid->joined.push_back(thr);
                    thr->state = thread_state::pause;
                }
                else
                {
                    var.set_boolean(false);
                }
                thr->push(var);
            }
        };

        object_runner_proto::object_runner_proto(object_pool *pool)
            : object(pool, pool->get_function_proto_instance())
        {
        }

        void object_runner_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(L"id"), new object_runner_get_id(pool));
            add_object(pool->get_static_string(L"alive"), new object_runner_alive(pool));
            add_object(pool->get_static_string(L"join"), new object_runner_join(pool));
        }
    };
};
