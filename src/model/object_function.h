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

#pragma once

#include "object.h"
#include "thread.h"

namespace g0at
{
    namespace model
    {
        enum class call_mode
        {
            as_function,
            as_method,
            as_constructor
        };

        class object_function : public object
        {
        public:
            object_function(object_pool *pool);
            object_type get_type() const override;
            object_function *to_object_function() override;
            std::wstring to_string() const override;
            virtual void call(thread *thr, int arg_count, call_mode mode) = 0;
        };

        class object_function_proto : public object
        {
        friend class object_pool;
        protected:
            object_function_proto(object_pool *pool);
            void init(object_pool *pool);
        public:
            void op_new(thread *thr, int arg_count) override;
        };
    };
};