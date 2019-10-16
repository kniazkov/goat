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

#include "object_string_builder.h"
#include "object_string.h"
#include "object_function_built_in.h"
#include "operator_wrapper.h"
#include "thread.h"
#include "lib/utils.h"
#include "lib/assert.h"
#include "lib/functional.h"
#include "resource/strings.h"

namespace g0at
{
    namespace model
    {
        object_string_builder::object_string_builder(object_pool *pool)
            : object(pool, pool->get_string_builder_proto_instance()), length(0)
        {
        }

        object_string_builder *object_string_builder::to_object_string_builder()
        {
            return this;
        }

        std::wstring object_string_builder::to_string() const
        {
            return data.str();
        }

        std::wstring object_string_builder::to_string_notation() const
        {
            std::wstringstream wss;
            wss << L'\"' << lib::escape_special_chars(data.str()) << L'\"';
            return wss.str();
        }

        void object_string_builder::op_shl(thread *thr)
        {
            thr->pop();
            variable right = thr->pop();
            append(right.to_string());
            variable result;
            result.set_object(this);
            thr->push(result);
        }

        /*
            Prototype
        */

        class object_string_builder_method : public object_function_built_in
        {
        public:
            object_string_builder_method(object_pool *_pool)
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
                object_string_builder *this_ptr_builder = this_ptr->to_object_string_builder();
                if (!this_ptr_builder)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                variable result = payload(thr, arg_count, this_ptr_builder);
                thr->pop(arg_count);
                thr->push(result);
            }

            virtual variable payload(thread *thr, int arg_count, object_string_builder *this_ptr) = 0;
        };


        class object_string_builder_length : public object_string_builder_method
        {
        public:
            object_string_builder_length(object_pool *_pool)
                : object_string_builder_method(_pool)
            {
            }
            
            variable payload(thread *thr, int arg_count, object_string_builder *this_ptr) override
            {
                variable result;
                result.set_integer(this_ptr->get_length());
                return result;
            }
        };

        class object_string_builder_data : public object_string_builder_method
        {
        public:
            object_string_builder_data(object_pool *_pool)
                : object_string_builder_method(_pool)
            {
            }
            
            variable payload(thread *thr, int arg_count, object_string_builder *this_ptr) override
            {
                variable result;
                result.set_object(thr->pool->create_object_string(this_ptr->get_data()));
                return result;
            }
        };

        class object_string_builder_add : public object_string_builder_method
        {
        public:
            object_string_builder_add(object_pool *_pool)
                : object_string_builder_method(_pool)
            {
            }
            
            variable payload(thread *thr, int arg_count, object_string_builder *this_ptr) override
            {
                for (int i = 0; i < arg_count; i++)
                {
                    std::wstring arg = thr->peek(i).to_string();
                    this_ptr->append(arg);
                }
                variable result;
                result.set_object(this_ptr);
                return result;
            }
        };

        object_string_builder_proto::object_string_builder_proto(object_pool *pool)
            : object(pool)
        {
        }

        void object_string_builder_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(resource::str_add), new object_string_builder_add(pool));
            add_object(pool->get_static_string(resource::str_length), new object_string_builder_length(pool));
            add_object(pool->get_static_string(resource::str_data), new object_string_builder_data(pool));
            add_object(pool->get_static_string(resource::str_oper_double_less), pool->get_wrap_shl_instance());
            lock();
        }

        void object_string_builder_proto::op_new(thread *thr, int arg_count)
        {
            thr->pop(arg_count);
            variable result;
            result.set_object(new object_string_builder(thr->pool));
            thr->push(result);
        }
    };
};
