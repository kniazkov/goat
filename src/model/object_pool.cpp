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

#include "object_pool.h"
#include "object.h"
#include "lib/assert.h"
#include "object_void.h"
#include "object_undefined.h"
#include "object_null.h"
#include "object_string.h"
#include "object_number.h"
#include "object_integer.h"
#include "object_function.h"
#include "object_thread.h"
#include "object_boolean.h"
#include "object_real.h"
#include "object_array.h"
#include "object_exception.h"
#include "object_char.h"
#include "object_uid.h"
#include "object_iterator.h"
#include "object_runner.h"
#include "context.h"

namespace g0at
{
    namespace model
    {
        object_pool::object_pool(std::vector<std::wstring> &identifiers_list)
        {
            id = 0;

            generic_proto_instance = nullptr;
            string_proto_instance = nullptr;
            void_instance = nullptr;
            undefined_instance = nullptr;
            null_instance = nullptr;
            number_proto_instance = nullptr;
            integer_proto_instance = nullptr;
            function_proto_instance = nullptr;
            thread_proto_instance = nullptr;
            runner_proto_instance = nullptr;
            boolean_proto_instance = nullptr;
            real_proto_instance = nullptr;
            array_proto_instance = nullptr;
            char_proto_instance = nullptr;
            uid_proto_instance = nullptr;
            iterator_proto_instance = nullptr;
            exception_proto_instance = nullptr;
            exception_illegal_argument_instance = nullptr;
            exception_illegal_context_instance = nullptr;
            exception_illegal_reference_instance = nullptr;
            exception_illegal_type_instance = nullptr;
            exception_operator_not_found_proto_instance = nullptr;
            exception_is_not_a_function_proto_instance = nullptr;
            exception_is_not_a_method_proto_instance = nullptr;

            /*
                "It's a kind of magic"
                That is, we must initialize the objects one by one,
                but in such a way as to maintain the correct dependency.
            */
            auto gp = new generic_proto(this);
            generic_proto_instance = gp;
            auto string_proto = new object_string_proto(this);
            string_proto_instance = string_proto;
            static_strings.init(identifiers_list, this);
            auto function_proto = new object_function_proto(this);
            function_proto_instance = function_proto;
            auto thread_proto = new object_thread_proto(this);
            thread_proto_instance = thread_proto;
            auto runner_proto = new object_runner_proto(this);
            runner_proto_instance = runner_proto;
            gp->init(this);
            void_instance = new object_void(this);
            undefined_instance = new object_undefined(this);
            null_instance = new object_null(this);
            number_proto_instance = new object_number_proto(this);
            auto integer_proto = new object_integer_proto(this);
            integer_proto_instance = integer_proto;
            boolean_proto_instance = new object_boolean_proto(this);
            auto real_proto = new object_real_proto(this);
            real_proto_instance = real_proto;
            auto array_proto = new object_array_proto(this);
            array_proto_instance = array_proto;
            auto char_proto = new object_char_proto(this);
            char_proto_instance = char_proto;
            auto uid_proto = new object_uid_proto(this);
            uid_proto_instance = uid_proto;
            auto exception_proto = new object_exception_proto(this);
            exception_proto_instance = exception_proto;
            auto iterator_proto = new object_iterator_proto(this);
            iterator_proto_instance = iterator_proto;
            string_proto->init(this);
            function_proto->init(this);
            thread_proto->init(this);
            integer_proto->init(this);
            real_proto->init(this);
            char_proto->init(this);
            uid_proto->init(this);
            runner_proto->init(this);
            array_proto->init(this);
            iterator_proto->init(this);
            exception_illegal_argument_instance = new object_exception_illegal_argument(this);
            exception_illegal_context_instance = new object_exception_illegal_context(this);
            exception_illegal_operation_instance = new object_exception_illegal_operation(this);
            auto exception_illegal_reference = new object_exception_illegal_reference(this);
            exception_illegal_reference_instance = exception_illegal_reference;
            auto exception_illegal_type = new object_exception_illegal_type(this);
            exception_illegal_type_instance = exception_illegal_type; 
            exception_operator_not_found_proto_instance = new object_exception_operator_not_found_proto(this);
            exception_is_not_a_function_proto_instance = new object_exception_is_not_a_function_proto(this);
            exception_is_not_a_method_proto_instance = new object_exception_is_not_a_method_proto(this);
            exception_proto->init(this);
            exception_illegal_reference->init(this);
            exception_illegal_type->init(this);
        }

