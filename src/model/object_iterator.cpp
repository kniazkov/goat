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

#include "object_iterator.h"
#include "object_string.h"
#include "object_function_built_in.h"
#include "object_exception.h"
#include "global/global.h"

namespace g0at
{
    namespace model
    {
        object_iterator_proto::object_iterator_proto(object_pool *pool)
            : object(pool)
        {
        }

        class object_iterator_valid : public object_function_built_in
        {
        public:
            object_iterator_valid(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode == call_mode::as_method)
                    thr->pop();
                thr->pop(arg_count);
                variable tmp;
                tmp.set_boolean(false);
                thr->push(tmp);
            }
        };

        class object_iterator_next : public object_function_built_in
        {
        public:
            object_iterator_next(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode == call_mode::as_method)
                    thr->pop();
                thr->pop(arg_count);
                thr->push_undefined();
            }
        };

        void object_iterator_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(L"valid"), new object_iterator_valid(pool));
            add_object(pool->get_static_string(L"next"), new object_iterator_next(pool));
            lock();
        }

        void object_iterator_proto::op_new(thread *thr, int arg_count)
        {
            thr->raise_exception(new object_exception_illegal_operation(thr->pool));
        }
    };
};
