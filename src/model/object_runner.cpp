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

        class object_runner_method : public object_function_built_in
        {
        public:
            object_runner_method(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            virtual variable payload(thread *this_thr, thread *other_thr) = 0;

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
                thread_id tid = runner->get_thread_id();
                thread *thr_by_tid = thr->get_thread_list()->get_thread_by_tid(tid);
                variable var = payload(thr, thr_by_tid);
                thr->push(var);
            }
        };

        class object_runner_get_id : public object_runner_method
        {
        public:
            object_runner_get_id(object_pool *_pool)
                : object_runner_method(_pool)
            {
            }
            
            variable payload(thread *this_thr, thread *other_thr)
            {
                variable var;
                if (other_thr)
                    var.set_integer(other_thr->get_id().as_int64());
                else
                    var.set_object(this_thr->pool->get_null_instance());
                return var;
            }
        };

        class object_runner_alive : public object_runner_method
        {
        public:
            object_runner_alive(object_pool *_pool)
                : object_runner_method(_pool)
            {
            }
            
            variable payload(thread *this_thr, thread *other_thr)
            {
                variable var;
                if (other_thr)
                    var.set_boolean(other_thr->state != thread_state::zombie);
                else
                    var.set_boolean(false);
                return var;
            }
        };

        class object_runner_join : public object_runner_method
        {
        public:
            object_runner_join(object_pool *_pool)
                : object_runner_method(_pool)
            {
            }
            
            variable payload(thread *this_thr, thread *other_thr)
            {
                variable var;
                if (other_thr && other_thr != this_thr)
                {
                    var.set_boolean(true);
                    other_thr->joined.push_back(this_thr);
                    this_thr->state = thread_state::pause;
                }
                else
                    var.set_boolean(false);
                return var;
            }
        };

        class object_runner_kill : public object_runner_method
        {
        public:
            object_runner_kill(object_pool *_pool)
                : object_runner_method(_pool)
            {
            }
            
            variable payload(thread *this_thr, thread *other_thr)
            {
                variable var;
                if (other_thr && other_thr->state != thread_state::zombie)
                {
                    var.set_boolean(true);
                    other_thr->state = thread_state::zombie;
                }
                else
                    var.set_boolean(false);
                return var;
            }
        };

        class object_runner_suspend : public object_runner_method
        {
        public:
            object_runner_suspend(object_pool *_pool)
                : object_runner_method(_pool)
            {
            }
            
            variable payload(thread *this_thr, thread *other_thr)
            {
                variable var;
                if (other_thr && other_thr->state == thread_state::ok)
                {
                    var.set_boolean(true);
                    other_thr->state = thread_state::pause;
                }
                else
                    var.set_boolean(false);
                return var;
            }
        };

        class object_runner_resume : public object_runner_method
        {
        public:
            object_runner_resume(object_pool *_pool)
                : object_runner_method(_pool)
            {
            }
            
            variable payload(thread *this_thr, thread *other_thr)
            {
                variable var;
                if (other_thr && other_thr->state == thread_state::pause)
                {
                    var.set_boolean(true);
                    other_thr->state = thread_state::ok;
                }
                else
                    var.set_boolean(false);
                return var;
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
            add_object(pool->get_static_string(L"kill"), new object_runner_kill(pool));
            add_object(pool->get_static_string(L"suspend"), new object_runner_suspend(pool));
            add_object(pool->get_static_string(L"resume"), new object_runner_resume(pool));
        }
    };
};
