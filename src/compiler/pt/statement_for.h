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
        class statement_for : public statement
        {
        public:
            statement_for(fragment _frag, lib::pointer<statement> _stmt_init, lib::pointer<expression> _condition, lib::pointer<statement> _increment, lib::pointer<statement> _body);
            void accept(node_visitor *visitor) override;
            statement_for *to_statement_for() override;

            lib::pointer<statement> get_stmt_init() { return stmt_init; }
            lib::pointer<expression> get_condition() { return condition; }
            lib::pointer<statement> get_increment() { return increment; }
            lib::pointer<statement> get_body() { return body; };

        protected:
            lib::pointer<statement> stmt_init;
            lib::pointer<expression> condition;
            lib::pointer<statement> increment;
            lib::pointer<statement> body;
        };
    };
};