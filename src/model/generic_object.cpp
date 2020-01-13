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

#include "object.h"
#include "object_pool.h"
#include "resource/strings.h"
#include "lib/assert.h"

namespace g0at
{
    namespace model
    {
        generic_object::generic_object(object_pool *pool)
            : object(pool)
        {
        }

#ifndef GC_DEBUG
        void generic_object::kill(object_pool *pool)
        {
            if (pool->generic_objects.destroy_or_cache(this, pool))
            {
                objects.clear();
                proto = nullptr;
                topology.reset();
                immutable = false;
            }
        }
#endif

        void generic_object::reinit(object_pool *pool)
        {
            assert(!proto && !topology);
            proto = pool->get_generic_proto_instance();
        }

        void generic_object::m_clone(thread *thr, int arg_count)
        {
            // find and call own 'clone()' method
            find_and_vcall(thr, arg_count, resource::str_clone);
        }
    };
};
