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

#include "scope_builder.h"
#include "compiler/pt/function.h"

namespace g0at
{
    namespace analyzer
    {
        scope_builder::scope_builder(lib::pointer<pt::scope> _s0)
            : s0(_s0)
        {
        }

        void scope_builder::visit(pt::function *ref)
        {
            lib::pointer<pt::scope> s1 = new pt::scope(s0);
            ref->set_scope(s1);
            scope_builder b(s1);
            for (int i = 0, code_size = ref->get_code_size(); i < code_size; i++)
            {
                ref->get_stmt(i)->accept(&b);
            }
        }
    };
};
