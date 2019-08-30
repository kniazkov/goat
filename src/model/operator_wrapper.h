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

#include "object_function_built_in.h"

namespace g0at
{
    namespace model
    {
        struct wrap_add
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_add(thr);
            }
        };

        template <typename W> class binary_operator_adapter : public object_function_built_in
        {
        public:
            binary_operator_adapter(object_pool *pool)
                : object_function_built_in(pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                if (arg_count != 1)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_argument_instance());
                    return;
                }
                object *this_ptr = thr->peek().get_object();
                W::call(this_ptr, thr);
            }
        };
    };
};