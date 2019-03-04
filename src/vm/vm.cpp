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

        void vm::run()
        {
            model::object_pool o_pool;
            model::object_cache cache(code->get_identifiers_list(), &o_pool);
            model::context *ctx = model::built_in::context_factory(&o_pool, &cache).create_context();
            model::thread thr(ctx, &o_pool, &cache);
            thr.state = model::thread_state::ok;
            if (!global::debug)
            {
                while(thr.state == model::thread_state::ok)
                {
                    uint32_t iid = thr.iid;
                    thr.iid++;
                    auto instr = code->get_instruction(iid);
                    instr->exec(&thr);
#if 0                
                    std::wstringstream tmp;
                    code::disasm visitor(tmp, code->get_identifiers_list());
                    instr->accept(&visitor);
                    std::cout << std::endl << instr->get_id() << "\t" << global::char_encoder->encode(tmp.str()) << " (" << thr.get_stack_size() << ") ";
#endif
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
                        thr.peek().to_object(&o_pool);
                    }
                }
            }
            assert(thr.stack_is_empty());
            o_pool.destroy_all();
        }
    };
};
