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

#include "call.h"
#include "model/object_function.h"
#include "model/object_string.h"
#include "model/object_exception.h"
#include "lib/assert.h"

namespace g0at
{
    namespace code
    {
        _call::_call(int _arg_count)
            : arg_count(_arg_count)
        {
            assert(_arg_count >= 0);
        }

        void _call::accept(instruction_visitor *visitor)
        {
            visitor->visit(this);
        }

        void _call::exec(model::thread *thr)
        {
            // get func. object from the stack
            model::object *obj = thr->pop().to_object(thr->pool);
            model::object_function *func = obj->to_object_function();

            // call
            if(func == nullptr)
                thr->raise_exception(new model::object_exception_is_not_a_function(thr->pool, obj->to_string_notation()));
            else
                func->call(thr, arg_count, model::call_mode::as_function);
        }

        bool _call::exec_debug(model::thread *thr, vm::debug_mode_info *debug_info)
        {
            exec(thr);
            thr->ctx->debug_level++;
            return false;
        }
    };
};
