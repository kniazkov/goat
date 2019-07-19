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

#include "object.h"
#include "object_pool.h"
#include "object_string.h"
#include "object_function_built_in.h"
#include "lib/assert.h"

namespace g0at
{
    namespace model
    {
        class generic_clone : public object_function_built_in
        {
        public:
            generic_clone(object_pool *_pool)
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
                thr->pop(arg_count);
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);

                object *clone = thr->pool->create_generic_object();
                this_ptr->copy_objects_to(clone);
                this_ptr->copy_proto_to(clone);

                variable tmp;
                tmp.set_object(clone);
                thr->push(tmp);
            }
        };

        class generic_instance_of : public object_function_built_in
        {
        public:
            generic_instance_of(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (arg_count < 1)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_argument_instance());
                    return;
                }
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);
                object *base = thr->peek().to_object(thr->pool);
                thr->pop(arg_count);
                variable tmp;
                tmp.set_boolean(this_ptr->instance_of(base));
                thr->push(tmp);
            }
        };

        class generic_flat : public object_function_built_in
        {
        public:
            generic_flat(object_pool *_pool)
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
                thr->pop(arg_count);
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);

                object *flat = thr->pool->create_generic_object();
                this_ptr->flat(flat);

                variable tmp;
                tmp.set_object(flat);
                thr->push(tmp);
            }
        };

        class generic_getter : public object_function_built_in
        {
        public:
            generic_getter(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (arg_count < 1)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_argument_instance());
                    return;
                }
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);
                object *key = thr->peek().to_object(thr->pool);
                thr->pop(arg_count);
                variable *found = this_ptr->find_object(key);
                if (found)
                    thr->push(*found);
                else
                    thr->push_undefined();                
            }
        };

        class generic_setter : public object_function_built_in
        {
        public:
            generic_setter(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (arg_count < 2)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_argument_instance());
                    return;
                }
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);
                object *key = thr->peek(0).to_object(thr->pool);
                variable value = thr->peek(arg_count - 1).deref();
                thr->pop(arg_count);
                this_ptr->add_object(key, value);
                thr->push(value);
            }
        };

        class generic_iterator_object : public object
        {
        public:
            generic_iterator_object(object_pool *pool, object *parent)
                : object(pool, pool->get_iterator_proto_instance())
            {
                object *tmp = pool->create_generic_object();
                parent->flat(tmp);
                keys = tmp->get_keys();
                index = -1;
            }

            void m_valid(thread *thr, int arg_count) override
            {
                thr->pop();
                thr->pop(arg_count);
                variable tmp;
                tmp.set_boolean(index + 1 < (int)keys.size());
                thr->push(tmp);
            }

            void m_next(thread *thr, int arg_count) override
            {
                thr->pop();
                thr->pop(arg_count);
                index++;
                if (index < (int)keys.size())
                {
                    variable tmp;
                    tmp.set_object(keys[index]);
                    thr->push(tmp);
                }
                else
                    thr->push_undefined();
            }

        protected:
            std::vector<object *> keys;
            int index;
        };

        class generic_iterator : public object_function_built_in
        {
        public:
            generic_iterator(object_pool *_pool)
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
                thr->pop(arg_count);
                variable iter;
                iter.set_object(new generic_iterator_object(thr->pool, this_ptr));
                thr->push(iter);
            }
        };

        class generic_contains : public object_function_built_in
        {
        public:
            generic_contains(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (arg_count < 1)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_argument_instance());
                    return;
                }
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);
                object *key = thr->peek().to_object(thr->pool);
                thr->pop(arg_count);
                variable *found = this_ptr->find_object(key);
                variable result;
                result.set_boolean(found != nullptr);
                thr->push(result);
            }
        };

        generic_proto::generic_proto(object_pool *pool)
            : object(pool, nullptr)
        {
        }

        void generic_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(L"clone"), new generic_clone(pool));
            add_object(pool->get_static_string(L"instanceof"), new generic_instance_of(pool));
            add_object(pool->get_static_string(L"flat"), new generic_flat(pool));
            add_object(pool->get_static_string(L"get"), new generic_getter(pool));
            add_object(pool->get_static_string(L"set"), new generic_setter(pool));
            add_object(pool->get_static_string(L"iterator"), new generic_iterator(pool));
            add_object(pool->get_static_string(L"contains"), new generic_contains(pool));
        }
    };
};