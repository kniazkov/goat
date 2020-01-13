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

#include "string.h"
#include "model/object_string.h"

namespace g0at
{
    namespace code
    {
        _string::_string(int _id)
            : id(_id)
        {
        }

        void _string::accept(instruction_visitor *visitor)
        {
            visitor->visit(this);
        }

        void _string::exec(model::thread *thr)
        {
            model::variable var;
            model::object_string *obj = thr->pool->get_static_string(id);
            var.set_object(obj);
            thr->push(var);
        }
    };
};
