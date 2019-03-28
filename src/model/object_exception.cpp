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

#include "object_exception.h"
#include "object_string.h"
#include "global/global.h"

namespace g0at
{
    namespace model
    {
        object_exception::object_exception(object_pool *pool)
            : object(pool, pool->get_exception_proto_instance())
        {
        }

        object_exception *object_exception::to_object_exception()
        {
            return this;
        }

        std::wstring object_exception::to_string() const
        {
            return global::resource->unknown_exception();
        }

        std::wstring object_exception::to_string_notation() const
        {
            return L"exception";
        }

        /*
            Prototype
        */

        object_exception_proto::object_exception_proto(object_pool *pool)
            : object(pool)
        {
        }

        void object_exception_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(L"IllegalArgument"), pool->get_exception_illegal_argument_instance());
        }

        /*
            IllegalArgument
        */
        object_exception_illegal_argument::object_exception_illegal_argument(object_pool *pool)
            : object_exception(pool)
        {
        }

        std::wstring object_exception_illegal_argument::to_string() const
        {
            return global::resource->illegal_argument();
        }
    };
};
