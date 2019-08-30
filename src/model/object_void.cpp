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

#include "object_void.h"
#include "object_string.h"
#include "thread.h"
#include "resource/strings.h"

namespace g0at
{
    namespace model
    {
        object_void::object_void(object_pool *pool)
            : object(pool)
        {
        }

        void object_void::init(object_pool *pool)
        {
            add_object(pool->get_static_string(resource::str_oper_exclamation), pool->get_wrap_not_instance());
            add_object(pool->get_static_string(resource::str_oper_double_exclamation), pool->get_wrap_bool_instance());
        }

        object_void *object_void::to_object_void()
        {
            return this;
        }

        std::wstring object_void::to_string() const
        {
            return resource::str_void;
        }

        void object_void::op_not(thread *thr)
        {
            thr->pop();
            variable tmp;
            tmp.set_boolean(true);
            thr->push(tmp);
        }

        void object_void::op_bool(thread *thr)
        {
            thr->pop();
            variable tmp;
            tmp.set_boolean(false);
            thr->push(tmp);
        }

        void object_void::m_iterator(thread *thr, int arg_count)
        {
            thr->pop();
            thr->pop(arg_count);
            variable tmp;
            tmp.set_object(thr->pool->get_iterator_proto_instance());
            thr->push(tmp);
        }
    };
};
