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

#include "object_array_like.h"
#include "object_string.h"
#include "object_function_built_in.h"
#include "object_exception.h"
#include "thread.h"
#include "lib/utils.h"
#include "lib/assert.h"
#include "resource/strings.h"
#include <sstream>

namespace g0at
{
    namespace model
    {
        /*
            Iterator
        */
        class object_array_like_iterator : public object
        {
        public:
            object_array_like_iterator(object_pool *pool, object_array_like *_parent)
                : object(pool, pool->get_iterator_proto_instance()), parent(_parent), index(-1)
            {
            }

            void trace() override
            {
                parent->mark();
            }

            void m_valid(thread *thr, int arg_count) override
            {
                thr->pop();
                thr->pop(arg_count);
                variable tmp;
                tmp.set_boolean(index + 1 < parent->get_length());
                thr->push(tmp);
            }

            void m_next(thread *thr, int arg_count) override
            {
                thr->pop();
                thr->pop(arg_count);
                index++;
                if (index < parent->get_length())
                    thr->push(parent->get_item(index));
                else
                    thr->push_undefined();
            }

        protected:
            object_array_like *parent;
            int index;
        };

        /*
            Array
        */
        object_array_like::object_array_like(object_pool *pool, object *proto)
            : object(pool, proto)
        {
        }

        object_array_like *object_array_like::to_object_array_like()
        {
            return this;
        }

        std::wstring object_array_like::to_string() const
        {
            std::wstringstream wss;
            wss << L'[';
            for (int i = 0, size = get_length(); i < size; i++)
            {
                if (i)
                    wss << L',';
                wss << get_item(i).to_string_notation();
            }
            wss << L']';
            return wss.str();
        }

        void object_array_like::m_get(thread *thr, int arg_count)
        {
            if (arg_count < 1)
            {
                thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                return;
            }
            variable index = thr->peek(1);
            int64_t int_index;
            if (index.get_integer(&int_index))
            {
                thr->pop();
                thr->pop(arg_count);
                if (int_index >= 0 && int_index < get_length())
                    thr->push(get_item((int)int_index));
                else
                    thr->push_undefined();                
            }
            else
            {
                thr->pool->get_array_like_proto_instance()->m_get(thr, arg_count);
            }
        }

        void object_array_like::m_set(thread *thr, int arg_count)
        {
            if (arg_count < 1)
            {
                thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                return;
            }
            variable index = thr->peek(1);
            int64_t int_index;
            if (index.get_integer(&int_index))
            {
                // Array-like objects are immutable
                thr->raise_exception(new object_exception_illegal_operation(thr->pool));
            }
            else
            {
                thr->pool->get_string_proto_instance()->m_set(thr, arg_count);
            }
        }

        void object_array_like::m_iterator(thread *thr, int arg_count)
        {
            thr->pop();
            thr->pop(arg_count);
            variable tmp;
            tmp.set_object(new object_array_like_iterator(thr->pool, this));
            thr->push(tmp);
        }

        /*
            Prototype
        */

        class object_array_like_method : public object_function_built_in
        {
        public:
            object_array_like_method(object_pool *_pool)
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
                object_array_like *this_ptr_array = this_ptr->to_object_array_like();
                if (!this_ptr_array)
                {
                    thr->raise_exception(new object_exception_illegal_context(thr->pool));
                    return;
                }
                variable result;
                if(payload(this_ptr_array, thr, arg_count, &result))
                {
                    thr->pop(arg_count);
                    thr->push(result);
                }
            }

            virtual bool payload(object_array_like *this_ptr, thread *thr, int arg_count, variable *result) = 0;
        };

        class object_array_length : public object_array_like_method
        {
        public:
            object_array_length(object_pool *_pool)
                : object_array_like_method(_pool)
            {
            }
            
            bool payload(object_array_like *this_ptr, thread *thr, int arg_count, variable *result) override
            {
                result->set_integer(this_ptr->get_length());
                return true;
            }
        };

        object_array_like_proto::object_array_like_proto(object_pool *pool)
            : object(pool)
        {
        }

        void object_array_like_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(resource::str_length), new object_array_length(pool));
            lock();
        }

        void object_array_like_proto::op_new(thread *thr, int arg_count)
        {
            thr->raise_exception(new object_exception_illegal_operation(thr->pool));
        }
    };
};

