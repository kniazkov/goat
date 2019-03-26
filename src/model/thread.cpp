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

#include "thread.h"
#include "lib/assert.h"
#include "lib/exception.h"

namespace g0at
{
    namespace model
    {
        class unhandled_runtime_exception : public lib::exception
        {
        public:
            unhandled_runtime_exception(std::wstring str)
                : exception(global::resource->unhandled_exception(str))
            {
            }
        };

        thread::thread(context *_ctx, object_pool *_pool, variable *_ret)
            : next(nullptr), iid(0), state(thread_state::pause), ctx(_ctx), pool(_pool), ret(_ret)
        {
        }

        void thread::mark_all()
        {
            if (ret)
                ret->mark();

            if (ctx)
                ctx->mark();

            data.mark_all();
        }        

        void thread::raise_exception(variable &var)
        {
            while(ctx && ctx->value_type != model::context_value_type::catch_address)
            {
                ctx = ctx->prev;
            }

            if (!ctx)
            {
                throw unhandled_runtime_exception(var.to_string());
            }
            else
            {
                assert(false && "Not implemented"); 
            }
        }
    };
};
