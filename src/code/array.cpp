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

#include "array.h"
#include "model/object_array.h"
#include "lib/assert.h"

namespace g0at
{
    namespace code
    {
        _array::_array(int _count)
            : count(_count)
        {
            assert(_count >= 0);
        }

        void _array::accept(instruction_visitor *visitor)
        {
            visitor->visit(this);
        }

        void _array::exec(model::thread *thr)
        {
            model::object_array *result = thr->pool->create_object_array();
            model::variable var;
            var.set_object(result);

            for (int i = 0; i < count; i++)
            {
                model::variable item = thr->pop();
                result->add_item(item);
            }

            thr->push(var);
        }
    };
};
