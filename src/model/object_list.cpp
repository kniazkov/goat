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

namespace g0at
{
    namespace model
    {
        object_list::object_list()
            : first(nullptr), last(nullptr)
        {
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
    };
};
