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

#pragma once

#include "token_with_list.h"
#include "expression.h"
#include "brackets_pair.h"

namespace g0at
{
    namespace ast
    {
        class function_call : public expression, public token_with_list
        {
        public:
            function_call(lib::pointer<expression> _func_object,  brackets_pair *_args);
            void accept(token_visitor *visitor) override;
            function_call *to_function_call() override;

            lib::pointer<expression> get_func_object() { return func_object; }
            token_list *get_args_list() { return &args; }

        protected:
            lib::pointer<expression> func_object;
            token_list args;
        };
    };
};
