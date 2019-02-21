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

#include "load_func.h"
#include "model/object_function_user_defined.h"
#include <assert.h>

namespace g0at
{
    namespace code
    {
        load_func::load_func(int _first_iid)
            : first_iid(_first_iid)
        {
        }

        void load_func::accept(instruction_visitor *visitor)
        {
            visitor->visit(this);
        }

        void load_func::exec(model::thread *thr)
        {
            model::object_function_user_defined *obj = new model::object_function_user_defined(thr->o_list, first_iid, thr->ctx);
            for (int i = 0, size = (int)arg_ids.size(); i < size; i++)
            {
                model::object_string *name = thr->cache->get_object(arg_ids[i]);
                obj->add_arg_name(name);
            }
            model::variable var;
            var.set_object(obj);
            thr->push(var);
        }
    };
};
