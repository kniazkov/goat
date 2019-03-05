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

#include "object_pool.h"
#include "object.h"
#include <assert.h>
#include "object_void.h"
#include "object_undefined.h"
#include "object_null.h"
#include "object_string.h"
#include "object_integer.h"
#include "object_function.h"
#include "object_boolean.h"
#include "object_real.h"

namespace g0at
{
    namespace model
    {
        object_pool::object_pool()
        {
            generic_proto_instance = new generic_proto(this);
            void_instance = new object_void(this);
            undefined_instance = new object_undefined(this);
            null_instance = new object_null(this);
            string_proto_instance = new object_string_proto(this);
            integer_proto_instance = new object_integer_proto(this);
            function_proto_instance = new object_function_proto(this);
            boolean_proto_instance = new object_boolean_proto(this);
            real_proto_instance = new object_real_proto(this);
        }

        generic_object * object_pool::create_generic_object()
        {
            generic_object *obj;
            generic_objects.alive++;
            if (generic_objects.dead.count > 0)
            {
                obj = static_cast<generic_object*>(generic_objects.dead.remove());
                obj->reinit(this);
            }
            else
            {
                obj = new generic_object(this);
            }
            return obj;
        }

        object_string * object_pool::create_object_string(std::wstring data)
        {
            object_string *obj;
            strings.alive++;
            if (strings.dead.count > 0)
            {
                obj = static_cast<object_string*>(strings.dead.remove());
                obj->reinit(data);
            }
            else
            {
                obj = new object_string(this, data);
            }
            return obj;
        }

        object_string * object_pool::create_object_string(std::wstring data, int id)
        {
            object_string *obj;
            strings.alive++;
            if (strings.dead.count > 0)
            {
                obj = static_cast<object_string*>(strings.dead.remove());
                obj->reinit(data, id);
            }
            else
            {
                obj = new object_string(this, data, id);
            }
            return obj;
        }
    };
};
