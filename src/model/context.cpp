/*

Copyright (C) 2017-2020 Ivan Kniazkov

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

#include "settings.h"
#include "context.h"
#include "lib/assert.h"
#ifdef GC_DEBUG
#include "vm/vm_exception.h"
#endif



namespace g0at
{
    namespace model
    {
        context::context(object_pool *pool)
            : object(pool), prev(nullptr), this_ptr(nullptr), address_type(context_address_type::none),
              ret(nullptr), stack_size(-1), lock(-1), debug_level(0), pos(nullptr)
        {
        }

        context::context(object_pool *pool, context *proto)
            : object(pool, proto), prev(proto), this_ptr(nullptr), address_type(context_address_type::none),
              ret(nullptr), stack_size(-1), lock(-1), debug_level(proto->debug_level), pos(proto->pos)
        {
            assert(proto != nullptr);
        }

        context::context(object_pool *pool, context *proto, context *parent)
            : object(pool, proto), prev(parent), this_ptr(nullptr), address_type(context_address_type::none),
              ret(nullptr), stack_size(-1), lock(-1), debug_level(parent->debug_level), pos(parent->pos)
        {
            assert(proto != nullptr);
        }

        context::context(object_pool *pool, object *_this_ptr, context *proto, context *parent)
            : object(pool, _this_ptr, proto), prev(parent), this_ptr(_this_ptr), address_type(context_address_type::none),
              ret(nullptr), stack_size(-1), lock(-1), debug_level(parent->debug_level), pos(parent->pos)
        {
            assert(this_ptr != nullptr);
            assert(proto != nullptr);
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
            debug_level = proto->debug_level;
        }

        void context::reinit(context *proto, context *parent)
        {
            assert(!this->proto && !topology);
            assert(proto != nullptr);
            assert(parent != nullptr);
            this->proto = proto;
            prev = parent;
            debug_level = parent->debug_level;
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
            debug_level = parent->debug_level;
        }

#ifndef GC_DEBUG
        void context::kill(object_pool *pool)
        {
            if (pool->contexts.destroy_or_cache(this, pool))
            {
                objects.clear();
                proto = nullptr;
                topology.reset();
                immutable = false;
                prev = nullptr;
                this_ptr = nullptr;
                for (int i = 0; i < addresses_count; i++)
                    address[i] = code::iid_t();
                address_type = context_address_type::none;
                ret = nullptr;
                debug_level = -1;
            }
        }
#endif

        void context::trace()
        {
            if (prev)
            {
#ifdef GC_DEBUG
                if (prev->is_died())
                    throw vm::use_of_a_dead_object(prev->to_string());
#endif
                prev->mark();
            }

            if (ret)
            {
#ifdef GC_DEBUG
                object *obj = ret->get_object();
                if (obj && obj->is_died())
                    throw vm::use_of_a_dead_object(obj->to_string());
#endif
                ret->mark();
            }
        }

        void context::trace_parallel(object_pool *pool)
        {
            if (prev)
            {
#ifdef GC_DEBUG
                if (prev->is_died())
                    throw vm::use_of_a_dead_object(prev->to_string());
#endif
                prev->mark_parallel(pool);
            }

            if (ret)
            {
#ifdef GC_DEBUG
                object *obj = ret->get_object();
                if (obj && obj->is_died())
                    throw vm::use_of_a_dead_object(obj->to_string());
#endif
                ret->mark_parallel(pool);
            }
        }
    };
};
