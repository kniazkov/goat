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

#include "statement_if.h"

namespace g0at
{
    namespace ast
    {
        statement_if::statement_if(keyword_if *_kw, lib::pointer<expression> _expr, lib::pointer<statement> _stmt_if)
            : expr(_expr), stmt_if(_stmt_if), stmt_else(nullptr)
        {
            pos = _kw->get_position();
        }

        statement_if::statement_if(keyword_if *_kw, lib::pointer<expression> _expr, lib::pointer<statement> _stmt_if, lib::pointer<statement> _stmt_else)
            : expr(_expr), stmt_if(_stmt_if), stmt_else(_stmt_else)
        {
            pos = _kw->get_position();
        }

        void statement_if::accept(token_visitor *visitor)
        {
            visitor->visit(this);
        }

        statement_if *statement_if::to_statement_if()
        {
            return this;
        }
    };
};
