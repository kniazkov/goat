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

#include "object_null.h"
#include "thread.h"
#include "resource/strings.h"

namespace g0at
{
    namespace model
    {
        object_null::object_null(object_pool *pool)
            : object(pool, pool->get_void_instance())
        {
        }

        object_null *object_null::to_object_null()
        {
            return this;
        }

        std::wstring object_null::to_string() const
        {
            return resource::str_null;
        }

        void object_null::op_not(thread *thr)
        {
            thr->pop();
            variable tmp;
            tmp.set_boolean(true);
            thr->push(tmp);
        }

        void object_null::op_bool(thread *thr)
        {
            thr->pop();
            variable tmp;
            tmp.set_boolean(false);
            thr->push(tmp);
        }

        void object_null::m_iterator(thread *thr, int arg_count)
        {
            thr->pop();
            thr->pop(arg_count);
            variable tmp;
            tmp.set_object(thr->pool->get_iterator_proto_instance());
            thr->push(tmp);
        }
    };
};
