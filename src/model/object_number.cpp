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

#include "object_number.h"
#include "object_string.h"
#include "object_function_built_in.h"
#include "object_exception.h"
#include "thread.h"
#include "lib/utils.h"
#include "resource/strings.h"

namespace g0at
{
    namespace model
    {
        static bool convert_to_number(variable &var, variable *result)
        {
            int64_t int_value;
            if (var.get_integer(&int_value))
            {
                result->set_integer(int_value);
                return true;
            }
            double real_value;
            if (var.get_real(&real_value))
            {
                result->set_real(real_value);
                return true;
            }
            bool boolean_value;
            if (var.get_boolean(&boolean_value))
            {
                result->set_integer(boolean_value ? 1 : 0);
                return true;
            }
            wchar_t char_value;
            if (var.get_char(&char_value))
            {
                result->set_integer(char_value);
                return true;
            }
            object *obj = var.get_object();
            if (obj)
            {
                object_string *obj_string = obj->to_object_string();
                if (obj_string)
                {
                    std::wstring string_value = obj_string->get_data();
                    if (lib::wstring_to_int64(string_value, &int_value, 10))
                    {
                        result->set_integer(int_value);
                        return true;
                    }
                    if (lib::wstring_to_double(string_value, &real_value))
                    {
                        result->set_real(real_value);
                        return true;
                    }
                }
            }
            return false;
        }

        /* 
            Prototype
        */
        object_number_proto::object_number_proto(object_pool *pool)
            : object(pool)
        {
        }

        class object_number_valueof : public object_function_built_in
        {
        public:
            object_number_valueof(object_pool *_pool)
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
                    if (!convert_to_number(arg, &result))
                    {
                        result.set_object(thr->pool->get_null_instance());
                    }
                    thr->push(result);
                    return;
                }
                thr->raise_exception(new object_exception_illegal_argument(thr->pool));
            }
        };

        void object_number_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(resource::str_valueof), new object_number_valueof(pool));
            lock();
        }

        void object_number_proto::op_new(thread *thr, int arg_count)
        {
            variable result;
            if (arg_count > 0)
            {
                variable arg = thr->peek();
                thr->pop(arg_count);
                if (!convert_to_number(arg, &result))
                {
                    result.set_object(thr->pool->get_null_instance());
                }
            }
            else
            {
                result.set_integer(0);
            }
            thr->push(result);
        }
    };
};
