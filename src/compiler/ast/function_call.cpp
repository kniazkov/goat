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

#include "function_call.h"
#include "lib/assert.h"

namespace g0at
{
    namespace ast
    {
        function_call::function_call(lib::pointer<expression> _func_object,  brackets_pair *_args)
        {
            assert(_args->get_symbol() == L'(');
            func_object = _func_object;
            raw.swap(_args->get_raw_list());
            pos = _args->get_position();
        }

        void function_call::accept(token_visitor *visitor)
        {
            visitor->visit(this);
        }

        function_call *function_call::to_function_call()
        {
            return this;
        }
    };
};
