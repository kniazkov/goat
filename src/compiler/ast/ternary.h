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

#include "nonterminal.h"
#include "expression.h"

namespace g0at
{
    namespace ast
    {
        class ternary : public expression, public nonterminal
        {
        public:
            ternary(lib::pointer<expression> _condition, lib::pointer<expression> _expr_true, lib::pointer<expression> _expr_false);
            void accept(token_visitor *visitor) override;
            ternary *to_ternary() override;

            lib::pointer<expression> get_condition() { return condition; }
            lib::pointer<expression> get_expr_true() { return expr_true; }
            lib::pointer<expression> get_expr_false() { return expr_false; }

        protected:
            lib::pointer<expression> condition;
            lib::pointer<expression> expr_true;
            lib::pointer<expression> expr_false;
        };
    };
};