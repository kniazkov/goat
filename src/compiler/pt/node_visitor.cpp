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

#include "node_visitor.h"

namespace g0at
{
    namespace pt
    {
        node_visitor::node_visitor()
        {
        }

        node_visitor::~node_visitor()
        {
        }

        void node_visitor::visit(variable *ref)
        {
        }

        void node_visitor::visit(static_string *ref)
        {
        }

        void node_visitor::visit(function_call *ref)
        {
        }

        void node_visitor::visit(statement_expression *ref)
        {
        }

        void node_visitor::visit(function *ref)
        {
        }

        void node_visitor::visit(addition *ref)
        {
        }

        void node_visitor::visit(integer *ref)
        {
        }

        void node_visitor::visit(subtraction *ref)
        {
        }

        void node_visitor::visit(negation *ref)
        {
        }

        void node_visitor::visit(value_void *ref)
        {
        }

        void node_visitor::visit(value_undefined *ref)
        {
        }

        void node_visitor::visit(value_null *ref)
        {
        }

        void node_visitor::visit(declare_variable *ref)
        {
        }

        void node_visitor::visit(assignment *ref)
        {
        }

        void node_visitor::visit(real *ref)
        {
        }

        void node_visitor::visit(declare_function *ref)
        {
        }

        void node_visitor::visit(statement_return *ref)
        {
        }

        void node_visitor::visit(node_object *ref)
        {
        }

        void node_visitor::visit(property *ref)
        {
        }

        void node_visitor::visit(value_true *ref)
        {
        }

        void node_visitor::visit(value_false *ref)
        {
        }

        void node_visitor::visit(is_equal_to *ref)
        {
        }

        void node_visitor::visit(is_not_equal_to *ref)
        {
        }

        void node_visitor::visit(statement_while *ref)
        {
        }

        void node_visitor::visit(method_call *ref)
        {
        }

        void node_visitor::visit(this_ptr *ref)
        {
        }

        void node_visitor::visit(node_array *ref)
        {
        }

        void node_visitor::visit(statement_block *ref)
        {
        }

        void node_visitor::visit(statement_if *ref)
        {
        }

        void node_visitor::visit(statement_throw *ref)
        {
        }

        void node_visitor::visit(statement_try *ref)
        {
        }

        void node_visitor::visit(inheritance *ref)
        {
        }

        void node_visitor::visit(character *ref)
        {
        }

        void node_visitor::visit(statement_for *ref)
        {
        }

        void node_visitor::visit(is_less_than *ref)
        {
        }

        void node_visitor::visit(statement_empty *ref)
        {
        }

        void node_visitor::visit(operator_new *ref)
        {
        }

        void node_visitor::visit(prefix_increment *ref)
        {
        }

        void node_visitor::visit(statement_lock *ref)
        {
        }
    }
};
