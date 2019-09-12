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

#include "operator_new.h"
#include "lib/assert.h"

namespace g0at
{
    namespace ast
    {
        operator_new::operator_new(keyword_new *_kw, lib::pointer<expression> _proto)
        {
            proto = _proto;
            frag.begin = _kw->get_fragment().begin;
            frag.end = _proto->get_fragment().end;
        }

        operator_new::operator_new(keyword_new *_kw, lib::pointer<expression> _proto,  brackets_pair *_args)
        {
            assert(_args->get_symbol() == L'(');
            proto = _proto;
            raw.swap(_args->get_raw_list());
            frag.begin = _kw->get_fragment().begin;
            frag.end = _args->get_fragment().end;
        }

        void operator_new::accept(token_visitor *visitor)
        {
            visitor->visit(this);
        }

        operator_new *operator_new::to_operator_new()
        {
            return this;
        }
    };
};
