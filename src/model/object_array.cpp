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

        void object_array::op_inherit(thread *thr)
        {
            thr->pop();
            object *right = thr->peek().to_object(thr->pool);
            right->proto = nullptr;
            right->topology = new topology_descriptor();
            int size = (int)vector.size();
            right->topology->proto.init(size);
            for (int i = 0; i < size; i++)
            {
                variable &var = vector[i];
                right->topology->proto[i] = var.to_object(thr->pool);
            }
            right->topology->build();
        }

        void object_array::m_get(thread *thr, int arg_count)
        {
            if (arg_count < 1)
            {
                thr->raise_exception(thr->pool->get_exception_illegal_argument_instance());
                return;
            }
            thr->pop().get_object();
            variable index = thr->peek();
            thr->pop(arg_count);
            int64_t int_index;
            if (index.get_integer(&int_index))
            {
                if (int_index >= 0 && int_index < vector.size())
                {
                    thr->push(vector[(size_t)int_index]);
                    return;
                }
            }
            thr->push_undefined();                
        }

        void object_array::m_set(thread *thr, int arg_count)
        {
            if (arg_count < 1)
            {
                thr->raise_exception(thr->pool->get_exception_illegal_argument_instance());
                return;
            }
            thr->pop().get_object();
            if (arg_count < 2)
            {
                variable value = thr->peek();
                vector.push_back(value);
            }
            else
            {
                variable index = thr->peek(0);
                variable value = thr->peek(arg_count - 1);
                thr->pop(arg_count);
                int64_t int_index;
                if (index.get_integer(&int_index))
                {
                    if (int_index >= 0 && int_index < INT32_MAX)
                    {
                        if (int_index < vector.size())
                        {
                            vector[(size_t)int_index] = value;
                        }
                        else
                        {
                            variable tmp;
                            tmp.set_object(thr->pool->get_undefined_instance());
                            for (int i = (int)vector.size(); i < int_index; i++)
                            {
                                vector.push_back(tmp);
                            }
                            vector.push_back(value);
                        }
                        thr->push(value);
                        return;
                    }
                }
                thr->raise_exception(thr->pool->get_exception_illegal_argument_instance());
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
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode != call_mode::as_method)
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
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if(arg_count > 0)
                {
                    if (mode != call_mode::as_method)
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
