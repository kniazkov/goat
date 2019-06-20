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

#include "lib/new.h"
#include "vm.h"
#include "gc.h"
#include "process.h"
#include "model/object.h"
#include "model/object_cache.h"
#include "model/built_in/context_factory.h"
#include "code/disasm.h"
#include "global/global.h"
#include "lib/assert.h"
#include <iostream>
#include <climits>

namespace g0at
{
    namespace vm
    {
        vm::vm(lib::pointer<code::code> _code)
            : code(_code)
        {
        }

        vm_report vm::run(environment *env)
        {
            model::object_pool pool(code->get_identifiers_list());
            model::context *ctx = model::built_in::context_factory(&pool).create_context();
            model::thread_list tlist(&pool);
            model::variable ret;
            model::thread *thr = tlist.create_thread(ctx, &ret);
            ret.set_object(pool.get_undefined_instance());
            thr->iid = code::iid_t(0);
            thr->next = thr;
            thr->state = model::thread_state::ok;
            process proc;
            proc.pool = &pool;
            proc.threads = &tlist;
            lib::pointer<lib::gc> gc = create_garbage_collector(env->gct, &proc);
            lib::set_garbage_collector(gc.get());
            if (!global::debug)
            {
                while(thr != nullptr)
                {
                    code::iid_t iid = thr->iid;
                    ++thr->iid;
                    auto instr = code->get_instruction(iid);
                    instr->exec(thr);
                    gc->collect_garbage_if_necessary();
                    thr = tlist.switch_thread();
                }
            }
            else
            {
                #if 0
                // debug mode
                while(thr->state == model::thread_state::ok)
                {
                    uint32_t iid = thr->iid;
                    thr->iid++;
                    auto instr = code->get_instruction(iid);
                    instr->exec(thr);
                    if (!thr->stack_is_empty())
                    {
                        // convert any value to real object
                        thr->peek().to_object(&pool);
                    }
                    gc->collect_garbage_if_necessary();
                }
                #else
                assert(false);
                #endif
            }

            vm_report r;
            int64_t ret_value_int64;
            if (ret.get_integer(&ret_value_int64) && ret_value_int64 >= INT_MIN && ret_value_int64 <= INT_MAX)
                r.ret_value = (int)ret_value_int64;
            else
                r.ret_value = 0;
            r.gcr = gc->get_report();
            r.opr = pool.get_report();
            pool.destroy_all();
            lib::set_garbage_collector(nullptr);
            return r;
        }
    };
};
