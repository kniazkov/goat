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
#include "../model/object_function.h"
#include <assert.h>

namespace g0at
{
    namespace code
    {
        void call::accept(instruction_visitor *visitor)
        {
            visitor->visit(this);
        }

        void call::exec(model::thread *thr)
        {
            model::object *obj = thr->pop().to_object(thr->o_list);
            model::object_function *func = obj->to_object_function();
            assert(func != nullptr); // TODO: exception if is not a function
            func->call(thr);
        }
    };
};