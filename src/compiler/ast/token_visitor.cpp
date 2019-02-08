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

#include "token_visitor.h"

namespace g0at
{
    namespace ast
    {
        token_visitor::token_visitor()
        {
        }

        token_visitor::~token_visitor()
        {
        }

        void token_visitor::visit(function *ref)
        {
        }

        void token_visitor::visit(identifier *ref)
        {
        }

        void token_visitor::visit(variable *ref)
        {
        }

        void token_visitor::visit(bracket *ref)
        {
        }

        void token_visitor::visit(static_string *ref)
        {
        }

        void token_visitor::visit(semicolon *ref)
        {
        }

        void token_visitor::visit(brackets_pair *ref)
        {
        }

        void token_visitor::visit(function_call *ref)
        {
        }

        void token_visitor::visit(statement_expression *ref)
        {
        }

        void token_visitor::visit(plus *ref)
        {
        }

        void token_visitor::visit(custom_operator *ref)
        {
        }

        void token_visitor::visit(addition *ref)
        {
        }

        void token_visitor::visit(integer *ref)
        {
        }

        void token_visitor::visit(minus *ref)
        {
        }

        void token_visitor::visit(subtraction *ref)
        {
        }

        void token_visitor::visit(negation *ref)
        {
        }

        void token_visitor::visit(value_void *ref)
        {
        }

        void token_visitor::visit(value_undefined *ref)
        {
        }

        void token_visitor::visit(value_null *ref)
        {
        }

        void token_visitor::visit(keyword_var *ref)
        {
        }

        void token_visitor::visit(declare_variable *ref)
        {
        }

        void token_visitor::visit(assign *ref)
        {
        }
    }
};
