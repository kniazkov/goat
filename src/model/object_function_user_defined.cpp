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

#include "object_function_user_defined.h"
#include <assert.h>

namespace g0at
{
    namespace model
    {
        object_function_user_defined::object_function_user_defined(object_list *list, int _first_iid)
            : object_function(list), first_iid(_first_iid)
        {
            assert(_first_iid > 0);
        }

        void object_function_user_defined::call(thread *thr, int arg_count)
        {
            context *ctx = new context(thr->o_list, thr->ctx);
            ctx->value = thr->iid;
            ctx->value_type = context_value_type::ret_address;
            for (int i = 0, decl_arg_count = (int)arg_names.size(); i < decl_arg_count; i++)
            {
                object *key = arg_names[i];
                if (i < arg_count)
                    ctx->add_object(key, thr->peek(i));
                else
                    ctx->add_object(key, thr->o_list->get_undefined_instance());
            }
            thr->ctx = ctx;
            thr->iid = first_iid;
        }
    };
};
