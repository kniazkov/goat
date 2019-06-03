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

namespace g0at
{
    namespace code
    {
        void _new::accept(instruction_visitor *visitor)
        {
            visitor->visit(this);
        }

        void _new::exec(model::thread *thr)
        {
            model::variable proto = thr->pop();
            model::object *new_object = new model::object(thr->pool, proto.to_object(thr->pool));

            model::variable var;
            var.set_object(new_object);
            thr->push(var);

            model::object_string *key = thr->pool->get_static_string(L"init");
            model::variable *init_var = new_object->find_object(key);
            if (init_var)
            {
                model::object *init_obj = init_var->get_object();
                if(init_obj)
                {
                    model::object_function *init_func = init_obj->to_object_function();
                    if (init_func)
                    {
                        init_func->call(thr, 0, model::call_mode::as_constructor);
                    }
                }
            }
        }
    };
};
