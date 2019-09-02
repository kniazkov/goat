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

#include "double_greater.h"
#include "signed_right_shift.h"

namespace g0at
{
    namespace ast
    {
        void double_greater::accept(token_visitor *visitor)
        {
            visitor->visit(this);
        }

        double_greater *double_greater::to_double_greater()
        {
            return this;
        }

        lib::pointer<token> double_greater::create_binary_operation(lib::pointer<expression> left, lib::pointer<expression> right)
        {
            return new signed_right_shift(left, right);
        }
    };
};
