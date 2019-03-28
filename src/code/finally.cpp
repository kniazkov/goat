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

#include "finally.h"
#include "lib/assert.h"

namespace g0at
{
    namespace code
    {
        _finally::_finally(int _iid)
            : iid(_iid)
        {
        }

        void _finally::accept(instruction_visitor *visitor)
        {
            visitor->visit(this);
        }

        void _finally::exec(model::thread *thr)
        {
            assert(iid >= 0);
            model::context *ctx = thr->pool->create_context(thr->ctx);
            ctx->value = iid;
            ctx->value_type = model::context_value_type::fin_address;
            model::context *parent = thr->ctx;
            while(parent && parent->value_type != model::context_value_type::ret_address)
            {
                parent = parent->prev;
            }
            if (parent)
            {
                assert(parent->ret != nullptr);
                ctx->ret = parent->ret;
            }
            thr->set_context(ctx);
        }
    };
};
