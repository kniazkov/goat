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

#include "vcall.h"
#include "model/object_function.h"
#include "model/object_string.h"
#include "model/object_exception.h"
#include "lib/assert.h"

namespace g0at
{
    namespace code
    {
        _vcall::_vcall(int _id, int _arg_count )
            : id(_id), arg_count(_arg_count)
        {
            assert(_arg_count >= 0);
        }

        void _vcall::accept(instruction_visitor *visitor)
        {
            visitor->visit(this);
        }

        void _vcall::exec(model::thread *thr)
        {
            model::object_function *func = nullptr;
            model::object_string *key = thr->pool->get_static_string(id);
            model::variable *var = thr->peek().to_object(thr->pool)->find_object(key);
            if (var)
            {
                model::object *obj = var->get_object();
                if(obj)
                    func = obj->to_object_function();
            }
            if (func == nullptr)
                thr->raise_exception(new model::object_exception_is_not_a_method(thr->pool, key->to_string_notation()));
            else
                func->call(thr, arg_count, model::call_mode::as_method);
        }

        bool _vcall::exec_debug(model::thread *thr, vm::debug_mode_info *debug_info)
        {
            exec(thr);
            source_manager *listing = debug_info->get_listing();
            if (listing)
                thr->ctx->pos = listing->get_position_by_absolute_position(debug_info->frame_begin);
            thr->ctx->debug_level++;
            return false;
        }
    };
};
