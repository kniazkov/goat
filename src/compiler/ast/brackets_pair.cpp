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

#include "brackets_pair.h"
#include <assert.h>

namespace g0at
{
    namespace ast
    {
        brackets_pair::brackets_pair(bracket *open_bracket)
        {
            assert(open_bracket->is_closed() == false);
            symbol = open_bracket->get_symbol();
            inverse_symbol = open_bracket->get_inverse_symbol();
            pos = open_bracket->get_position();
        }

        void brackets_pair::accept(token_visitor *visitor)
        {
            visitor->visit(this);
        }

        brackets_pair *brackets_pair::to_brackets_pair()
        {
            return this;
        }
    };
};
