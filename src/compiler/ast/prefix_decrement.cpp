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

#include "prefix_decrement.h"

namespace g0at
{
    namespace ast
    {
        prefix_decrement::prefix_decrement(lib::pointer<expression> _right, token_operator *_oper)
            : unary_prefix(_right, _oper)
        {
        }

        void prefix_decrement::accept(token_visitor *visitor)
        {
            visitor->visit(this);
        }

        prefix_decrement *prefix_decrement::to_prefix_decrement()
        {
            return this;
        }
    };
};
