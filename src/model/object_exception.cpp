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

#include "object_exception.h"
#include "object_string.h"
#include "thread.h"
#include "global/global.h"
#include "resource/strings.h"

namespace g0at
{
    namespace model
    {
        object_exception::object_exception(object_pool *pool)
            : object(pool, pool->get_exception_proto_instance())
        {
        }

        object_exception *object_exception::to_object_exception()
        {
            return this;
        }

        std::wstring object_exception::to_string() const
        {
            return global::resource->unknown_exception();
        }

        std::wstring object_exception::to_string_notation() const
        {
            return resource::str_exception;
        }

        /*
            Prototype
        */

        object_exception_proto::object_exception_proto(object_pool *pool)
            : object(pool)
        {
        }

        std::wstring object_exception_proto::to_string() const
        {
            return global::resource->unknown_exception();
        }

        std::wstring object_exception_proto::to_string_notation() const
        {
            return resource::str_exception;
        }

        void object_exception_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(resource::str_IllegalArgument), pool->get_exception_illegal_argument_proto_instance());
            add_object(pool->get_static_string(resource::str_IllegalContext), pool->get_exception_illegal_context_proto_instance());
            add_object(pool->get_static_string(resource::str_IllegalOperation), pool->get_exception_illegal_operation_proto_instance());
            add_object(pool->get_static_string(resource::str_IllegalReference), pool->get_exception_illegal_reference_proto_instance());
            add_object(pool->get_static_string(resource::str_IllegalType), pool->get_exception_illegal_type_proto_instance());
            lock();
        }

        void object_exception_proto::op_new(thread *thr, int arg_count)
        {
            thr->pop(arg_count);
            variable result;
            result.set_object(new object_exception(thr->pool));
            thr->push(result);
        }

        /*
            IllegalArgument
        */
        object_exception_illegal_argument_proto::object_exception_illegal_argument_proto(object_pool *pool)
            : object(pool, pool->get_exception_proto_instance())
        {
        }

        std::wstring object_exception_illegal_argument_proto::to_string() const
        {
            return global::resource->illegal_argument();
        }

        void object_exception_illegal_argument_proto::init(object_pool *pool)
        {
            lock();
        }

        void object_exception_illegal_argument_proto::op_new(thread *thr, int arg_count)
        {
            thr->pop(arg_count);
            variable result;
            result.set_object(new object_exception_illegal_argument(thr->pool));
            thr->push(result);
        }

        object_exception_illegal_argument::object_exception_illegal_argument(object_pool *pool)
            : object(pool, pool->get_exception_illegal_argument_proto_instance())
        {
        }

        std::wstring object_exception_illegal_argument::to_string() const
        {
            return global::resource->illegal_argument();
        }

        /*
            IllegalContext
        */
        object_exception_illegal_context_proto::object_exception_illegal_context_proto(object_pool *pool)
            : object(pool, pool->get_exception_proto_instance())
        {
        }

        std::wstring object_exception_illegal_context_proto::to_string() const
        {
            return global::resource->illegal_context();
        }

        void object_exception_illegal_context_proto::init(object_pool *pool)
        {
            lock();
        }

        void object_exception_illegal_context_proto::op_new(thread *thr, int arg_count)
        {
            thr->pop(arg_count);
            variable result;
            result.set_object(new object_exception_illegal_context(thr->pool));
            thr->push(result);
        }

        object_exception_illegal_context::object_exception_illegal_context(object_pool *pool)
            : object(pool, pool->get_exception_illegal_context_proto_instance())
        {
        }

        std::wstring object_exception_illegal_context::to_string() const
        {
            return global::resource->illegal_context();
        }

        /*
            IllegalOperation
        */
        object_exception_illegal_operation_proto::object_exception_illegal_operation_proto(object_pool *pool)
            : object(pool, pool->get_exception_proto_instance())
        {
        }

        std::wstring object_exception_illegal_operation_proto::to_string() const
        {
            return global::resource->illegal_operation();
        }

        void object_exception_illegal_operation_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(resource::str_DivisionByZero), pool->get_exception_division_by_zero_proto_instance());
            add_object(pool->get_static_string(resource::str_IllegalReference), pool->get_exception_illegal_reference_proto_instance());
            add_object(pool->get_static_string(resource::str_IllegalType), pool->get_exception_illegal_type_proto_instance());
            add_object(pool->get_static_string(resource::str_UndeclaredVariable), pool->get_exception_undeclared_variable_proto_instance());
            lock();
        }

        void object_exception_illegal_operation_proto::op_new(thread *thr, int arg_count)
        {
            thr->pop(arg_count);
            variable result;
            result.set_object(new object_exception_illegal_operation(thr->pool));
            thr->push(result);
        }

        object_exception_illegal_operation::object_exception_illegal_operation(object_pool *pool)
            : object(pool, pool->get_exception_illegal_operation_proto_instance())
        {
        }

        std::wstring object_exception_illegal_operation::to_string() const
        {
            return global::resource->illegal_operation();
        }

        /*
            DivisionByZero
        */
        object_exception_division_by_zero_proto::object_exception_division_by_zero_proto(object_pool *pool)
            : object(pool, pool->get_exception_illegal_operation_proto_instance())
        {
        }

        std::wstring object_exception_division_by_zero_proto::to_string() const
        {
            return global::resource->division_by_zero();
        }

        void object_exception_division_by_zero_proto::init(object_pool *pool)
        {
            lock();
        }

        void object_exception_division_by_zero_proto::op_new(thread *thr, int arg_count)
        {
            thr->pop(arg_count);
            variable result;
            result.set_object(new object_exception_division_by_zero(thr->pool));
            thr->push(result);
        }

        object_exception_division_by_zero::object_exception_division_by_zero(object_pool *pool)
            : object(pool, pool->get_exception_division_by_zero_proto_instance())
        {
        }

        std::wstring object_exception_division_by_zero::to_string() const
        {
            return global::resource->division_by_zero();
        }

        /*
            IllegalReference
        */
        object_exception_illegal_reference_proto::object_exception_illegal_reference_proto(object_pool *pool)
            : object(pool, pool->get_exception_illegal_operation_proto_instance())
        {
        }

        std::wstring object_exception_illegal_reference_proto::to_string() const
        {
            return global::resource->illegal_reference();
        }

        void object_exception_illegal_reference_proto::init(object_pool *pool)
        {
            lock();
        }

        void object_exception_illegal_reference_proto::op_new(thread *thr, int arg_count)
        {
            thr->pop(arg_count);
            variable result;
            result.set_object(new object_exception_illegal_reference(thr->pool));
            thr->push(result);
        }

        object_exception_illegal_reference::object_exception_illegal_reference(object_pool *pool)
            : object(pool, pool->get_exception_illegal_reference_proto_instance())
        {
        }

        std::wstring object_exception_illegal_reference::to_string() const
        {
            return global::resource->illegal_reference();
        }

        /*
            IllegalType
        */
        object_exception_illegal_type_proto::object_exception_illegal_type_proto(object_pool *pool)
            : object(pool, pool->get_exception_illegal_operation_proto_instance())
        {
        }

        std::wstring object_exception_illegal_type_proto::to_string() const
        {
            return global::resource->illegal_type();
        }

        void object_exception_illegal_type_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(resource::str_IsNotAFunction), pool->get_exception_is_not_a_function_proto_instance());
            add_object(pool->get_static_string(resource::str_IsNotAMethod), pool->get_exception_is_not_a_method_proto_instance());
            add_object(pool->get_static_string(resource::str_OperatorNotFound), pool->get_exception_operator_not_found_proto_instance());
            lock();
        }

        void object_exception_illegal_type_proto::op_new(thread *thr, int arg_count)
        {
            thr->pop(arg_count);
            variable result;
            result.set_object(new object_exception_illegal_type(thr->pool));
            thr->push(result);
        }

        object_exception_illegal_type::object_exception_illegal_type(object_pool *pool)
            : object(pool, pool->get_exception_illegal_type_proto_instance())
        {
        }

        std::wstring object_exception_illegal_type::to_string() const
        {
            return global::resource->illegal_type();
        }

        /*
            OperatorNotFound
        */
        object_exception_operator_not_found_proto::object_exception_operator_not_found_proto(object_pool *pool)
            : object(pool, pool->get_exception_illegal_type_proto_instance())
        {
        }

        std::wstring object_exception_operator_not_found_proto::to_string() const
        {
            return global::resource->illegal_type();
        }

        void object_exception_operator_not_found_proto::init(object_pool *pool)
        {
            lock();
        }

        void object_exception_operator_not_found_proto::op_new(thread *thr, int arg_count)
        {
            if (arg_count > 0)
            {
                object *arg = thr->peek().get_object();
                if (arg)
                {
                    object_string *arg_string = arg->to_object_string();
                    if (arg_string)
                    {
                        thr->pop(arg_count);
                        variable result;
                        result.set_object(new object_exception_operator_not_found(thr->pool, arg_string->get_data()));
                        thr->push(result);
                        return;
                    }
                }
            }
            thr->raise_exception(new object_exception_illegal_argument(thr->pool));
        }

        object_exception_operator_not_found::object_exception_operator_not_found(object_pool *pool, std::wstring _oper)
            : object(pool, pool->get_exception_operator_not_found_proto_instance()), oper(_oper)
        {
        }

        std::wstring object_exception_operator_not_found::to_string() const
        {
            return global::resource->operator_not_found(oper);
        }

        /*
            IsNotAFunction
        */
        object_exception_is_not_a_function_proto::object_exception_is_not_a_function_proto(object_pool *pool)
            : object(pool, pool->get_exception_illegal_type_proto_instance())
        {
        }

        std::wstring object_exception_is_not_a_function_proto::to_string() const
        {
            return global::resource->illegal_type();
        }

        void object_exception_is_not_a_function_proto::init(object_pool *pool)
        {
            lock();
        }

        void object_exception_is_not_a_function_proto::op_new(thread *thr, int arg_count)
        {
            if (arg_count > 0)
            {
                object *arg = thr->peek().get_object();
                if (arg)
                {
                    object_string *arg_string = arg->to_object_string();
                    if (arg_string)
                    {
                        thr->pop(arg_count);
                        variable result;
                        result.set_object(new object_exception_is_not_a_function(thr->pool, arg_string->get_data()));
                        thr->push(result);
                        return;
                    }
                }
            }
            thr->raise_exception(new object_exception_illegal_argument(thr->pool));
        }

        object_exception_is_not_a_function::object_exception_is_not_a_function(object_pool *pool, std::wstring _name)
            : object(pool, pool->get_exception_is_not_a_function_proto_instance()), name(_name)
        {
        }

        std::wstring object_exception_is_not_a_function::to_string() const
        {
            return global::resource->is_not_a_function(name);
        }

        /*
            IsNotAMethod
        */
        object_exception_is_not_a_method_proto::object_exception_is_not_a_method_proto(object_pool *pool)
            : object(pool, pool->get_exception_illegal_type_proto_instance())
        {
        }

        std::wstring object_exception_is_not_a_method_proto::to_string() const
        {
            return global::resource->illegal_type();
        }

        void object_exception_is_not_a_method_proto::init(object_pool *pool)
        {
            lock();
        }

        void object_exception_is_not_a_method_proto::op_new(thread *thr, int arg_count)
        {
            if (arg_count > 0)
            {
                object *arg = thr->peek().get_object();
                if (arg)
                {
                    object_string *arg_string = arg->to_object_string();
                    if (arg_string)
                    {
                        thr->pop(arg_count);
                        variable result;
                        result.set_object(new object_exception_is_not_a_method(thr->pool, arg_string->get_data()));
                        thr->push(result);
                        return;
                    }
                }
            }
            thr->raise_exception(new object_exception_illegal_argument(thr->pool));
        }

        object_exception_is_not_a_method::object_exception_is_not_a_method(object_pool *pool, std::wstring _name)
            : object(pool, pool->get_exception_is_not_a_method_proto_instance()), name(_name)
        {
        }

        std::wstring object_exception_is_not_a_method::to_string() const
        {
            return global::resource->is_not_a_method(name);
        }

        /*
            UndeclaredVariable
        */
        object_exception_undeclared_variable_proto::object_exception_undeclared_variable_proto(object_pool *pool)
            : object(pool, pool->get_exception_illegal_operation_proto_instance())
        {
        }

        std::wstring object_exception_undeclared_variable_proto::to_string() const
        {
            return global::resource->illegal_operation();
        }

        void object_exception_undeclared_variable_proto::init(object_pool *pool)
        {
            lock();
        }

        void object_exception_undeclared_variable_proto::op_new(thread *thr, int arg_count)
        {
            if (arg_count > 0)
            {
                object *arg = thr->peek().get_object();
                if (arg)
                {
                    object_string *arg_string = arg->to_object_string();
                    if (arg_string)
                    {
                        thr->pop(arg_count);
                        variable result;
                        result.set_object(new object_exception_undeclared_variable(thr->pool, arg_string->get_data()));
                        thr->push(result);
                        return;
                    }
                }
            }
            thr->raise_exception(new object_exception_illegal_argument(thr->pool));
        }

        object_exception_undeclared_variable::object_exception_undeclared_variable(object_pool *pool, std::wstring _name)
            : object(pool, pool->get_exception_undeclared_variable_proto_instance()), name(_name)
        {
        }

        std::wstring object_exception_undeclared_variable::to_string() const
        {
            return global::resource->undeclared_variable(name);
        }
    };
};
