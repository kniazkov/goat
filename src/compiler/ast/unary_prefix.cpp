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

#include "unary_prefix.h"
#include "token_operator.h"

namespace g0at
{
    namespace ast
    {
        unary_prefix::unary_prefix(lib::pointer<expression> _right, token_operator *_oper)
            : right(_right)
        {
            frag.begin = _oper->get_fragment().begin;
            frag.end = _right->get_fragment().end;
        }

        unary_prefix *unary_prefix::to_unary_prefix()
        {
            return this;
        }
    };
};
