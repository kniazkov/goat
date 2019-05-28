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

#include "node_visitor.h"
#include <deque>


namespace g0at
{
    namespace pt
    {
        class node;

        class parse_tree_traversal : public node_visitor
        {
        public:
            parse_tree_traversal();
            void traverse(node *root);
            void visit(function *ref) override;
            void visit(variable *ref) override;
            void visit(static_string *ref) override;
            void visit(function_call *ref) override;
            void visit(statement_expression *ref) override;
            void visit(addition *ref) override;
            void visit(integer *ref) override;
            void visit(subtraction *ref) override;
            void visit(negation *ref) override;
            void visit(value_void *ref) override;
            void visit(value_undefined *ref) override;
            void visit(value_null *ref) override;
            void visit(declare_variable *ref) override;
            void visit(assignment *ref) override;
            void visit(real *ref) override;
            void visit(declare_function *ref) override;
            void visit(statement_return *ref) override;
            void visit(node_object *ref) override;
            void visit(property *ref) override;
            void visit(value_true *ref) override;
            void visit(value_false *ref) override;
            void visit(is_equal_to *ref) override;
            void visit(is_not_equal_to *ref) override;
            void visit(statement_while *ref) override;
            void visit(method_call *ref) override;
            void visit(this_ptr *ref) override;
            void visit(node_array *ref) override;
            void visit(statement_block *ref) override;
            void visit(statement_if *ref) override;
            void visit(statement_throw *ref) override;
            void visit(statement_try *ref) override;
            void visit(inheritance *ref) override;
            void visit(character *ref) override;

        protected:
            void postpone(node *n) { queue.push_front(n); }
            bool is_postponed(node *n);

            virtual void payload(function *ref);
            virtual void payload(variable *ref);
            virtual void payload(static_string *ref);
            virtual void payload(function_call *ref);
            virtual void payload(statement_expression *ref);
            virtual void payload(addition *ref);
            virtual void payload(integer *ref);
            virtual void payload(subtraction *ref);
            virtual void payload(negation *ref);
            virtual void payload(value_void *ref);
            virtual void payload(value_undefined *ref);
            virtual void payload(value_null *ref);
            virtual void payload(declare_variable *ref);
            virtual void payload(assignment *ref);
            virtual void payload(real *ref);
            virtual void payload(declare_function *ref);
            virtual void payload(statement_return *ref);
            virtual void payload(node_object *ref);
            virtual void payload(property *ref);
            virtual void payload(value_true *ref);
            virtual void payload(value_false *ref);
            virtual void payload(is_equal_to *ref);
            virtual void payload(is_not_equal_to *ref);
            virtual void payload(statement_while *ref);
            virtual void payload(method_call *ref);
            virtual void payload(this_ptr *ref);
            virtual void payload(node_array *ref);
            virtual void payload(statement_block *ref);
            virtual void payload(statement_if *ref);
            virtual void payload(statement_throw *ref);
            virtual void payload(statement_try *ref);
            virtual void payload(inheritance *ref);
            virtual void payload(character *ref);

        private:
            std::deque<node*> queue;
        };
    };
};