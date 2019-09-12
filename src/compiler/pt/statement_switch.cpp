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

#include "statement_switch.h"
#include "lib/assert.h"

namespace g0at
{
    namespace pt
    {
        statement_switch::statement_switch(fragment _frag, lib::pointer<expression> _expr)
            : statement(_frag), expr(_expr)
        {
        }

        void statement_switch::accept(node_visitor *visitor)
        {
            visitor->visit(this);
        }

        statement_switch *statement_switch::to_statement_switch()
        {
            return this;
        }

        void statement_switch::set_default_block(lib::pointer<default_block> block)
        {
            assert(def_block == nullptr);
            def_block = block;
        }
    };
};
