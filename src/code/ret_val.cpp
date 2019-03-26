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

#include "ret_val.h"
#include <assert.h>

namespace g0at
{
    namespace code
    {
        void ret_val::accept(instruction_visitor *visitor)
        {
            visitor->visit(this);
        }

        void ret_val::exec(model::thread *thr)
        {
            while(thr->ctx && thr->ctx->value_type != model::context_value_type::ret_address)
            {
                thr->ctx = thr->ctx->prev;
            }

            if (!thr->ctx)
            {
                thr->state = model::thread_state::zombie;
                *(thr->ret) = thr->pop();
            }
            else
            {
                *(thr->ctx->ret) = thr->pop();
                thr->iid = thr->ctx->value;
                thr->ctx = thr->ctx->prev;
                assert(thr->ctx != nullptr);
            }
        }
    };
};
