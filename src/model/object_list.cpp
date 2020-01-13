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

#include "object_list.h"
#include "object.h"
#include <algorithm>

namespace g0at
{
    namespace model
    {
        object_list::object_list()
            : first(nullptr), last(nullptr), count(0)
        {
        }

        void object_list::swap(object_list &other)
        {
            std::swap(first, other.first);
            std::swap(last, other.last);
            std::swap(count, other.count);
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
            count++;
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
            count--;
        }

        object * object_list::remove()
        {
            object *item = last;
            if (item)
            {
                if (item->prev)
                    item->prev->next = nullptr;
                else
                    first = nullptr;

                last = item->prev;
                count--;
            }
            return item;
        }

        object * object_list::remove_first()
        {
            object *item = first;
            if (item)
            {
                if (item->next)
                    item->next->prev = nullptr;
                else
                    last = nullptr;

                first = item->next;
                count--;
            }
            return item;
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
            first = nullptr;
            last = nullptr;
        }
    };
};
