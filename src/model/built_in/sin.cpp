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
#include <cmath>

namespace g0at
{
    namespace model
    {
        namespace built_in
        {
            class sin : public object_function_built_in
            {
            public:
                sin(object_pool *_pool)
                    : object_function_built_in(_pool)
                {
                }
                
                void call(thread *thr, int arg_count, call_mode mode) override
                {
                    if (arg_count > 0)
                    {
                        if (mode == call_mode::as_method)
                            thr->pop();
                        variable arg = thr->peek();
                        thr->pop(arg_count);
                        double real_val;
                        if (arg.get_real(&real_val))
                        {
                            variable tmp;
                            tmp.set_real(std::sin(real_val));
                            thr->push(tmp);
                            return;
                        }
                    }
                    thr->raise_exception(thr->pool->get_exception_illegal_argument_instance());
                }
            };

            object *context_factory::create_function_sin()
            {
                return new sin(pool);
            }
        };
    };
};
