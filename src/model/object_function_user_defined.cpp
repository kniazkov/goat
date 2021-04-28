/*

Copyright (C) 2017-2021 Ivan Kniazkov

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

#include "object_function_user_defined.h"
#include "lib/assert.h"

namespace g0at
{
    namespace model
    {
        object_function_user_defined::object_function_user_defined(object_pool *pool, code::iid_t _first_iid, context *_proto_ctx)
            : object_function(pool), first_iid(_first_iid), proto_ctx(_proto_ctx)
        {
            assert(_first_iid.valid());
        }

        void object_function_user_defined::call(thread *thr, int arg_count, call_mode mode)
        {
            // prepare a new context
            context *ctx;
            object *this_ptr = nullptr;
            switch(mode)
            {
                case call_mode::as_function:
                    ctx = thr->pool->create_context(proto_ctx, thr->ctx);
                    break;
                case call_mode::as_method:
                case call_mode::as_constructor:
                {
                    this_ptr = thr->pop().get_object();
                    ctx = thr->pool->create_context(this_ptr, proto_ctx, thr->ctx);
                    break;
                }
            }
            ctx->address[0] = thr->iid;
            ctx->address_type = context_address_type::ret_address;
            int decl_arg_count = (int)arg_names.size();
            for (int i = 0; i < decl_arg_count; i++)
            {
                object *key = arg_names[i];
                if (i < arg_count)
                {
                    variable arg = thr->pop().deref();
                    ctx->add_object(key, arg);
                }
                else
                    ctx->add_object(key, thr->pool->get_undefined_instance());
            }
            if (arg_count > decl_arg_count)
            {
                thr->pop(arg_count - decl_arg_count);
            }

            if (mode == call_mode::as_constructor)
            {
                // constructor always returns 'this' pointer
                variable tmp;
                tmp.set_object(this_ptr);
                thr->push(tmp);
            }
            else
            {
                // prepare cell to place result (return value)
                ctx->ret = thr->push_undefined();
            }
            
            // change context
            thr->set_context(ctx);
            thr->iid = first_iid;
        }

        void object_function_user_defined::trace()
        {
            proto_ctx->mark();
            for (object *arg : arg_names)
            {
                arg->mark();
            }
        }

        void object_function_user_defined::trace_parallel(object_pool *pool)
        {
            proto_ctx->mark_parallel(pool);
            for (object *arg : arg_names)
            {
                arg->mark_parallel(pool);
            }
        }

        goat_value * object_function_user_defined::get_value(const goat_allocator *allocator)
        {
            return create_goat_function(allocator, this);
        }
    };
};
