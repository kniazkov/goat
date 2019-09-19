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

#pragma once

#include "statement.h"
#include "nonterminal.h"
#include "expression.h"
#include "keyword_while.h"

namespace g0at
{
    namespace ast
    {
        class statement_while : public statement, public nonterminal
        {
        public:
            statement_while(keyword_while *_kw, lib::pointer<expression> _expr, lib::pointer<statement> _stmt);
            void accept(token_visitor *visitor) override;
            statement_while *to_statement_while() override;

            lib::pointer<expression> get_expression() { return expr; }
            lib::pointer<statement> get_statement() { return stmt; }

        protected:
            lib::pointer<expression> expr;
            lib::pointer<statement> stmt;
        };
    };
};