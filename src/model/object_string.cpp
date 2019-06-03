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
#include "thread.h"
#include "lib/utils.h"
#include "lib/assert.h"
#include <sstream>

namespace g0at
{
    namespace model
    {
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

        void object_string::op_less(thread *thr)
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
                        result.set_boolean(data < r_string->get_data());
                        break;
                    }
                }
                result.set_boolean(false);
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
                variable tmp;
                tmp.set_integer(this_ptr_string->get_data().length());
                thr->push(tmp);
            }
        };

        object_string_proto::object_string_proto(object_pool *pool)
            : object(pool)
        {
        }

        void object_string_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(L"length"), new object_string_length(pool));
        }
    };
};
