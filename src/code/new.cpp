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

#include "new.h"
#include "model/object_string.h"
#include "model/object_function.h"
#include "lib/assert.h"

namespace g0at
{
    namespace code
    {
        _new::_new(int _arg_count)
            : arg_count(_arg_count)
        {
            assert(_arg_count >= 0);
        }

        void _new::accept(instruction_visitor *visitor)
        {
            visitor->visit(this);
        }

        void _new::exec(model::thread *thr)
        {
            model::variable proto = thr->pop();
            model::object *proto_object = proto.to_object(thr->pool);
            proto_object->op_new(thr, arg_count);
        }
    };
};
