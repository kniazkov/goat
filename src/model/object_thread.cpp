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

#include "object_thread.h"
#include "object_function_built_in.h"
#include "object_string.h"
#include "lib/assert.h"

namespace g0at
{
    namespace model
    {
        object_thread::object_thread(object_pool *pool, int _first_iid, context *_proto_ctx)
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

        /* 
            Prototype
        */

        /**
         * @brief Built-in method 'run()' for threads
         * 
         * The 'run()' method runs a Goat thread.
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
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                object *this_ptr = thr->pop().get_object();
                thr->pop(arg_count);
                thr->push_undefined();

                assert(this_ptr != nullptr);
                object_thread *this_ptr_func = this_ptr->to_object_thread();
                if (!this_ptr_func)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                thread *new_thr = new thread(thr->ctx, thr->pool, nullptr);
                new_thr->next = thr->next;
                thr->next = new_thr;
                new_thr->state = thread_state::ok;
                new_thr->iid = this_ptr_func->get_first_iid();
                return;
            }
        };

        object_thread_proto::object_thread_proto(object_pool *pool)
            : object(pool, pool->get_function_proto_instance())
        {
        }

        void object_thread_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(L"run"), new object_thread_run(pool));
        }
    };
};
