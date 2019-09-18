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

#include "retv.h"
#include "lib/assert.h"

namespace g0at
{
    namespace code
    {
        void _retv::accept(instruction_visitor *visitor)
        {
            visitor->visit(this);
        }

        void _retv::exec(model::thread *thr)
        {
            model::variable val = thr->pop();
            thr->flow = model::thread_flow::direct;
            if (thr->ctx->address_type == model::context_address_type::ret_address)
            {
                if (thr->ctx->ret)
                    *(thr->ctx->ret) = val;
                thr->iid = thr->ctx->address[0];
                thr->restore_context();
                assert(thr->ctx != nullptr);
            }
            else
            {
                thr->restore_context();
                while (thr->ctx)
                {
                    switch(thr->ctx->address_type)
                    {
                        case model::context_address_type::ret_address :
                            if (thr->ctx->ret)
                                *(thr->ctx->ret) = val;
                            thr->iid = thr->ctx->address[0];
                            thr->restore_context();
                            assert(thr->ctx != nullptr);
                            return;
                        case model::context_address_type::fin_address :
                            if (thr->ctx->ret)
                                *(thr->ctx->ret) = val;
                            thr->flow = model::thread_flow::descent_return;
                            thr->iid = thr->ctx->address[0];
                            return;
                        default:
                            thr->restore_context();
                    }
                }
                thr->state = model::thread_state::zombie;
                if (thr->ret)
                    *(thr->ret) = val;
            }
        }

        bool _retv::exec_debug(model::thread *thr, vm::debug_mode_info *debug_info)
        {
            exec(thr);
            debug_info->current_level--;
            return false;
        }
    };
};
