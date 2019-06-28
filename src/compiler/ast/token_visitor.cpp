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

        void token_visitor::visit(assignment *ref)
        {
        }

        void token_visitor::visit(comma *ref)
        {
        }

        void token_visitor::visit(real *ref)
        {
        }

        void token_visitor::visit(keyword_function *ref)
        {
        }

        void token_visitor::visit(declare_function *ref)
        {
        }

        void token_visitor::visit(keyword_return *ref)
        {
        }

        void token_visitor::visit(statement_return *ref)
        {
        }

        void token_visitor::visit(token_object *ref)
        {
        }

        void token_visitor::visit(colon *ref)
        {
        }

        void token_visitor::visit(dot *ref)
        {
        }

        void token_visitor::visit(property *ref)
        {
        }

        void token_visitor::visit(value_true *ref)
        {
        }

        void token_visitor::visit(value_false *ref)
        {
        }

        void token_visitor::visit(equals *ref)
        {
        }

        void token_visitor::visit(not_equal *ref)
        {
        }

        void token_visitor::visit(is_equal_to *ref)
        {
        }

        void token_visitor::visit(is_not_equal_to *ref)
        {
        }

        void token_visitor::visit(keyword_while *ref)
        {
        }

        void token_visitor::visit(statement_while *ref)
        {
        }

        void token_visitor::visit(method_call *ref)
        {
        }

        void token_visitor::visit(this_ptr *ref)
        {
        }

        void token_visitor::visit(token_array *ref)
        {
        }

        void token_visitor::visit(statement_block *ref)
        {
        }

        void token_visitor::visit(keyword_if *ref)
        {
        }

        void token_visitor::visit(keyword_else *ref)
        {
        }

        void token_visitor::visit(statement_if *ref)
        {
        }

        void token_visitor::visit(keyword_throw *ref)
        {
        }

        void token_visitor::visit(statement_throw *ref)
        {
        }

        void token_visitor::visit(keyword_try *ref)
        {
        }

        void token_visitor::visit(keyword_catch *ref)
        {
        }

        void token_visitor::visit(keyword_finally *ref)
        {
        }

        void token_visitor::visit(statement_try *ref)
        {
        }

        void token_visitor::visit(inherit *ref)
        {
        }

        void token_visitor::visit(inheritance *ref)
        {
        }

        void token_visitor::visit(character *ref)
        {
        }

        void token_visitor::visit(keyword_for *ref)
        {
        }

        void token_visitor::visit(statement_for *ref)
        {
        }

        void token_visitor::visit(less *ref)
        {
        }

        void token_visitor::visit(is_less_than *ref)
        {
        }

        void token_visitor::visit(statement_empty *ref)
        {
        }

        void token_visitor::visit(keyword_new *ref)
        {
        }

        void token_visitor::visit(operator_new *ref)
        {
        }

        void token_visitor::visit(increment *ref)
        {
        }

        void token_visitor::visit(prefix_increment *ref)
        {
        }

        void token_visitor::visit(keyword_thread *ref)
        {
        }

        void token_visitor::visit(keyword_lock *ref)
        {
        }

        void token_visitor::visit(statement_lock *ref)
        {
        }

        void token_visitor::visit(index_access *ref)
        {
        }

        void token_visitor::visit(keyword_in *ref)
        {
        }
    }
};
