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

#include "index_access.h"
#include "lib/assert.h"

namespace g0at
{
    namespace ast
    {
        index_access::index_access(lib::pointer<expression> _expr,  brackets_pair *_args)
        {
            assert(_args->get_symbol() == L'[');
            expr = _expr;
            raw.swap(_args->get_raw_list());
            frag.begin = _expr->get_fragment().begin;
            frag.end = _args->get_fragment().end;
        }

        void index_access::accept(token_visitor *visitor)
        {
            visitor->visit(this);
        }

        index_access *index_access::to_index_access()
        {
            return this;
        }
    };
};
