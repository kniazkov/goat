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

#include "object_function.h"
#include "object_function_built_in.h"
#include "object_string.h"
#include "object_array.h"
#include "object_exception.h"
#include "lib/assert.h"

namespace g0at
{
    namespace model
    {
        object_function::object_function(object_pool *pool)
            : object(pool, pool->get_function_proto_instance())
        {
        } 

        object_type object_function::get_type() const
        {
            return object_type::function;
        }

        object_function *object_function::to_object_function()
        {
            return this;
        }

        std::wstring object_function::to_string() const
        {
            return L"function";
        }

        /* 
            Prototype
        */
        
        /**
         * @brief Built-in method 'call()' for methods
         * 
         * The 'call()' method calls a Goat function with a given 'this' value
         * and arguments provided individually.
         */
        class object_function_call : public object_function_built_in
        {
        public:
            object_function_call(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                /*
                    Current stack:
                      [ this ]
                      [ ctx  ]
                      [ arg0 ]
                      [ arg1 ]
                      [ .... ]
                    Need to remove 'this' from the stack, and pass the rest
                    to the 'call' method.
                */
                if (arg_count > 0)
                {
                    if (mode != call_mode::as_method)
                    {
                        thr->raise_exception(new object_exception_illegal_context(thr->pool));
                        return;
                    }
                    object *this_ptr = thr->pop().get_object();
                    assert(this_ptr != nullptr);
                    object_function *this_ptr_func = this_ptr->to_object_function();
                    if (!this_ptr_func)
                    {
                        thr->raise_exception(new object_exception_illegal_context(thr->pool));
                        return;
                    }
                    this_ptr_func->call(thr, arg_count - 1, call_mode::as_method);
                    return;
                }
                thr->raise_exception(new object_exception_illegal_argument(thr->pool));
            }
        };

        /**
         * @brief Built-in method 'apply()' for methods
         * 
         * The 'apply()' method calls a function with a given this value,
         * and arguments provided as an array.
         */
        class object_function_apply : public object_function_built_in
        {
        public:
            object_function_apply(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                /*
                    Current stack:
                      [ this ]
                      [ ctx  ]
                      [ args ]
                      [ .... ]
                    Need to get this stack:
                      [ ctx  ]
                      [ arg0 ]
                      [ arg1 ]
                      [ .... ]
                */
                if (arg_count > 0)
                {
                    if (mode != call_mode::as_method)
                    {
                        thr->raise_exception(new object_exception_illegal_context(thr->pool));
                        return;
                    }
                    object *this_ptr = thr->pop().get_object();
                    assert(this_ptr != nullptr);
                    object_function *this_ptr_func = this_ptr->to_object_function();
                    if (!this_ptr_func)
                    {
                        thr->raise_exception(new object_exception_illegal_context(thr->pool));
                        return;
                    }

                    variable ctx = thr->pop();
                    object *args = thr->pop().get_object();
                    object_array *array_args = args ? args->to_object_array() : nullptr;
                    if (!array_args)
                    {
                        thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                        return;
                    }

                    int length = array_args->get_length();
                    for (int i = length - 1; i > -1; i--)
                    {
                        thr->push(array_args->get_item(i));
                    }
                    thr->push(ctx);

                    this_ptr_func->call(thr, length, call_mode::as_method);
                    return;
                }
                thr->raise_exception(new object_exception_illegal_argument(thr->pool));
            }
        };

        object_function_proto::object_function_proto(object_pool *pool)
            : object(pool)
        {
        }

        void object_function_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(L"call"), new object_function_call(pool));
            add_object(pool->get_static_string(L"apply"), new object_function_apply(pool));
            lock();
        }

        void object_function_proto::op_new(thread *thr, int arg_count)
        {
            thr->raise_exception(new object_exception_illegal_operation(thr->pool));
        }
    };
};
