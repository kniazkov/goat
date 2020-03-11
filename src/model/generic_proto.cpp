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

#include "object.h"
#include "object_pool.h"
#include "object_string.h"
#include "object_function_built_in.h"
#include "object_exception.h"
#include "object_array.h"
#include "lib/assert.h"
#include "resource/strings.h"

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
                    thr->raise_exception(new object_exception_illegal_context(thr->pool));
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
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return;
                }
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(new object_exception_illegal_context(thr->pool));
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
                    thr->raise_exception(new object_exception_illegal_context(thr->pool));
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
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return;
                }
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(new object_exception_illegal_context(thr->pool));
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
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return;
                }
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(new object_exception_illegal_context(thr->pool));
                    return;
                }
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);
                object *key = thr->peek(0).to_object(thr->pool);
                variable value = thr->peek(arg_count - 1).deref();
                if (!this_ptr->add_object(key, value))
                {
                    thr->raise_exception(new object_exception_illegal_operation(thr->pool));
                    return;
                }
                thr->pop(arg_count);
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
                    thr->raise_exception(new object_exception_illegal_context(thr->pool));
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

        class generic_keys : public object_function_built_in
        {
        public:
            generic_keys(object_pool *_pool)
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
                bool deep_search = false;
                if (arg_count > 0)
                {
                    variable deep_search_arg = thr->peek();
                    if (!deep_search_arg.get_boolean(&deep_search))
                    {
                        thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    }
                }
                thr->pop(arg_count);
                variable result;
                std::vector<object *> keys;
                object_array *ret_array = thr->pool->create_object_array();
                result.set_object(ret_array);
                if (!deep_search)
                {
                    keys = this_ptr->get_keys();
                }
                else
                {
                    object *tmp = thr->pool->create_generic_object();
                    this_ptr->flat(tmp);
                    keys = tmp->get_keys();
                }
                for (object *key : keys)
                {
                    variable var;
                    var.set_object(key);
                    ret_array->add_item(var);
                }
                thr->push(result);
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
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return;
                }
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(new object_exception_illegal_context(thr->pool));
                    return;
                }
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);
                object *key = thr->peek(0).to_object(thr->pool);
                bool deep_search = false;
                if (arg_count > 1)
                {
                    variable deep_search_arg = thr->peek(1);
                    if (!deep_search_arg.get_boolean(&deep_search))
                    {
                        thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    }
                }
                thr->pop(arg_count);
                variable *found = deep_search ? this_ptr->find_object(key) : this_ptr->find_own_object(key);
                variable result;
                result.set_boolean(found != nullptr);
                thr->push(result);
            }
        };

        class generic_not : public object_function_built_in
        {
        public:
            generic_not(object_pool *_pool)
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
                thr->pop();
                thr->pop(arg_count);
                variable result;
                result.set_boolean(false);
                thr->push(result);
            }
        };

        class generic_bool : public object_function_built_in
        {
        public:
            generic_bool(object_pool *_pool)
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
                thr->pop();
                thr->pop(arg_count);
                variable result;
                result.set_boolean(true);
                thr->push(result);
            }
        };

        class generic_and : public object_function_built_in
        {
        public:
            generic_and(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }

            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (arg_count < 1)
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return;
                }
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(new object_exception_illegal_context(thr->pool));
                    return;
                }
                thr->pop();
                variable arg = thr->peek();
                thr->pop(arg_count);
                thr->push(arg);
            }
        };

        class generic_or : public object_function_built_in
        {
        public:
            generic_or(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }

            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (arg_count < 1)
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return;
                }
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(new object_exception_illegal_context(thr->pool));
                    return;
                }
                variable this_ptr = thr->pop();
                thr->pop(arg_count);
                thr->push(this_ptr);
            }
        };

        class generic_protect : public object_function_built_in
        {
        public:
            generic_protect(object_pool *_pool)
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
                thr->pop(arg_count);
                variable result;
                this_ptr->lock();
                result.set_object(this_ptr);
                thr->push(result);
            }
        };

        generic_proto::generic_proto(object_pool *pool)
            : object(pool, nullptr)
        {
        }

        void generic_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(resource::str_clone), new generic_clone(pool));
            add_object(pool->get_static_string(resource::str_instanceof), new generic_instance_of(pool));
            add_object(pool->get_static_string(resource::str_flat), new generic_flat(pool));
            add_object(pool->get_static_string(resource::str_get), new generic_getter(pool));
            add_object(pool->get_static_string(resource::str_set), new generic_setter(pool));
            add_object(pool->get_static_string(resource::str_iterator), new generic_iterator(pool));
            add_object(pool->get_static_string(resource::str_keys), new generic_keys(pool));
            add_object(pool->get_static_string(resource::str_contains), new generic_contains(pool));
            add_object(pool->get_static_string(resource::str_oper_exclamation), new generic_not(pool));
            add_object(pool->get_static_string(resource::str_oper_double_exclamation), new generic_bool(pool));
            add_object(pool->get_static_string(resource::str_oper_double_ampersand), new generic_and(pool));
            add_object(pool->get_static_string(resource::str_oper_double_vertical_bar), new generic_or(pool));
            add_object(pool->get_static_string(resource::str_oper_protect), new generic_protect(pool));
            lock();
        }
    };
};
