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
#include "lib/utils.h"

namespace g0at
{
    namespace model
    {
        namespace built_in
        {
            class clock : public object_function_built_in
            {
            public:
                clock(object_pool *_pool)
                    : object_function_built_in(_pool)
                {
                }
                
                void call(thread *thr, int arg_count, call_mode mode) override
                {
                    if (mode == call_mode::as_method)
                        thr->pop();
                    thr->pop(arg_count);
                    variable tmp;
                    tmp.set_integer(lib::get_time_ns());
                    thr->push(tmp);
                    return;
                }
            };

            object *context_factory::create_function_clock()
            {
                return new clock(pool);
            }
        };
    };
};
