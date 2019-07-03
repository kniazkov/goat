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

#include "leave.h"
#include "lib/assert.h"

namespace g0at
{
    namespace code
    {
        void _leave::accept(instruction_visitor *visitor)
        {
            visitor->visit(this);
        }

        void _leave::exec(model::thread *thr)
        {
            thr->restore_context();
            switch(thr->flow)
            {
                case model::thread_flow::direct :
                    assert(thr->ctx != nullptr);
                    return;
                case model::thread_flow::descent_return :
                    while (thr->ctx)
                    {
                        switch(thr->ctx->address_type)
                        {
                            case model::context_address_type::ret_address :
                                thr->flow = model::thread_flow::direct;
                                thr->iid = thr->ctx->address[0];
                                thr->restore_context();
                                assert(thr->ctx != nullptr);
                                return;
                            case model::context_address_type::fin_address :
                                thr->iid = thr->ctx->address[0];
                                return;
                            default:
                                thr->restore_context();
                        }
                    }
                    thr->state = model::thread_state::zombie;
                    return;
                case model::thread_flow::descent_exception :
                    while (thr->ctx)
                    {
                        switch(thr->ctx->address_type)
                        {
                            case model::context_address_type::catch_address :
                                thr->flow = model::thread_flow::direct;
                                thr->iid = thr->ctx->address[0];
                                return;
                            case model::context_address_type::fin_address :
                                thr->iid = thr->ctx->address[0];
                                return;
                            default:
                                thr->restore_context();
                        }
                    }
                    thr->state = model::thread_state::zombie;
                    return;
                case model::thread_flow::descent_break :
                    while (thr->ctx)
                    {
                        switch(thr->ctx->address_type)
                        {
                            case model::context_address_type::cycle_addresses :
                                thr->flow = model::thread_flow::direct;
                                thr->iid = thr->ctx->address[1];
                                thr->restore_context();
                                assert(thr->ctx != nullptr);
                                return;
                            case model::context_address_type::fin_address :
                                thr->iid = thr->ctx->address[0];
                                return;
                            default:
                                thr->restore_context();
                        }
                    }
                    thr->state = model::thread_state::zombie;
                    return;
            }
        }
    };
};
