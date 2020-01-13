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

#include "function.h"
#include "expression.h"

namespace g0at
{
    namespace ast
    {
        class declare_function : public expression
        {
        public:
            declare_function(lib::pointer<function> _func);
            void accept(token_visitor *visitor) override;
            declare_function *to_declare_function() override;

            lib::pointer<function> get_func() { return func; }

        protected:
            lib::pointer<function> func;
        };
    };
};
