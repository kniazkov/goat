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

#include "parse_tree_traversal.h"
#include "node.h"
#include "function.h"
#include "variable.h"
#include "static_string.h"
#include "function_call.h"
#include "statement_expression.h"
#include "addition.h"
#include "integer.h"
#include "subtraction.h"
#include "negation.h"
#include "declare_variable.h"
#include "assignment.h"
#include "real.h"
#include "declare_function.h"
#include "statement_return.h"
#include "node_object.h"
#include "property.h"
#include "is_equal_to.h"
#include "is_not_equal_to.h"
#include "statement_while.h"
#include "method_call.h"
#include "this_ptr.h"
#include "node_array.h"
#include "statement_block.h"
#include "statement_if.h"
#include "statement_throw.h"
#include "statement_try.h"
#include "inheritance.h"
#include "character.h"
#include "value_void.h"
#include "value_undefined.h"
#include "value_null.h"
#include "value_true.h"
#include "value_false.h"


namespace g0at
{
    namespace pt
    {
        parse_tree_traversal::parse_tree_traversal()
        {
        }

        void parse_tree_traversal::traverse(node *root)
        {
            root->accept(this);
            while(!queue.empty())
            {
                node *next = queue.back();
                queue.pop_back();
                next->accept(this);
            }
        }

        void parse_tree_traversal::visit(function *ref)
        {
            payload(ref);
            for (int i = 0, code_size = ref->get_code_size(); i < code_size; i++)
            {
                ref->get_stmt(i)->accept(this);
            }
        }

        void parse_tree_traversal::payload(function *ref)
        {
        }

        void parse_tree_traversal::visit(static_string *ref)
        {
            payload(ref);
        }

        void parse_tree_traversal::payload(static_string *ref)
        {
        }

        void parse_tree_traversal::visit(variable *ref)
        {
            payload(ref);
        }

        void parse_tree_traversal::payload(variable *ref)
        {
        }

        void parse_tree_traversal::visit(function_call *ref)
        {
            payload(ref);
            int args_count = ref->get_args_count();
            for (int i = args_count - 1; i > -1; i--)
            {
                ref->get_arg(i)->accept(this);
            }
        }

        void parse_tree_traversal::payload(function_call *ref)
        {
        }

        void parse_tree_traversal::visit(statement_expression *ref)
        {
            payload(ref);
            ref->get_expression()->accept(this);
        }

        void parse_tree_traversal::payload(statement_expression *ref)
        {
        }

        void parse_tree_traversal::visit(addition *ref)
        {
            payload(ref);
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
        }

        void parse_tree_traversal::payload(addition *ref)
        {
        }

        void parse_tree_traversal::visit(integer *ref)
        {
            payload(ref);
        }

        void parse_tree_traversal::payload(integer *ref)
        {
        }

        void parse_tree_traversal::visit(subtraction *ref)
        {
            payload(ref);
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
        }

        void parse_tree_traversal::payload(subtraction *ref)
        {
        }

        void parse_tree_traversal::visit(negation *ref)
        {
            payload(ref);
            ref->get_right()->accept(this);
        }

        void parse_tree_traversal::payload(negation *ref)
        {
        }

        void parse_tree_traversal::visit(value_void *ref)
        {
            payload(ref);
        }

        void parse_tree_traversal::payload(value_void *ref)
        {
        }

        void parse_tree_traversal::visit(value_undefined *ref)
        {
            payload(ref);
        }

        void parse_tree_traversal::payload(value_undefined *ref)
        {
        }

        void parse_tree_traversal::visit(value_null *ref)
        {
            payload(ref);
        }

        void parse_tree_traversal::payload(value_null *ref)
        {
        }

        void parse_tree_traversal::visit(declare_variable *ref)
        {
            payload(ref);
            for (int i = 0, count = ref->get_count(); i < count; i++)
            {
                variable_info info = ref->get_variable(i);
                if (info.init_val)
                    info.init_val->accept(this);
            }
        }

        void parse_tree_traversal::payload(declare_variable *ref)
        {
        }

        void parse_tree_traversal::visit(assignment *ref)
        {
            payload(ref);
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
        }

        void parse_tree_traversal::payload(assignment *ref)
        {
        }

        void parse_tree_traversal::visit(real *ref)
        {
            payload(ref);
        }

        void parse_tree_traversal::payload(real *ref)
        {
        }

        void parse_tree_traversal::visit(declare_function *ref)
        {
            payload(ref);
            ref->get_func()->accept(this);
        }

