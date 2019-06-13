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
            while(
                thr->ctx 
                && thr->ctx->value_type != model::context_value_type::ret_address
                && thr->ctx->value_type != model::context_value_type::fin_address)
            {
                thr->restore_context();
            }

            if (!thr->ctx)
            {
                thr->state = model::thread_state::zombie;
                if (thr->ret)
                    *(thr->ret) = thr->pop();
            }
            else
            {
                if (thr->ctx->ret)
                    *(thr->ctx->ret) = val;
                thr->iid = thr->ctx->value;
                thr->restore_context();
                assert(thr->ctx != nullptr);
            }
        }
    };
};
