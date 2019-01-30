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

#include "node.h"

namespace g0at
{
    namespace pt
    {
        node::node(lib::pointer<position> _pos)
            : pos(_pos)
        {
        }

        node::~node()
        {
        }

        expression *node::to_expression()
        {
            return nullptr;
        }

        statement *node::to_statement()
        {
            return nullptr;
        }

        static_string *node::to_static_string()
        {
            return nullptr;
        }

        function_call *node::to_function_call()
        {
            return nullptr;
        }

        statement_expression *node::to_statement_expression()
        {
            return nullptr;
        }

        function *node::to_function()
        {
            return nullptr;
        }

        binary *node::to_binary()
        {
            return nullptr;
        }

        addition *node::to_addition()
        {
            return nullptr;
        }

        integer *node::to_integer()
        {
            return nullptr;
        }
    }
};
