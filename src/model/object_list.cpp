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

#include "object_list.h"
#include "object.h"
#include "object_string.h"
#include "object_integer.h"
#include "object_function.h"
#include <assert.h>

namespace g0at
{
    namespace model
    {
        object_list::object_list()
        {
            first = nullptr;
            last = nullptr;
            generic_proto_instance = nullptr;
            string_proto_instance = nullptr;
            integer_proto_instance = nullptr;
            function_proto_instance = nullptr;

            init();
        }

        void object_list::add(object *item)
        {
            item->prev = last;
            item->next = nullptr;

            if (last)
                last->next = item;
            else
                first = item;
            last = item;
        }

        void object_list::remove(object *item)
        {
            if (item->prev)
                item->prev->next = item->next;
            else 
                first = item->next;

            if (item->next)
                item->next->prev = item->prev;
            else
                last = item->prev;
        }

        void object_list::destroy_all()
        {
            object *obj = first;
            while(obj)
            {
                object *next = obj->next;
                delete obj;
                obj = next;
            }
        }

        object *object_list::get_generic_proto_instance()
        {
            assert(generic_proto_instance != nullptr);
            return generic_proto_instance;
        }

        object *object_list::get_string_proto_instance()
        {
            assert(string_proto_instance != nullptr);
            return string_proto_instance;
        }

        object *object_list::get_integer_proto_instance()
        {
            assert(integer_proto_instance != nullptr);
            return integer_proto_instance;
        }

        object *object_list::get_function_proto_instance()
        {
            assert(function_proto_instance != nullptr);
            return function_proto_instance;
        }

        void object_list::init()
        {
            generic_proto_instance = new generic_proto(this);
            string_proto_instance = new object_string_proto(this);
            integer_proto_instance = new object_integer_proto(this);
            function_proto_instance = new object_function_proto(this);
        }
    };
};
