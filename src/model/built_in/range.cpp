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

#include "context_factory.h"
#include "model/object_function_built_in.h"
#include "global/global.h"

namespace g0at
{
    namespace model
    {
        namespace built_in
        {
            class range_iterator : public object
            {
            public:
                range_iterator(object_pool *pool, int64_t _begin, int64_t _end, int64_t _step)
                    : object(pool, pool->get_iterator_proto_instance()),
                      current(_begin), end(_end), step(_step)
                {
                }

                void m_valid(thread *thr, int arg_count) override
                {
                    thr->pop();
                    thr->pop(arg_count);
                    variable tmp;
                    tmp.set_boolean(current < end);
                    thr->push(tmp);
                }

                void m_next(thread *thr, int arg_count) override
                {
                    thr->pop();
                    thr->pop(arg_count);
                    if (current < end)
                    {
                        variable tmp;
                        tmp.set_integer(current);
                        current += step;
                        thr->push(tmp);
                    }
                    else
                        thr->push_undefined();
                }

            protected:
                int64_t current;
                int64_t end;
                int64_t step;
            };

            class range_object : public object
            {
            public:
                range_object(object_pool *pool, int64_t _begin, int64_t _end, int64_t _step)
                    : object(pool),
                      begin(_begin), end(_end), step(_step)
                {
                }

                void m_iterator(thread *thr, int arg_count) override
                {
                    thr->pop();
                    thr->pop(arg_count);
                    variable tmp;
                    tmp.set_object(new range_iterator(thr->pool, begin, end, step));
                    thr->push(tmp);
                }

            protected:
                int64_t begin;
                int64_t end;
                int64_t step;
            };

            class range : public object_function_built_in
            {
            public:
                range(object_pool *_pool)
                    : object_function_built_in(_pool)
                {
                }
                
                void call(thread *thr, int arg_count, call_mode mode) override
                {
                    do
                    {
                        if (arg_count < 1)
                            break;
                        if (mode == call_mode::as_method)
                            thr->pop();
                        int64_t begin;
                        if (!thr->peek(0).get_integer(&begin))
                            break;
                        variable result;
                        if (arg_count > 1)
                        {
                            int64_t end;
                            if (!thr->peek(1).get_integer(&end))
                                break;
                            if (arg_count > 2)
                            {
                                int64_t step;
                                if (!thr->peek(2).get_integer(&step))
                                    break;
                                result.set_object(new range_object(thr->pool, begin, end, step));
                            }
                            else
                                result.set_object(new range_object(thr->pool, begin, end, 1));
                        }
                        else
                            result.set_object(new range_object(thr->pool, 0, begin, 1));
                        thr->pop(arg_count);
                        thr->push(result);
                        return;    
                    } while (false);
                    thr->raise_exception(thr->pool->get_exception_illegal_argument_instance());
                }
            };

            object *context_factory::create_function_range()
            {
                return new range(pool);
            }
        };
    };
};
