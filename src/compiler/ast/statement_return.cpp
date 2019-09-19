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

#include "statement_return.h"

namespace g0at
{
    namespace ast
    {
        statement_return::statement_return(keyword_return *_kw, lib::pointer<expression> _expr)
            : expr(_expr)
        {
            if (expr)
            {
                frag.begin = _kw->get_fragment().begin;
                frag.end = _expr->get_fragment().end;
            }
            else
            {
                frag = _kw->get_fragment();
            }
        }

        void statement_return::accept(token_visitor *visitor)
        {
            visitor->visit(this);
        }

        statement_return *statement_return::to_statement_return()
        {
            return this;
        }
    };
};
