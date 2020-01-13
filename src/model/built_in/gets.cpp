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
#include "model/object_string.h"
#include "global/global.h"
#include <cstdio>
#include <sstream>
#include <string>

namespace g0at
{
    namespace model
    {
        namespace built_in
        {
            class gets : public object_function_built_in
            {
            public:
                gets(object_pool *_pool)
                    : object_function_built_in(_pool)
                {
                }
                
                void call(thread *thr, int arg_count, call_mode mode) override
                {
                    if (mode == call_mode::as_method)
                        thr->pop();
                    thr->pop(arg_count);
                    std::wstringstream wss;
                    int ch = std::getchar();
                    while (ch != EOF && ch != '\n') {
                        if (ch != '\r')
                            wss << (wchar_t)ch;
                        ch = std::getchar();
                    }
                    variable result;
                    result.set_object(thr->pool->create_object_string(wss.str()));
                    thr->push(result);
                }
            };

            object *context_factory::create_function_gets()
            {
                return new gets(pool);
            }
        };
    };
};
