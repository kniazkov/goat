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

#include "var.h"
#include "model/object_string.h"
#include <assert.h>

namespace g0at
{
    namespace code
    {
        _var::_var(int _id)
            : id(_id)
        {
        }

        void _var::accept(instruction_visitor *visitor)
        {
            visitor->visit(this);
        }

        void _var::exec(model::thread *thr)
        {
            model::object_string *key = thr->pool->get_static_string(id);
            model::variable value = thr->pop();
            thr->ctx->add_object(key, value);
        }
    };
};
