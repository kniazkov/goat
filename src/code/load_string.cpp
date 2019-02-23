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

#include "load_string.h"
#include "model/object_string.h"

namespace g0at
{
    namespace code
    {
        load_string::load_string(int _id)
            : id(_id)
        {
        }

        void load_string::accept(instruction_visitor *visitor)
        {
            visitor->visit(this);
        }

        void load_string::exec(model::thread *thr)
        {
            model::variable var;
            model::object_string *obj = thr->cache->get_object(id);
            var.set_object(obj);
            thr->push(var);
        }
    };
};
