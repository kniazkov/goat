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

#include "object_array.h"
#include "object_string.h"
#include "object_function_built_in.h"
#include "thread.h"
#include "lib/utils.h"
#include "lib/assert.h"
#include <sstream>

namespace g0at
{
    namespace model
    {
        object_array::object_array(object_pool *pool)
            : object(pool, pool->get_array_proto_instance())
        {
        }

        void object_array::kill(object_pool *pool)
        {
            if (pool->arrays.destroy_or_cache(this, pool))
            {
                objects.clear();
                vector.clear();
            }
        }
        
        void object_array::reinit()
        {
            assert(vector.empty());
        }

        object_array *object_array::to_object_array()
        {
            return this;
        }

        std::wstring object_array::to_string() const
        {
            std::wstringstream wss;
            wss << L'[';
            for (int i = 0, size = (int)vector.size(); i < size; i++)
            {
                if (i)
                    wss << L',';
                wss << vector[i].to_string_notation();
            }
            wss << L']';
            return wss.str();
        }

        void object_array::trace() 
        {
            for (variable &item : vector)
            {
                item.mark();
            }
        }

        /*
            Prototype
        */

        class object_array_length : public object_function_built_in
        {
        public:
            object_array_length(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, bool as_method) override
            {
                if (!as_method)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);
                object_array *this_ptr_array = this_ptr->to_object_array();
                if (!this_ptr_array)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                thr->pop(arg_count);
                variable tmp;
                tmp.set_integer(this_ptr_array->get_length());
                thr->push(tmp);
            }
        };

        class object_array_push : public object_function_built_in
        {
        public:
            object_array_push(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, bool as_method) override
            {
                if(arg_count > 0)
                {
                    if (!as_method)
                    {
                        thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                        return;
                    }
                    object *this_ptr = thr->pop().get_object();
                    assert(this_ptr != nullptr);
                    object_array *this_ptr_array = this_ptr->to_object_array();
                    if (!this_ptr_array)
                    {
                        thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                        return;
                    }
                    while (arg_count--)
                    {
                        variable item = thr->pop();
                        this_ptr_array->add_item(item);
                    }
                    thr->push_undefined();
                    return;
                }
                thr->raise_exception(thr->pool->get_exception_illegal_argument_instance());
            }
        };

        object_array_proto::object_array_proto(object_pool *pool)
            : object(pool)
        {
        }

        void object_array_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(L"length"), new object_array_length(pool));
            add_object(pool->get_static_string(L"push"), new object_array_push(pool));
        }
    };
};
