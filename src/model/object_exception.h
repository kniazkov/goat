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

#pragma once

#include "object.h"

namespace g0at
{
    namespace model
    {
        /*
            Goat exception hierarchy:

            Exception
                IllegalArgument
                IllegalContext
                IllegalOperation
                IllegalReference
                    OperatorNotFound
                IllegalType
                    IsNotAFunction
                    IsNotAMethod
        */

        class object_exception : public object
        {
        public:
            object_exception(object_pool *pool);
            object_exception *to_object_exception() override;
            std::wstring to_string() const override;
            std::wstring to_string_notation() const override;
        };

        class object_exception_proto : public object
        {
        friend class object_pool;
        protected:
            object_exception_proto(object_pool *pool);
            std::wstring to_string() const override;
            std::wstring to_string_notation() const override;
            void init(object_pool *pool);
        };

        class object_exception_illegal_argument : public object_exception
        {
        friend class object_pool;
        protected:
            object_exception_illegal_argument(object_pool *pool);
            std::wstring to_string() const override;
        };

        class object_exception_illegal_context : public object_exception
        {
        friend class object_pool;
        protected:
            object_exception_illegal_context(object_pool *pool);
            std::wstring to_string() const override;
        };

        class object_exception_illegal_operation : public object_exception
        {
        friend class object_pool;
        protected:
            object_exception_illegal_operation(object_pool *pool);
            std::wstring to_string() const override;
        };

        class object_exception_illegal_reference : public object_exception
        {
        friend class object_pool;
        protected:
            object_exception_illegal_reference(object_pool *pool);
            void init(object_pool *pool);
            std::wstring to_string() const override;
        };

        class object_exception_illegal_type : public object_exception
        {
        friend class object_pool;
        protected:
            object_exception_illegal_type(object_pool *pool);
            void init(object_pool *pool);
            std::wstring to_string() const override;
        };

        class object_exception_operator_not_found_proto : public object
        {
        friend class object_pool;
        protected:
            object_exception_operator_not_found_proto(object_pool *pool);
            std::wstring to_string() const override;
        };

        class object_exception_operator_not_found : public object
        {
        public:
            object_exception_operator_not_found(object_pool *pool, std::wstring _oper);
            std::wstring to_string() const override;
        
        protected:
            std::wstring oper;
        };

        class object_exception_is_not_a_function_proto : public object
        {
        friend class object_pool;
        protected:
            object_exception_is_not_a_function_proto(object_pool *pool);
            std::wstring to_string() const override;
        };

        class object_exception_is_not_a_function : public object
        {
        public:
            object_exception_is_not_a_function(object_pool *pool, std::wstring _name);
            std::wstring to_string() const override;
        
        protected:
            std::wstring name;
        };

        class object_exception_is_not_a_method_proto : public object
        {
        friend class object_pool;
        protected:
            object_exception_is_not_a_method_proto(object_pool *pool);
            std::wstring to_string() const override;
        };

        class object_exception_is_not_a_method : public object
        {
        public:
            object_exception_is_not_a_method(object_pool *pool, std::wstring _name);
            std::wstring to_string() const override;
        
        protected:
            std::wstring name;
        };
    };
};