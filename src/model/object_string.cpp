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

#include "object_string.h"
#include "thread.h"
#include "lib/utils.h"
#include <assert.h>
#include <sstream>

namespace g0at
{
    namespace model
    {
        object_string::object_string(object_pool *pool, std::wstring _data)
            : object(pool), data(_data), id(-1)
        {
            proto.push_back(pool->get_string_proto_instance());
        }

        object_string::object_string(object_pool *pool, std::wstring _data, int _id)
            : object(pool), data(_data), id(_id)
        {
        }

        object_type object_string::get_type() const
        {
            return object_type::string;
        }

        object_string *object_string::to_object_string()
        {
            return this;
        }

        bool object_string::less(const object *obj) const
        {
            assert(obj->get_type() == object_type::string);
            const object_string *str = static_cast<const object_string*>(obj);
            if (id < 0 || str->id < 0)
                return data < str->data;
            else
                return id < str->id;
        }

        std::wstring object_string::to_string() const
        {
            return data;
        }

        std::wstring object_string::to_string_notation() const
        {
            std::wstringstream wss;
            wss << L'\"' << lib::escape_special_chars(data) << L'\"';
            return wss.str();
        }

        void object_string::op_add(thread *thr)
        {
            thr->pop();
            variable right = thr->pop();
            std::wstring str = data + right.to_string();
            variable result;
            result.set_object(new object_string(thr->o_pool, str));
            thr->push(result);
        }

        /*
            Prototype
        */

        object_string_proto::object_string_proto(object_pool *pool)
            : object(pool)
        {
        }
    };
};
