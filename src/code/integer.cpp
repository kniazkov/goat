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

#include "integer.h"
#include "model/object_integer.h"

namespace g0at
{
    namespace code
    {
        _integer::_integer(int64_t _value)
            : value(_value)
        {
        }

        void _integer::accept(instruction_visitor *visitor)
        {
            visitor->visit(this);
        }

        void _integer::exec(model::thread *thr)
        {
            model::variable var;
            var.set_integer(value);
            thr->push(var);
        }
    };
};
