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

#include "statement_for.h"

namespace g0at
{
    namespace pt
    {
        statement_for::statement_for(fragment _frag, lib::pointer<statement> _stmt_init, lib::pointer<expression> _condition, lib::pointer<statement> _increment, lib::pointer<statement> _body)
            : statement(_frag), stmt_init(_stmt_init), condition(_condition), increment(_increment), body(_body)
        {
        }

        void statement_for::accept(node_visitor *visitor)
        {
            visitor->visit(this);
        }

        statement_for *statement_for::to_statement_for()
        {
            return this;
        }
    };
};
