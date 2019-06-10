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
            model::object *new_object = new model::object(thr->pool, proto_object);

            model::variable var;
            var.set_object(new_object);
            thr->push(var);

            model::object_string *key = thr->pool->get_static_string(L"init");
            proto_object->find_own_and_call_if_exists(thr, arg_count, key, model::call_mode::as_constructor);
            proto_object->for_each_proto([thr, key](model::object *proto_proto_object)
            {
                proto_proto_object->find_own_and_call_if_exists(thr, 0, key, model::call_mode::as_constructor);
            });
        }
    };
};
