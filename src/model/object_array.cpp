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
        class object_array_iterator : public object
        {
        public:
            object_array_iterator(object_pool *pool, object_array *_parent)
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
            object_array *parent;
            int index;
        };

        /*
            Array
        */
        object_array::object_array(object_pool *pool)
            : object(pool, pool->get_array_proto_instance())
        {
        }

#ifndef GC_DEBUG
        void object_array::kill(object_pool *pool)
        {
            if (pool->arrays.destroy_or_cache(this, pool))
            {
                objects.clear();
                vector.clear();
                immutable = false;
            }
        }
#endif

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

        void object_array::trace_parallel(object_pool *pool) 
        {
            for (variable &item : vector)
            {
                item.mark_parallel(pool);
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

        void object_array::op_add(thread *thr)
        {
            thr->pop();
            variable right = thr->pop();
            object *r_obj = right.get_object();
            variable var;
            object_array *result = thr->pool->create_object_array();
            var.set_object(result);
            int i, size;
            for (i = 0, size = (int)vector.size(); i < size; i++)
                result->vector.push_back(vector[i]);
            do
            {
                if (r_obj)
                {
                    object_array *r_array = r_obj->to_object_array();
                    if (r_array)
                    {
                        for (i = 0, size = (int)r_array->vector.size(); i < size; i++)
                            result->vector.push_back(r_array->vector[i]);
                        break;
                    }
                }
                result->vector.push_back(right);
            } while (false);
            thr->push(var);
        }

        void object_array::m_get(thread *thr, int arg_count)
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
                if (int_index >= 0 && int_index < (int64_t)vector.size())
                    thr->push(vector[(size_t)int_index]);
                else
                    thr->push_undefined();                
            }
            else
            {
                thr->pool->get_array_proto_instance()->m_get(thr, arg_count);
            }
        }

        void object_array::m_set(thread *thr, int arg_count)
        {
            if (arg_count < 1)
            {
                thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                return;
            }
            if (arg_count < 2)
            {
                thr->pop();
                variable value = thr->peek();
                vector.push_back(value);
            }
            else
            {
                variable index = thr->peek(1);
                int64_t int_index;
                if (index.get_integer(&int_index))
                {
                    thr->pop();
                    variable value = thr->peek(arg_count - 1).deref();
                    thr->pop(arg_count);
                    if (int_index >= 0 && int_index < INT32_MAX)
                    {
                        if (int_index < (int64_t)vector.size())
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
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                }
                else
                {
                    thr->pool->get_array_proto_instance()->m_set(thr, arg_count);
                }
            }
        }

        void object_array::m_clone(thread *thr, int arg_count)
        {
            thr->pop();
            thr->pop(arg_count);
            auto obj = thr->pool->create_object_array();
            for (auto item : vector)
            {
                obj->add_item(item);
            }
            variable result;
            result.set_object(obj);
            thr->push(result);
        }

        void object_array::m_iterator(thread *thr, int arg_count)
        {
            thr->pop();
            thr->pop(arg_count);
            variable tmp;
            tmp.set_object(new object_array_iterator(thr->pool, this));
            thr->push(tmp);
        }

        /*
            Prototype
        */

        class object_array_method : public object_function_built_in
        {
        public:
            object_array_method(object_pool *_pool)
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
                object_array *this_ptr_array = this_ptr->to_object_array();
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

            virtual bool payload(object_array *this_ptr, thread *thr, int arg_count, variable *result) = 0;
        };

        class object_array_length : public object_array_method
        {
        public:
            object_array_length(object_pool *_pool)
                : object_array_method(_pool)
            {
            }
            
            bool payload(object_array *this_ptr, thread *thr, int arg_count, variable *result) override
            {
                result->set_integer(this_ptr->get_length());
                return true;
            }
        };

        class object_array_push : public object_array_method
        {
        public:
            object_array_push(object_pool *_pool)
                : object_array_method(_pool)
            {
            }
            
            bool payload(object_array *this_ptr, thread *thr, int arg_count, variable *result) override
            {
                for (int i = 0; i < arg_count; i++)
                {
                    variable item = thr->peek(i).deref();
                    this_ptr->add_item(item);
                }
                result->set_object(thr->pool->get_undefined_instance());
                return true;
            }
        };

        class object_array_operator_plus : public object_function_built_in
        {
        public:
            object_array_operator_plus(object_pool *_pool)
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
                if (arg_count < 1)
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return;
                }
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);
                object_array *this_ptr_array = this_ptr->to_object_array();
                if (!this_ptr_array)
                {
                    thr->raise_exception(new object_exception_illegal_context(thr->pool));
                    return;
                }
                variable right = thr->pop();
                object *r_obj = right.get_object();
                variable var;
                object_array *result = thr->pool->create_object_array();
                var.set_object(result);
                int i, size;
                for (i = 0, size = this_ptr_array->get_length(); i < size; i++)
                    result->add_item(this_ptr_array->get_item(i));
                do
                {
                    if (r_obj)
                    {
                        object_array *r_array = r_obj->to_object_array();
                        if (r_array)
                        {
                            for (i = 0, size = r_array->get_length(); i < size; i++)
                                result->add_item(r_array->get_item(i));
                            break;
                        }
                    }
                    result->add_item(right);
                } while (false);
                thr->push(var);
            }
        };

        object_array_proto::object_array_proto(object_pool *pool)
            : object(pool)
        {
        }

        void object_array_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(resource::str_length), new object_array_length(pool));
            add_object(pool->get_static_string(resource::str_push), new object_array_push(pool));
            add_object(pool->get_static_string(resource::str_oper_plus), new object_array_operator_plus(pool));
            lock();
        }

        void object_array_proto::op_new(thread *thr, int arg_count)
        {
            auto obj = thr->pool->create_object_array();
            while(arg_count--)
            {
                variable arg = thr->pop();
                obj->add_item(arg);
            }
            variable result;
            result.set_object(obj);
            thr->push(result);
        }
    };
};

