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

#include "object_string.h"
#include "object_function_built_in.h"
#include "operator_wrapper.h"
#include "thread.h"
#include "lib/utils.h"
#include "lib/assert.h"
#include "lib/functional.h"
#include "resource/strings.h"
#include <sstream>

namespace g0at
{
    namespace model
    {
        /*
            Iterator
        */
        class object_string_iterator : public object
        {
        public:
            object_string_iterator(object_pool *pool, std::wstring _data)
                : object(pool, pool->get_iterator_proto_instance()), data(_data), index(-1)
            {
            }

            void m_valid(thread *thr, int arg_count) override
            {
                thr->pop();
                thr->pop(arg_count);
                variable tmp;
                tmp.set_boolean(index + 1 < (int)data.size());
                thr->push(tmp);
            }

            void m_next(thread *thr, int arg_count) override
            {
                thr->pop();
                thr->pop(arg_count);
                index++;
                if (index < (int)data.size())
                {
                    variable tmp;
                    tmp.set_char(data[index]);
                    thr->push(tmp);
                }
                else
                    thr->push_undefined();
            }

        protected:
            std::wstring data;
            int index;
        };

        /*
            String
        */
        object_string::object_string(object_pool *pool, std::wstring _data)
            : object(pool, pool->get_string_proto_instance()), data(_data), id(-1)
        {
        }

        object_string::object_string(object_pool *pool, std::wstring _data, int _id)
            : object(pool, pool->get_string_proto_instance()), data(_data), id(_id)
        {
        }

        void object_string::kill(object_pool *pool)
        {
            if (pool->strings.destroy_or_cache(this, pool))
            {
                objects.clear();
                data.clear();
            }
        }
        
        void object_string::reinit(std::wstring _data)
        {
            data = _data;
            id = -1;
        }

        void object_string::reinit(std::wstring _data, int _id)
        {
            data = _data;
            id = _id;
        }


        object_type object_string::get_type() const
        {
            return object_type::string;
        }

        object_string *object_string::to_object_string()
        {
            return this;
        }

        bool object_string::less(const object *obj) const
        {
            assert(obj->get_type() == object_type::string);
            const object_string *str = static_cast<const object_string*>(obj);
            if (id < 0 || str->id < 0)
                return data < str->data;
            else
                return id < str->id;
        }

        std::wstring object_string::to_string() const
        {
            return data;
        }

        std::wstring object_string::to_string_notation() const
        {
            std::wstringstream wss;
            wss << L'\"' << lib::escape_special_chars(data) << L'\"';
            return wss.str();
        }

        void object_string::op_add(thread *thr)
        {
            thr->pop();
            variable right = thr->pop();
            std::wstring str = data + right.to_string();
            variable result;
            result.set_object(thr->pool->create_object_string(str));
            thr->push(result);
        }

        void object_string::op_not(thread *thr)
        {
            thr->pop();
            variable result;
            result.set_boolean(data == L"");
            thr->push(result);
        }

        void object_string::op_eq(thread *thr)
        {
            binary_logical_operation<lib::func::equals, false>(thr);
        }

        void object_string::op_neq(thread *thr)
        {
            binary_logical_operation<lib::func::not_equal, true>(thr);
        }

        void object_string::op_less(thread *thr)
        {
            binary_logical_operation<lib::func::less, true>(thr);
        }

        void object_string::m_get(thread *thr, int arg_count)
        {
            if (arg_count < 1)
            {
                thr->raise_exception(thr->pool->get_exception_illegal_argument_instance());
                return;
            }
            variable index = thr->peek(1);
            int64_t int_index;
            if (index.get_integer(&int_index))
            {
                thr->pop();
                thr->pop(arg_count);
                if (int_index >= 0 && int_index < data.size())
                {
                    variable tmp;
                    tmp.set_char(data[(size_t)int_index]);
                    thr->push(tmp);
                }
                else
                    thr->push_undefined();
            }
            else
            {
                thr->pool->get_string_proto_instance()->m_get(thr, arg_count);
            }
        }

        void object_string::m_set(thread *thr, int arg_count)
        {
            if (arg_count < 1)
            {
                thr->raise_exception(thr->pool->get_exception_illegal_argument_instance());
                return;
            }
            variable index = thr->peek(1);
            int64_t int_index;
            if (index.get_integer(&int_index))
            {
                // Strings are immutable
                thr->raise_exception(thr->pool->get_exception_illegal_operation_instance());
            }
            else
            {
                thr->pool->get_string_proto_instance()->m_set(thr, arg_count);
            }
        }

        void object_string::m_iterator(thread *thr, int arg_count)
        {
            thr->pop();
            thr->pop(arg_count);
            variable tmp;
            tmp.set_object(new object_string_iterator(thr->pool, data));
            thr->push(tmp);
        }

        template <template<typename R, typename X, typename Y> class F, bool Def> void object_string::binary_logical_operation(thread *thr)
        {
            thr->pop();
            variable right = thr->pop();
            object *r_obj = right.get_object();
            variable result;
            do
            {
                if (r_obj)
                {
                    object_string *r_string = r_obj->to_object_string();
                    if (r_string)
                    {
                        result.set_boolean(F<bool, std::wstring, std::wstring>::calculate(data, r_string->get_data()));
                        break;
                    }
                }
                result.set_boolean(Def);
            } while (false);
            thr->push(result);
        }

        /*
            Prototype
        */

        class object_string_length : public object_function_built_in
        {
        public:
            object_string_length(object_pool *_pool)
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
                object_string *this_ptr_string = this_ptr->to_object_string();
                if (!this_ptr_string)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                thr->pop(arg_count);
                variable result;
                result.set_integer(this_ptr_string->get_data().length());
                thr->push(result);
            }
        };

        class object_string_operator_plus : public object_function_built_in
        {
        public:
            object_string_operator_plus(object_pool *_pool)
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
                if (arg_count < 1)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_argument_instance());
                    return;
                }
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);
                object_string *this_ptr_string = this_ptr->to_object_string();
                if (!this_ptr_string)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                variable right = thr->pop();
                std::wstring str = this_ptr_string->get_data() + right.to_string();
                variable result;
                result.set_object(thr->pool->create_object_string(str));
                thr->push(result);
            }
        };

        class object_string_operator_not : public object_function_built_in
        {
        public:
            object_string_operator_not(object_pool *_pool)
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
                object_string *this_ptr_string = this_ptr->to_object_string();
                if (!this_ptr_string)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                thr->pop(arg_count);
                variable result;
                result.set_boolean(this_ptr_string->get_data() == L"");
                thr->push(result);
            }
        };

        object_string_proto::object_string_proto(object_pool *pool)
            : object(pool)
        {
        }

        void object_string_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(resource::str_length), new object_string_length(pool));
            //add_object(pool->get_static_string(resource::str_oper_plus), new object_string_operator_plus(pool));
            add_object(pool->get_static_string(resource::str_oper_plus), new binary_operator_adapter<wrap_add>(pool));
            add_object(pool->get_static_string(resource::str_oper_exclamation), new object_string_operator_not(pool));
        }
    };
};
