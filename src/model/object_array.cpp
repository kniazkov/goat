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

#include "object_array.h"
#include "thread.h"
#include "lib/utils.h"
#include <assert.h>
#include <sstream>

namespace g0at
{
    namespace model
    {
        object_array::object_array(object_pool *pool)
            : object(pool, pool->get_array_proto_instance())
        {
        }

        void object_array::kill(object_pool *pool)
        {
            if (pool->arrays.destroy_or_cache(this, pool))
            {
                objects.clear();
                vector.clear();
            }
        }
        
        void object_array::reinit()
        {
            assert(vector.empty());
        }

        object_array *object_array::to_object_array()
        {
            return this;
        }

        std::wstring object_array::to_string() const
        {
            std::wstringstream wss;
            wss << L'[';
            for (int i = 0, size = (int)vector.size(); i < size; i++)
            {
                if (i)
                    wss << L',';
                wss << vector[i].to_string_notation();
            }
            wss << L']';
            return wss.str();
        }

        /*
            Prototype
        */

        object_array_proto::object_array_proto(object_pool *pool)
            : object(pool)
        {
        }
    };
};
