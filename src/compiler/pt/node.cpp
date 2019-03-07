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

        variable *node::to_variable()
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

        subtraction *node::to_subtraction()
        {
            return nullptr;
        }

        unary_prefix *node::to_unary_prefix()
        {
            return nullptr;
        }

        negation *node::to_negation()
        {
            return nullptr;
        }

        value_void *node::to_value_void()
        {
            return nullptr;
        }

        value_undefined *node::to_value_undefined()
        {
            return nullptr;
        }

        value_null *node::to_value_null()
        {
            return nullptr;
        }

        declare_variable *node::to_declare_variable()
        {
            return nullptr;
        }

        assignment *node::to_assignment()
        {
            return nullptr;
        }

        real *node::to_real()
        {
            return nullptr;
        }

        declare_function *node::to_declare_function()
        {
            return nullptr;
        }

        statement_return *node::to_statement_return()
        {
            return nullptr;
        }

        node_object *node::to_node_object()
        {
            return nullptr;
        }

        property *node::to_property()
        {
            return nullptr;
        }

        value_true *node::to_value_true()
        {
            return nullptr;
        }

        value_false *node::to_value_false()
        {
            return nullptr;
        }

        is_equal_to *node::to_is_equal_to()
        {
            return nullptr;
        }
        
        is_not_equal_to *node::to_is_not_equal_to()
        {
            return nullptr;
        }
        
        statement_while *node::to_statement_while()
        {
            return nullptr;
        }
        
        method_call *node::to_method_call()
        {
            return nullptr;
        }
   }
};
