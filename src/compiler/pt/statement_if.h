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

#pragma once

#include "statement.h"
#include "expression.h"

namespace g0at
{
    namespace pt
    {
        class statement_if : public statement
        {
        public:
            statement_if(fragment _frag, lib::pointer<expression> _expr, lib::pointer<statement> _stmt_if);
            statement_if(fragment _frag, lib::pointer<expression> _expr, lib::pointer<statement> _stmt_if, lib::pointer<statement> _stmt_else);
            void accept(node_visitor *visitor) override;
            statement_if *to_statement_if() override;

            lib::pointer<expression> get_expression() { return expr; }
            lib::pointer<statement> get_stmt_if() { return stmt_if; }
            lib::pointer<statement> get_stmt_else() { return stmt_else; }

        protected:
            lib::pointer<expression> expr;
            lib::pointer<statement> stmt_if;
            lib::pointer<statement> stmt_else;
        };
    };
};