        void parse_tree_traversal::payload(declare_function *ref)
        {
        }

        void parse_tree_traversal::visit(statement_return *ref)
        {
            payload(ref);
            auto expr = ref->get_expression();
            if (expr)
                expr->accept(this);
        }

        void parse_tree_traversal::payload(statement_return *ref)
        {
        }

        void parse_tree_traversal::visit(node_object *ref)
        {
            payload(ref);
            int count = ref->get_items_count();
            for (int i = count - 1; i > -1; i--)
            {
                auto item = ref->get_item(i);
                item.second->accept(this);
                item.first->accept(this);
            }
        }

        void parse_tree_traversal::payload(node_object *ref)
        {
        }

        void parse_tree_traversal::visit(property *ref)
        {
            payload(ref);
            ref->get_left()->accept(this);
        }

        void parse_tree_traversal::payload(property *ref)
        {
        }

        void parse_tree_traversal::visit(value_true *ref)
        {
            payload(ref);
        }

        void parse_tree_traversal::payload(value_true *ref)
        {
        }

        void parse_tree_traversal::visit(value_false *ref)
        {
            payload(ref);
        }

        void parse_tree_traversal::payload(value_false *ref)
        {
        }

        void parse_tree_traversal::visit(is_equal_to *ref)
        {
            payload(ref);
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
        }

        void parse_tree_traversal::payload(is_equal_to *ref)
        {
        }

        void parse_tree_traversal::visit(is_not_equal_to *ref)
        {
            payload(ref);
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
        }

        void parse_tree_traversal::payload(is_not_equal_to *ref)
        {
        }

        void parse_tree_traversal::visit(statement_while *ref)
        {
            payload(ref);
            ref->get_expression()->accept(this);
            ref->get_statement()->accept(this);
        }

        void parse_tree_traversal::payload(statement_while *ref)
        {
        }

        void parse_tree_traversal::visit(method_call *ref)
        {
            payload(ref);
            int args_count = ref->get_args_count();
            for (int i = args_count - 1; i > -1; i--)
            {
                ref->get_arg(i)->accept(this);
            }
            ref->get_left()->accept(this);
        }

        void parse_tree_traversal::payload(method_call *ref)
        {
        }

        void parse_tree_traversal::visit(this_ptr *ref)
        {
            payload(ref);
        }

        void parse_tree_traversal::payload(this_ptr *ref)
        {
        }

        void parse_tree_traversal::visit(node_array *ref)
        {
            payload(ref);
            for (int i = 0, count = ref->get_objects_count(); i < count; i++)
            {
                auto item = ref->get_object(i);
                item->accept(this);
            }
        }

        void parse_tree_traversal::payload(node_array *ref)
        {
        }

        void parse_tree_traversal::visit(statement_block *ref)
        {
            payload(ref);
            int code_size = ref->get_code_size();
            for (int i = 0; i < code_size; i++)
            {
                ref->get_stmt(i)->accept(this);
            }
        }

        void parse_tree_traversal::payload(statement_block *ref)
        {
        }

        void parse_tree_traversal::visit(statement_if *ref)
        {
            payload(ref);
            ref->get_expression()->accept(this);
            ref->get_stmt_if()->accept(this);
            auto stmt_else = ref->get_stmt_else();
            if (stmt_else)
            {
                stmt_else->accept(this);
            }
        }

        void parse_tree_traversal::payload(statement_if *ref)
        {
        }

        void parse_tree_traversal::visit(statement_throw *ref)
        {
            payload(ref);
            auto expr = ref->get_expression();
            if (expr)
            {
                expr->accept(this);
            }
        }

        void parse_tree_traversal::payload(statement_throw *ref)
        {
        }

        void parse_tree_traversal::visit(statement_try *ref)
        {
            payload(ref);
            ref->get_stmt_try()->accept(this);
            auto stmt_catch = ref->get_stmt_catch();
            auto stmt_finally = ref->get_stmt_finally();

            if (stmt_catch)
            {
                stmt_catch->accept(this);
            }

            if (stmt_finally)
            {
                stmt_finally->accept(this);
            }
        }

        void parse_tree_traversal::payload(statement_try *ref)
        {
        }

        void parse_tree_traversal::visit(inheritance *ref)
        {
            payload(ref);
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
        }

        void parse_tree_traversal::payload(inheritance *ref)
        {
        }

        void parse_tree_traversal::visit(character *ref)
        {
            payload(ref);
        }

        void parse_tree_traversal::payload(character *ref)
        {
        }
    };
};
