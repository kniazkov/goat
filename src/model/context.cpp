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

#include "context.h"
#include "lib/assert.h"

namespace g0at
{
    namespace model
    {
        context::context(object_pool *pool)
            : object(pool), prev(nullptr), this_ptr(nullptr), value(0), value_type(context_value_type::none), ret(nullptr), stack_size(-1)
        {
        }

        context::context(object_pool *pool, context *proto)
            : object(pool, proto), prev(proto), this_ptr(nullptr), value_type(context_value_type::none), ret(nullptr), stack_size(-1)
        {
            assert(proto != nullptr);
        }

        context::context(object_pool *pool, context *proto, context *parent)
            : object(pool, proto), prev(parent), this_ptr(nullptr), value_type(context_value_type::none), ret(nullptr), stack_size(-1)
        {
            assert(proto != nullptr);
            assert(parent != nullptr);
        }

        context::context(object_pool *pool, object *_this_ptr, context *proto, context *parent)
            : object(pool, _this_ptr, proto), prev(parent), this_ptr(_this_ptr), value_type(context_value_type::none), ret(nullptr), stack_size(-1)
        {
            assert(this_ptr != nullptr);
            assert(proto != nullptr);
            assert(parent != nullptr);
        }

        void context::reinit(object_pool *pool)
        {
            assert(!proto && !topology);
            proto = pool->get_generic_proto_instance();
        }

        void context::reinit(context *proto)
        {
            assert(!this->proto && !topology);
            assert(proto != nullptr);
            this->proto = proto;
            prev = proto;
        }

        void context::reinit(context *proto, context *parent)
        {
            assert(!this->proto && !topology);
            assert(proto != nullptr);
            assert(parent != nullptr);
            this->proto = proto;
            prev = parent;
        }

        void context::reinit(object *this_ptr, context *proto, context *parent)
        {
            assert(!this->proto && !topology);
            assert(this_ptr != nullptr);
            assert(proto != nullptr);
            assert(parent != nullptr);
            this->this_ptr = this_ptr;
            topology = new topology_descriptor();
            topology->proto.init(2);
            topology->proto[0] = this_ptr;
            topology->proto[1] = proto;
            topology->build();
            prev = parent;
        }

        void context::kill(object_pool *pool)
        {
            if (pool->contexts.destroy_or_cache(this, pool))
            {
                objects.clear();
                proto = nullptr;
                topology.reset();
                prev = nullptr;
                this_ptr = nullptr;
                value = 0;
                value_type = context_value_type::none;
                ret = nullptr;
            }
        }

        void context::trace()
        {
            if (prev)
                prev->mark();

            if (ret)
                ret->mark();
        }
    };
};
