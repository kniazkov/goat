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

#include "object_integer.h"
#include "thread.h"
#include <assert.h>

namespace g0at
{
    namespace model
    {
        object_integer::object_integer(object_list *list, int64_t _value)
            : object(list), value(_value)
        {
        }

        object_type object_integer::get_type() const
        {
            return INTEGER;
        }

        object_integer *object_integer::to_object_integer()
        {
            return this;
        }

        bool object_integer::less(const object *obj) const
        {
            assert(obj->get_type() == INTEGER);
            const object_integer *obj_int = static_cast<const object_integer*>(obj);
            return value < obj_int->value;
        }

        std::wstring object_integer::to_string() const
        {
            return std::to_wstring(value);
        }

        bool object_integer::get_integer(int64_t *pval)
        {
            *pval = value;
            return true;
        }

        void object_integer::op_add(thread *thr)
        {
            thr->pop();
            variable right = thr->pop();
            int64_t right_value;
            bool right_is_integer = right.get_integer(&right_value);
            assert(right_is_integer);
            variable result;
            result.set_object(new object_integer(thr->o_list, value + right_value));
            thr->push(result);
        }
    };
};
