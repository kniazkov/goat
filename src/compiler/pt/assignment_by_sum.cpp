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

#include "assignment_by_sum.h"

namespace g0at
{
    namespace pt
    {
        assignment_by_sum::assignment_by_sum(lib::pointer<position> _pos, lib::pointer<expression> _left, lib::pointer<expression> _right)
            : binary(_pos, _left, _right)
        {
        }

        void assignment_by_sum::accept(node_visitor *visitor)
        {
            visitor->visit(this);
        }

        assignment_by_sum *assignment_by_sum::to_assignment_by_sum()
        {
            return this;
        }
    };
};
