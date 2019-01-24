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

#include "load_var.h"
#include "../model/object_string.h"
#include <assert.h>

namespace g0at
{
    namespace code
    {
        load_var::load_var(std::wstring _name)
            : name(_name)
        {
        }

        void load_var::accept(instruction_visitor *visitor)
        {
            visitor->visit(this);
        }

        void load_var::exec(model::thread *thr)
        {
            // TODO: fix it please!
            model::object_string *key = new model::object_string(thr->o_list, name);
            model::variable *var = thr->ctx->find_object(key);
            assert(var != nullptr); // exception!
            thr->push(*var);
        }
    };
};