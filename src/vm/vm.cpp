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

#include "vm.h"
#include "model/object.h"
#include "model/object_cache.h"
#include "model/built_in/context_factory.h"
#include <assert.h>

namespace g0at
{
    namespace vm
    {
        vm::vm(lib::pointer<code::code> _code)
            : code(_code)
        {
        }

        void vm::run()
        {
            model::object_list o_list;
            model::object_cache cache(code->get_identifiers_list(), &o_list);
            model::context *ctx = model::built_in::context_factory(&o_list, &cache).create_context();
            model::thread thr(ctx, &o_list, &cache);
            thr.state = model::thread_state::WORK;
            uint32_t iid = 0;
            while(thr.state == model::thread_state::WORK)
            {
                code->get_instruction(iid)->exec(&thr);
                iid++;
            }
            assert(thr.stack_is_empty());
            o_list.destroy_all();
        }
    };
};
