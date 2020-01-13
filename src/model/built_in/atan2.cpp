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

#include "context_factory.h"
#include "model/object_function_built_in.h"
#include "model/object_exception.h"
#include "global/global.h"
#include <cmath>

namespace g0at
{
    namespace model
    {
        namespace built_in
        {
            class atan2 : public object_function_built_in
            {
            public:
                atan2(object_pool *_pool)
                    : object_function_built_in(_pool)
                {
                }
                
                void call(thread *thr, int arg_count, call_mode mode) override
                {
                    if (arg_count > 1)
                    {
                        if (mode != call_mode::as_function)
                            thr->pop();
                        variable arg_1 = thr->peek();
                        variable arg_2 = thr->peek(1);
                        thr->pop(arg_count);
                        double val_arg_1,
                            val_arg_2;
                        if (arg_1.get_real(&val_arg_1) && arg_2.get_real(&val_arg_2))
                        {
                            variable tmp;
                            tmp.set_real(std::atan2(val_arg_1, val_arg_2));
                            thr->push(tmp);
                            return;
                        }
                    }
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                }
            };

            object *context_factory::create_function_atan2()
            {
                return new atan2(pool);
            }
        };
    };
};