        void object_pool::add(object *item)
        {
            report.new_count++;
            population.add(item);
        }

        void object_pool::destroy_all()
        {
            population.destroy_all();
            generic_objects.dead.destroy_all();
            contexts.dead.destroy_all();
            strings.dead.destroy_all();
            integers.dead.destroy_all();
            real_numbers.dead.destroy_all();
            booleans.dead.destroy_all();
            arrays.dead.destroy_all();
            chars.dead.destroy_all();
        }

        generic_object * object_pool::create_generic_object()
        {
            generic_object *obj = static_cast<generic_object*>(generic_objects.get(this));
            if (obj)
                obj->reinit(this);
            else
                obj = new generic_object(this);
            return obj;
        }

        context * object_pool::create_context()
        {
            context *obj = static_cast<context*>(contexts.get(this));
            if (obj)
                obj->reinit(this);
            else
                obj = new context(this);
            return obj;
        }

        context * object_pool::create_context(context *proto)
        {
            context *obj = static_cast<context*>(contexts.get(this));
            if (obj)
                obj->reinit(proto);
            else
                obj = new context(this, proto);
            return obj;
        }

        context * object_pool::create_context(context *proto, context *parent)
        {
            context *obj = static_cast<context*>(contexts.get(this));
            if (obj)
                obj->reinit(proto, parent);
            else
                obj = new context(this, proto, parent);
            return obj;
        }

        context * object_pool::create_context(object *this_ptr, context *proto, context *parent)
        {
            context *obj = static_cast<context*>(contexts.get(this));
            if (obj)
                obj->reinit(this_ptr, proto, parent);
            else
                obj = new context(this, this_ptr, proto, parent);
            return obj;
        }

        object_string * object_pool::create_object_string(std::wstring data)
        {
            object_string *obj = static_cast<object_string*>(strings.get(this));
            if (obj)
                obj->reinit(data);
            else
                obj = new object_string(this, data);
            return obj;
        }

        object_string * object_pool::create_object_string(std::wstring data, int id)
        {
            object_string *obj = static_cast<object_string*>(strings.get(this));
            if (obj)
                obj->reinit(data, id);
            else
                obj = new object_string(this, data, id);
            return obj;
        }

        object_integer * object_pool::create_object_integer(int64_t value)
        {
            object_integer *obj = static_cast<object_integer*>(integers.get(this));
            if (obj)
                obj->reinit(value);
            else
                obj = new object_integer(this, value);
            return obj;
        }

        object_real * object_pool::create_object_real(double value)
        {
            object_real *obj = static_cast<object_real*>(real_numbers.get(this));
            if (obj)
                obj->reinit(value);
            else
                obj = new object_real(this, value);
            return obj;
        }

        object_boolean * object_pool::create_object_boolean(bool value)
        {
            object_boolean *obj = static_cast<object_boolean*>(booleans.get(this));
            if (obj)
                obj->reinit(value);
            else
                obj = new object_boolean(this, value);
            return obj;
        }

        object_array * object_pool::create_object_array()
        {
            object_array *obj = static_cast<object_array*>(arrays.get(this));
            if (obj)
                obj->reinit();
            else
                obj = new object_array(this);
            return obj;
        }

        object_char * object_pool::create_object_char(wchar_t value)
        {
            object_char *obj = static_cast<object_char*>(chars.get(this));
            if (obj)
                obj->reinit(value);
            else
                obj = new object_char(this, value);
            return obj;
        }

        object_uid * object_pool::create_object_uid()
        {
            object_uid *obj = static_cast<object_uid*>(uids.get(this));
            if (obj)
                obj->reinit();
            else
                obj = new object_uid(this);
            return obj;
        }
    };
};
