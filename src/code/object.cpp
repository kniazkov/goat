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

#include "object.h"
#include "lib/assert.h"

namespace g0at
{
    namespace code
    {
        _object::_object(int _count)
            : count(_count)
        {
            assert(_count >= 0);
        }

        void _object::accept(instruction_visitor *visitor)
        {
            visitor->visit(this);
        }

        void _object::exec(model::thread *thr)
        {
            model::object *result = thr->pool->create_generic_object();
            model::variable var;
            var.set_object(result);

            for (int i = 0; i < count; i++)
            {
                model::object *key = thr->pop().to_object(thr->pool);
                model::variable value = thr->pop().deref();
                result->add_object(key, value);
            }

            thr->push(var);
        }
    };
};
