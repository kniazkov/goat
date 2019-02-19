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
#include <iostream>

namespace g0at
{
    namespace model
    {
        namespace built_in
        {
            class print : public object_function_built_in
            {
            public:
                print(object_list *_list)
                    : object_function_built_in(_list)
                {
                }
                
                void call(thread *thr, int arg_count) override
                {
                    std::cout << global::char_encoder->encode(thr->peek().to_string());
                }
            };

            object *context_factory::create_function_print()
            {
                return new print(list);
            }
        };
    };
};
