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
            class atan2 : public object_function_built_in
            {
            public:
                atan2(object_list *_list)
                    : object_function_built_in(_list)
                {
                }
                
                void call(thread *thr) override
                {
                    variable &result = thr->peek();
                    variable arg_1 = thr->peek(1);
                    variable arg_2 = thr->peek(2);
                    double val_arg_1,
                        val_arg_2;
                    if (arg_1.get_real(&val_arg_1) && arg_2.get_real(&val_arg_2))
                    {
                        result.set_real(std::atan2(val_arg_1, val_arg_2));
                    }
                }
            };

            object *context_factory::create_function_atan2()
            {
                return new atan2(list);
            }
        };
    };
};
