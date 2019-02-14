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
                sin(object_list *_list)
                    : object_function_built_in(_list)
                {
                }
                
                void call(thread *thr, int arg_count) override
                {
                    variable &result = thr->peek();
                    variable arg = thr->peek(1);
                    double real_val;
                    if (arg.get_real(&real_val))
                    {
                        result.set_real(std::sin(real_val));
                    }
                }
            };

            object *context_factory::create_function_sin()
            {
                return new sin(list);
            }
        };
    };
};
