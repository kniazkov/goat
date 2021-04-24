/*

Copyright (C) 2021 Ivan Kniazkov

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

#include "object_raw_data.h"
#include "object_exception.h"
#include "resource/strings.h"
#include "thread.h"

namespace g0at
{
    namespace model
    {
        object_raw_data::object_raw_data(object_pool *_pool, void *_raw_data)
            : object(_pool, _pool->get_raw_data_proto_instance()), raw_data(_raw_data)
        {
        }

        object_raw_data *object_raw_data::to_object_raw_data()
        {
            return this;
        }

        std::wstring object_raw_data::to_string() const
        {
            return resource::str_raw;
        }

        /*
            Prototype
        */

        object_raw_data_proto::object_raw_data_proto(object_pool *pool)
            : object(pool)
        {
        }

        void object_raw_data_proto::op_new(thread *thr, int arg_count)
        {
            thr->raise_exception(new object_exception_illegal_operation(thr->pool));
        }
    };
};
