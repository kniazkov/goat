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

#include "custom_operator.h"

namespace g0at
{
    namespace ast
    {
        custom_operator::custom_operator(std::wstring _oper)
            : oper(_oper)
        {
        }

        void custom_operator::accept(token_visitor *visitor)
        {
            visitor->visit(this);
        }

        custom_operator *custom_operator::to_custom_operator()
        {
            return this;
        }

        lib::pointer<token> custom_operator::create_binary_operation(lib::pointer<expression> left, lib::pointer<expression> right)
        {
            return nullptr; // TODO: new custom_binary_operation(left, right);
        }

        lib::pointer<token> custom_operator::create_unary_prefix_operation(lib::pointer<expression> right)
        {
            return nullptr; // TODO: new custom_binary_operation(left, right);
        }
    };
};
