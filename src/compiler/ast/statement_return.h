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
#include "nonterminal.h"
#include "expression.h"
#include "keyword_return.h"

namespace g0at
{
    namespace ast
    {
        class statement_return : public statement, public nonterminal
        {
        public:
            statement_return(keyword_return *_kw, lib::pointer<expression> _expr);
            void accept(token_visitor *visitor) override;
            statement_return *to_statement_return() override;

            lib::pointer<expression> get_expression() { return expr; }

        protected:
            lib::pointer<expression> expr;
        };
    };
};