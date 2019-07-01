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

#include "statement_for_in.h"

namespace g0at
{
    namespace pt
    {
        statement_for_in::statement_for_in(lib::pointer<position> _pos, std::wstring _name, bool _declared, lib::pointer<expression> _expr, lib::pointer<statement> _body)
            : statement(_pos), name(_name), declared(_declared), expr(_expr), body(_body)
        {
        }

        void statement_for_in::accept(node_visitor *visitor)
        {
            visitor->visit(this);
        }

        statement_for_in *statement_for_in::to_statement_for_in()
        {
            return this;
        }
    };
};