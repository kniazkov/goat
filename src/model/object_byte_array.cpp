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

#include "object_byte_array.h"
#include "object_string.h"
#include "object_function_built_in.h"
#include "object_exception.h"
#include "thread.h"
#include "lib/utils.h"
#include "lib/assert.h"
#include "lib/utf8_encoder.h"
#include "resource/strings.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace g0at
{
    namespace model
    {
        /*
            Iterator
        */
        class object_byte_array_iterator : public object
        {
        public:
            object_byte_array_iterator(object_pool *pool, object_byte_array *_parent)
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
                {
                    variable result;
                    result.set_integer(parent->get_byte_at(index));
                    thr->push(result);
                }
                else
                    thr->push_undefined();
            }

        protected:
            object_byte_array *parent;
            int index;
        };

        /*
            ByteArray
        */

        object_byte_array::object_byte_array(object_pool *pool)
            : object(pool, pool->get_byte_array_proto_instance())
        {
        }

        object_byte_array::object_byte_array(object_pool *pool, std::string data)
            : object(pool, pool->get_byte_array_proto_instance())
        {
            auto size = data.size();
            if (size > 0)
            {
                vector.reserve(size);
                vector.insert(vector.end(), data.begin(), data.end());
            }
        }

        object_byte_array::object_byte_array(object_pool *pool, const uint8_t *data, size_t size)
            : object(pool, pool->get_byte_array_proto_instance())
        {
            vector.reserve(size);
            vector.insert(vector.end(), data, data + size);
        }

        object_byte_array *object_byte_array::to_object_byte_array()
        {
            return this;
        }

        std::wstring object_byte_array::to_string() const
        {
            std::wstringstream wss;
            for (int i = 0, size = (int)vector.size(); i < size; i++)
            {
                if (i)
                    wss << L' ';
                uint8_t byte = vector[i];
                uint8_t hi = byte >> 4;
                uint8_t low = byte & 15;
                wss << (wchar_t)(hi > 9 ? L'a' + hi - 10 : L'0' + hi) << (wchar_t)(low > 9 ? L'a' + low - 10 : L'0' + low);
            }
            return wss.str();
        }

        goat_value * object_byte_array::get_value(const goat_allocator *allocator)
        {
            return create_goat_byte_array(allocator, false, &vector[0], vector.size());
        }

        void object_byte_array::m_get(thread *thr, int arg_count)
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
                {
                    variable result;
                    result.set_integer((int)vector[(size_t)int_index]);
                    thr->push(result);
                }
                else
                    thr->push_undefined();                
            }
            else
            {
                thr->pool->get_array_proto_instance()->m_get(thr, arg_count);
            }
        }

        void object_byte_array::m_set(thread *thr, int arg_count)
        {
            if (arg_count < 1)
            {
                thr->raise_exception(new object_exception_illegal_argument(thr->pool));
            }
            else if (arg_count < 2)
            {
                thr->pop();
                variable value = thr->pop();
                uint8_t byte;
                if (!value.get_byte(&byte))
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return;
                }
                vector.push_back(byte);
                thr->push(value);
            }
            else
            {
                variable index = thr->peek(1);
                int64_t int_index;
                if (index.get_integer(&int_index))
                {
                    thr->pop();
                    variable value = thr->peek(arg_count - 1);
                    uint8_t byte;
                    if (!value.get_byte(&byte))
                    {
                        thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                        return;
                    }
                    thr->pop(arg_count);
                    if (int_index >= 0 && int_index < INT32_MAX)
                    {
                        if (int_index < (int64_t)vector.size())
                        {
                            vector[(size_t)int_index] = byte;
                        }
                        else
                        {
                            for (int i = (int)vector.size(); i < int_index; i++)
                            {
                                vector.push_back(0);
                            }
                            vector.push_back(byte);
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

        void object_byte_array::m_iterator(thread *thr, int arg_count)
        {
            thr->pop();
            thr->pop(arg_count);
            variable tmp;
            tmp.set_object(new object_byte_array_iterator(thr->pool, this));
            thr->push(tmp);
        }

        void object_byte_array::fill(uint8_t byte, int count)
        {
            if (count > 0)
            {
                vector.reserve(vector.size() + count);
                while(count)
                {
                    vector.push_back(byte);
                    count--;
                }
            }
        }

        /*
            Prototype
        */

        class object_byte_array_method : public object_function_built_in
        {
        public:
            object_byte_array_method(object_pool *_pool)
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
                object_byte_array *this_ptr_array = this_ptr->to_object_byte_array();
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

            virtual bool payload(object_byte_array *this_ptr, thread *thr, int arg_count, variable *result) = 0;
        };

        class object_byte_array_length : public object_byte_array_method
        {
        public:
            object_byte_array_length(object_pool *_pool)
                : object_byte_array_method(_pool)
            {
            }

            bool payload(object_byte_array *this_ptr, thread *thr, int arg_count, variable *result) override
            {
                result->set_integer(this_ptr->get_length());
                return true;
            }
        };

        class object_byte_array_push : public object_byte_array_method
        {
        public:
            object_byte_array_push(object_pool *_pool)
                : object_byte_array_method(_pool)
            {
            }
            
            bool payload(object_byte_array *this_ptr, thread *thr, int arg_count, variable *result) override
            {
                for (int i = 0; i < arg_count; i++)
                {
                    variable value = thr->peek(i);
                    uint8_t byte;
                    if (!value.get_byte(&byte))
                    {
                        thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                        return false;
                    }
                    this_ptr->add_byte(byte);
                }
                result->set_object(thr->pool->get_undefined_instance());
                return true;
            }
        };

        class object_byte_array_decode_utf8 : public object_byte_array_method
        {
        public:
            object_byte_array_decode_utf8(object_pool *_pool)
                : object_byte_array_method(_pool)
            {
            }

            bool payload(object_byte_array *this_ptr, thread *thr, int arg_count, variable *result) override
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
                            
                            if (encoding == L"utf8")
                            {
                                std::string data((char*)this_ptr->get_data(), this_ptr->get_length());
                                std::wstring decoded_data = lib::decode_utf8(data);
                                result->set_object(thr->pool->create_object_string(decoded_data));
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

        class object_byte_array_fill : public object_byte_array_method
        {
        public:
            object_byte_array_fill(object_pool *_pool)
                : object_byte_array_method(_pool)
            {
            }
            
            bool payload(object_byte_array *this_ptr, thread *thr, int arg_count, variable *result) override
            {
                uint8_t byte;
                int64_t count;
                if (arg_count < 2 ||
                    !thr->peek(0).get_byte(&byte) || !thr->peek(1).get_integer(&count) || count > INT32_MAX || count < 0)
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return false;
                }
                this_ptr->fill(byte, (int)count);
                result->set_object(thr->pool->get_undefined_instance());
                return true;
            }
        };

        object_byte_array_proto::object_byte_array_proto(object_pool *pool)
            : object(pool)
        {
        }

        void object_byte_array_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(resource::str_length), new object_byte_array_length(pool));
            add_object(pool->get_static_string(resource::str_push), new object_byte_array_push(pool));
            add_object(pool->get_static_string(resource::str_decode), new object_byte_array_decode_utf8(pool));
            add_object(pool->get_static_string(resource::str_fill), new object_byte_array_fill(pool));
            lock();
        }

        void object_byte_array_proto::op_new(thread *thr, int arg_count)
        {
            auto obj = new object_byte_array(thr->pool);
            while(arg_count--)
            {
                variable arg = thr->pop();
                uint8_t byte;
                if (!arg.get_byte(&byte))
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return;
                }
                obj->add_byte(byte);
            }
            variable result;
            result.set_object(obj);
            thr->push(result);
        }
    };
};
