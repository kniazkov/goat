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

#include "object_program_arguments.h"
#include "object_string.h"
#include "lib/assert.h"
#include "lib/utils.h"

namespace g0at
{
    namespace model
    {
        /*
            Array
        */
        object_program_arguments::object_program_arguments(object_pool *pool, std::vector<char*> *args)
            : object_array_like(pool, pool->get_array_like_proto_instance())
        {
            if (args)
                for (auto iter = args->begin(); iter != args->end(); iter++)
                {
                    data.push_back(pool->create_object_string(lib::string_to_wstring(*iter)));
                }
        }

        void object_program_arguments::trace() 
        {
            for (object *obj : data)
            {
                obj->mark();
            }
        }

        void object_program_arguments::trace_parallel(object_pool *pool) 
        {
            for (object *obj : data)
            {
                obj->mark_parallel(pool);
            }
        }

        int object_program_arguments::get_length() const
        {
            return (int)data.size();
        }

        variable object_program_arguments::get_item(int idx) const
        {
            variable result;
            assert (idx >= 0 && idx < data.size());
            result.set_object(data[idx]);
            return result;
        }

        variable object_program_arguments::get_item(int idx)
        {
            variable result;
            assert (idx >= 0 && idx < data.size());
            result.set_object(data[idx]);
            return result;
        }
    };
};

