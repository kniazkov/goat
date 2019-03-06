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
#include "gc.h"
#include "process.h"
#include "model/object.h"
#include "model/object_cache.h"
#include "model/built_in/context_factory.h"
#include "code/disasm.h"
#include "global/global.h"
#include <assert.h>
#include <iostream>

namespace g0at
{
    namespace vm
    {
        vm::vm(lib::pointer<code::code> _code)
            : code(_code)
        {
        }

        void vm::run(environment *env)
        {
            model::object_pool pool;
            model::object_cache cache(code->get_identifiers_list(), &pool);
            model::context *ctx = model::built_in::context_factory(&pool, &cache).create_context();
            model::thread thr(ctx, &pool, &cache);
            thr.next = &thr;
            thr.state = model::thread_state::ok;
            process proc;
            proc.pool = &pool;
            proc.cache = &cache;
            proc.threads = &thr;
            if (!global::debug)
            {
                while(thr.state == model::thread_state::ok)
                {
                    uint32_t iid = thr.iid;
                    thr.iid++;
                    auto instr = code->get_instruction(iid);
                    instr->exec(&thr);
                    env->gc->collect_garbage(&proc);
                }
            }
            else
            {
                // debug mode
                while(thr.state == model::thread_state::ok)
                {
                    uint32_t iid = thr.iid;
                    thr.iid++;
                    auto instr = code->get_instruction(iid);
                    instr->exec(&thr);
                    if (!thr.stack_is_empty())
                    {
                        // convert any value to real object
                        thr.peek().to_object(&pool);
                    }
                    env->gc->collect_garbage(&proc);
                }
            }
            assert(thr.stack_is_empty());
            pool.destroy_all();
        }
    };
};
