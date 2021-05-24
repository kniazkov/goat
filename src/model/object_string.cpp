/*

Copyright (C) 2017-2021 Ivan Kniazkov

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
#include "object_array.h"
#include "object_byte_array.h"
#include "operator_wrapper.h"
#include "object_exception.h"
#include "thread.h"
#include "lib/utils.h"
#include "lib/assert.h"
#include "lib/functional.h"
#include "lib/utf8_encoder.h"
#include "resource/strings.h"
#include <sstream>
#include <algorithm>
#include <cctype>

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
        object_string::object_string(object_pool *_pool, std::wstring _data)
            : object(_pool, _pool->get_string_proto_instance()), pool(_pool), data(_data), ids(-1)
        {
        }

        object_string::object_string(object_pool *_pool, std::wstring _data, int _ids)
            : object(_pool, _pool->get_string_proto_instance()), pool(_pool), data(_data), ids(_ids)
        {
        }

#ifndef GC_DEBUG
        void object_string::kill(object_pool *pool)
        {
            if (pool->strings.destroy_or_cache(this, pool))
            {
                objects.clear();
                data.clear();
                immutable = false;
            }
        }
#endif
        
        void object_string::reinit(std::wstring _data)
        {
            data = _data;
            ids = -1;
        }

        void object_string::reinit(std::wstring _data, int _ids)
        {
            data = _data;
            ids = _ids;
        }

        object_type object_string::get_type() const
        {
            return object_type::string;
        }

        object_string *object_string::to_object_string()
        {
            return this;
        }

        goat_value * object_string::get_value(const goat_allocator *allocator)
        {
#if 0
            return create_goat_string_ext(allocator, data.c_str(), data.length());
#else
            goat_string *result = (goat_string*)goat_alloc(allocator, sizeof(goat_string));
            result->base.type = goat_type_string;
            result->value = data.c_str();
            result->value_length = data.length();
            return (goat_value*)result;
#endif
        }

        bool object_string::less(const object *obj) const
        {
            assert(obj->get_type() == object_type::string);
            const object_string *other = static_cast<const object_string*>(obj);

            if (ids >= 0 && other->ids >= 0)
            {
                // first, compare indexes if possible
                return ids < other->ids;
            }

            // bad case: find indexes
            int my_ids = ids;
            if (my_ids < 0)
            {
                object_string *synonym = pool->get_static_string(data);
                my_ids = synonym->ids;
                assert(my_ids >= 0);
            }

            int other_ids = other->ids;
            if (other_ids < 0)
            {
                object_string *synonym = pool->get_static_string(other->data);
                other_ids = synonym->ids;
                assert(other_ids >= 0);
            }

            return my_ids < other_ids;
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

        void object_string::op_bool(thread *thr)
        {
            thr->pop();
            variable result;
            result.set_boolean(data != L"");
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
            binary_logical_operation<lib::func::less, false>(thr);
        }

        void object_string::op_leq(thread *thr)
        {
            binary_logical_operation<lib::func::less_or_equal, false>(thr);
        }

        void object_string::op_great(thread *thr)
        {
            binary_logical_operation<lib::func::greater, false>(thr);
        }

        void object_string::op_greq(thread *thr)
        {
            binary_logical_operation<lib::func::greater_or_equal, false>(thr);
        }

        void object_string::op_and(thread *thr)
        {
            thr->pop();
            if (data == L"")
            {
                thr->pop();
                variable result;
                result.set_object(this);
                thr->push(result);
            }
        }

        void object_string::op_or(thread *thr)
        {
            thr->pop();
            if (data != L"")
            {
                thr->pop();
                variable result;
                result.set_object(this);
                thr->push(result);
            }
        }

        void object_string::m_get(thread *thr, int arg_count)
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
                if (int_index >= 0 && int_index < (int64_t)data.size())
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
                thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                return;
            }
            variable index = thr->peek(1);
            int64_t int_index;
            if (index.get_integer(&int_index))
            {
                // Strings are immutable
                thr->raise_exception(new object_exception_illegal_operation(thr->pool));
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

        class object_string_method : public object_function_built_in
        {
        public:
            object_string_method(object_pool *_pool)
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
                object_string *this_ptr_string = this_ptr->to_object_string();
                if (!this_ptr_string)
                {
                    thr->raise_exception(new object_exception_illegal_context(thr->pool));
                    return;
                }
                variable result;
                if(payload(this_ptr_string, thr, arg_count, &result))
                {
                    thr->pop(arg_count);
                    thr->push(result);
                }
            }

            virtual bool payload(object_string *this_ptr, thread *thr, int arg_count, variable *result) = 0;
        };

        class object_string_length : public object_string_method
        {
        public:
            object_string_length(object_pool *_pool)
                : object_string_method(_pool)
            {
            }

            bool payload(object_string *this_ptr, thread *thr, int arg_count, variable *result) override
            {
                result->set_integer((int64_t)this_ptr->get_data().size());
                return true;
            }
        };

        class object_string_trim : public object_string_method
        {
        public:
            object_string_trim(object_pool *_pool)
                : object_string_method(_pool)
            {
            }

            bool payload(object_string *this_ptr, thread *thr, int arg_count, variable *result) override
            {
                result->set_object(thr->pool->create_object_string(lib::trim(this_ptr->get_data())));
                return true;
            }
        };

        class object_string_encode : public object_string_method
        {
        public:
            object_string_encode(object_pool *_pool)
                : object_string_method(_pool)
            {
            }

            bool payload(object_string *this_ptr, thread *thr, int arg_count, variable *result) override
            {
                if (arg_count > 0)
                {
                    variable arg = thr->peek();
                    object *obj = arg.get_object();
                    if (obj)
                    {
                        object_string *str = obj->to_object_string();
                        if (str)
                        {
                            std::wstring encoding = str->get_data();
                            std::transform(encoding.begin(), encoding.end(), encoding.begin(),
                                [](wchar_t c){ return std::tolower(c); });
                            
                            std::wstring data = this_ptr->get_data();

                            if (encoding == L"utf8")
                            {
                                std::string encoded_data = lib::encode_utf8(data);
                                result->set_object(new object_byte_array(thr->pool, encoded_data));
                                return true;
                            }

                            result->set_object(thr->pool->get_undefined_instance());
                            return true;
                        }
                    }
                }
                thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                return false;
            }
        };

        class object_string_split : public object_string_method
        {
        public:
            object_string_split(object_pool *_pool)
                : object_string_method(_pool)
            {
            }

            bool payload(object_string *this_ptr, thread *thr, int arg_count, variable *result) override
            {
                if (arg_count < 1)
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return false;
                }

                std::wstring value = this_ptr->get_data();
                object_array *list = thr->pool->create_object_array();
                result->set_object(list);

                if (value == L"")
                    return true;

                variable arg = thr->peek();
                wchar_t char_arg;
                if (arg.get_char(&char_arg))
                {
                    size_t i = 0,
                        j;
                    variable item;
                    while(std::string::npos != (j = value.find(char_arg, i)))
                    {
                        item.set_object(thr->pool->create_object_string(value.substr(i, j - i)));
                        list->add_item(item);
                        i = j + 1;
                    }
                    item.set_object(thr->pool->create_object_string(value.substr(i)));
                    list->add_item(item);
                    return true;
                }

                object *obj_arg = arg.get_object();
                if (obj_arg)
                {
                    object_string *obj_string_arg = obj_arg->to_object_string();
                    if (obj_string_arg)
                    {
                        std::wstring string_arg = obj_string_arg->get_data();
                        size_t arg_len = string_arg.size();
                        if (arg_len > 0)
                        {
                            size_t i = 0,
                                j;
                            variable item;
                            while(std::string::npos != (j = value.find(string_arg, i)))
                            {
                                item.set_object(thr->pool->create_object_string(value.substr(i, j - i)));
                                list->add_item(item);
                                i = j + arg_len;
                            }
                            item.set_object(thr->pool->create_object_string(value.substr(i)));
                            list->add_item(item);
                            return true;
                        }                        
                    }
                }

                thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                return false;
            }
        };

        class object_string_find : public object_string_method
        {
        public:
            object_string_find(object_pool *_pool)
                : object_string_method(_pool)
            {
            }

            bool payload(object_string *this_ptr, thread *thr, int arg_count, variable *result) override
            {
                if (arg_count < 1)
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return false;
                }

                std::wstring value = this_ptr->get_data();

                if (value == L"") {
                    result->set_integer(-1);
                    return true;
                }

                variable arg = thr->peek();
                wchar_t char_arg;
                if (arg.get_char(&char_arg))
                {
                    size_t i = value.find(char_arg);
                    result->set_integer(i != std::string::npos ? (int64_t)i : -1);
                    return true;
                }

                object *obj_arg = arg.get_object();
                if (obj_arg)
                {
                    object_string *obj_string_arg = obj_arg->to_object_string();
                    if (obj_string_arg)
                    {
                        std::wstring string_arg = obj_string_arg->get_data();
                        size_t arg_len = string_arg.size();
                        if (arg_len > 0)
                        {
                            size_t i = value.find(string_arg);
                            result->set_integer(i != std::string::npos ? (int64_t)i : -1);
                            return true;
                        }
                    }
                }

                thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                return false;
            }
        };

        class object_string_substr : public object_string_method
        {
        public:
            object_string_substr(object_pool *_pool)
                : object_string_method(_pool)
            {
            }

            bool payload(object_string *this_ptr, thread *thr, int arg_count, variable *result) override
            {
                if (arg_count < 1)
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return false;
                }

                int64_t begin;
                if (!thr->peek(0).get_integer(&begin) || begin > INT32_MAX)
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return false;
                }
                std::wstring value = this_ptr->get_data();
                int64_t size = (int64_t)value.size();
                if (begin >= size) begin = size;
                else if (begin < 0) begin = 0;

                int64_t count;
                if (arg_count > 1)
                {
                    if (!thr->peek(1).get_integer(&count) || count > INT32_MAX)
                    {
                        thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                        return false;
                    }
                    if (count > size - begin) count = size - begin;
                    else if (count < 0) count = 0;                    
                }
                else
                {
                    count = size - begin;
                }
                
                if (count == 0)
                    result->set_object(thr->pool->create_object_string(resource::empty_string));
                else
                    result->set_object(thr->pool->create_object_string(value.substr((size_t)begin, (size_t)count)));

                return true;
            }
        };

        class object_string_valueof : public object_function_built_in
        {
        public:
            object_string_valueof(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (arg_count > 0)
                {
                    if (mode == call_mode::as_method)
                        thr->pop();
                    variable arg = thr->peek();
                    thr->pop(arg_count);
                    variable result;
                    result.set_object(thr->pool->create_object_string(arg.to_string()));
                    thr->push(result);
                    return;
                }
                thr->raise_exception(new object_exception_illegal_argument(thr->pool));
            }
        };

        object_string_proto::object_string_proto(object_pool *pool)
            : object(pool)
        {
        }

        void object_string_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(resource::str_length), new object_string_length(pool));
            add_object(pool->get_static_string(resource::str_trim), new object_string_trim(pool));
            add_object(pool->get_static_string(resource::str_encode), new object_string_encode(pool));
            add_object(pool->get_static_string(resource::str_split), new object_string_split(pool));
            add_object(pool->get_static_string(resource::str_find), new object_string_find(pool));
            add_object(pool->get_static_string(resource::str_substr), new object_string_substr(pool));
            add_object(pool->get_static_string(resource::str_valueof), new object_string_valueof(pool));
            add_object(pool->get_static_string(resource::str_oper_plus), pool->get_wrap_add_instance());
            add_object(pool->get_static_string(resource::str_oper_exclamation), pool->get_wrap_not_instance());
            add_object(pool->get_static_string(resource::str_oper_double_exclamation), pool->get_wrap_bool_instance());
            add_object(pool->get_static_string(resource::str_oper_less), pool->get_wrap_less_instance());
            add_object(pool->get_static_string(resource::str_oper_less_equal), pool->get_wrap_leq_instance());
            add_object(pool->get_static_string(resource::str_oper_greater), pool->get_wrap_great_instance());
            add_object(pool->get_static_string(resource::str_oper_greater_equal), pool->get_wrap_greq_instance());
            add_object(pool->get_static_string(resource::str_oper_double_ampersand), pool->get_wrap_and_instance());
            add_object(pool->get_static_string(resource::str_oper_double_vertical_bar), pool->get_wrap_or_instance());
            lock();
        }

        void object_string_proto::op_new(thread *thr, int arg_count)
        {
            variable result;
            if (arg_count > 0)
            {
                variable arg = thr->peek();
                thr->pop(arg_count);
                result.set_object(thr->pool->create_object_string(arg.to_string()));
            }
            else
            {
                result.set_object(thr->pool->create_object_string(resource::empty_string));
            }
            thr->push(result);
        }
    };
};
