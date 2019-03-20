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

#include "statement_block.h"

namespace g0at
{
    namespace pt
    {
        statement_block::statement_block(lib::pointer<position> _pos)
            : statement(_pos), vars(false)
        {
        }

        void statement_block::accept(node_visitor *visitor)
        {
            visitor->visit(this);
        }

        statement_block *statement_block::to_statement_block()
        {
            return this;
        }

        void statement_block::add_stmt(lib::pointer<statement> stmt)
        {
            code.push_back(stmt);
            if (stmt->to_declare_variable() != nullptr)
            {
                vars = true;
            }
        }
    };
};
