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

#include "scope_builder.h"
#include "compiler/pt/function.h"
#include "compiler/pt/variable.h"
#include "compiler/pt/static_string.h"
#include "compiler/pt/function_call.h"
#include "compiler/pt/statement_expression.h"
#include "compiler/pt/addition.h"
#include "compiler/pt/integer.h"
#include "compiler/pt/subtraction.h"
#include "compiler/pt/negation.h"
#include "compiler/pt/declare_variable.h"
#include "compiler/pt/assignment.h"
#include "compiler/pt/real.h"
#include "compiler/pt/declare_function.h"
#include "compiler/pt/statement_return.h"
#include "compiler/pt/node_object.h"
#include "compiler/pt/property.h"
#include "compiler/pt/is_equal_to.h"
#include "compiler/pt/is_not_equal_to.h"
#include "compiler/pt/statement_while.h"
#include "compiler/pt/method_call.h"
#include "compiler/pt/this_ptr.h"
#include "compiler/pt/node_array.h"
#include "compiler/pt/statement_block.h"
#include "compiler/pt/statement_if.h"
#include "compiler/pt/statement_throw.h"
#include "compiler/pt/statement_try.h"
#include "compiler/pt/inheritance.h"
#include "compiler/pt/character.h"
#include "compiler/pt/value_void.h"
#include "compiler/pt/value_undefined.h"
#include "compiler/pt/value_null.h"
#include "compiler/pt/value_true.h"
#include "compiler/pt/value_false.h"
#include "compiler/pt/statement_for.h"


namespace g0at
{
    namespace analyzer
    {
        scope_builder::scope_builder(lib::pointer<pt::scope> _s0)
            : s0(_s0)
        {
        }

        void scope_builder::visit(pt::function *ref)
        {
            lib::pointer<pt::scope> s1 = new pt::scope(s0);
            ref->set_scope(s0);
            scope_builder b(s1);
            for (int i = 0, code_size = ref->get_code_size(); i < code_size; i++)
            {
                ref->get_stmt(i)->accept(&b);
            }
        }

        void scope_builder::visit(pt::static_string *ref)
        {
            ref->set_scope(s0);
        }

        void scope_builder::visit(pt::variable *ref)
        {
            ref->set_scope(s0);
        }

        void scope_builder::visit(pt::function_call *ref)
        {
            ref->set_scope(s0);
            int args_count = ref->get_args_count();
            for (int i = args_count - 1; i > -1; i--)
            {
                ref->get_arg(i)->accept(this);
            }
        }

        void scope_builder::visit(pt::statement_expression *ref)
        {
            ref->set_scope(s0);
            ref->get_expression()->accept(this);
        }

        void scope_builder::visit(pt::addition *ref)
        {
            ref->set_scope(s0);
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
        }

        void scope_builder::visit(pt::integer *ref)
        {
            ref->set_scope(s0);
        }

        void scope_builder::visit(pt::subtraction *ref)
        {
            ref->set_scope(s0);
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
        }

        void scope_builder::visit(pt::negation *ref)
        {
            ref->set_scope(s0);
            ref->get_right()->accept(this);
        }

        void scope_builder::visit(pt::value_void *ref)
        {
            ref->set_scope(s0);
        }

        void scope_builder::visit(pt::value_undefined *ref)
        {
            ref->set_scope(s0);
        }

        void scope_builder::visit(pt::value_null *ref)
        {
            ref->set_scope(s0);
        }

        void scope_builder::visit(pt::declare_variable *ref)
        {
            s0 = new pt::scope(s0);
            ref->set_scope(s0);
            for (int i = 0, count = ref->get_count(); i < count; i++)
            {
                pt::variable_info info = ref->get_variable(i);
                if (info.init_val)
                    info.init_val->accept(this);
            }
        }

        void scope_builder::visit(pt::assignment *ref)
        {
            ref->set_scope(s0);
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
        }

        void scope_builder::visit(pt::real *ref)
        {
            ref->set_scope(s0);
        }

        void scope_builder::visit(pt::declare_function *ref)
        {
            // TODO: change it
            ref->set_scope(s0);
            ref->get_func()->accept(this);
        }

        void scope_builder::visit(pt::statement_return *ref)
        {
            ref->set_scope(s0);
            auto expr = ref->get_expression();
            if (expr)
                expr->accept(this);
        }

        void scope_builder::visit(pt::node_object *ref)
        {
            ref->set_scope(s0);
            int count = ref->get_items_count();
            for (int i = count - 1; i > -1; i--)
            {
                auto item = ref->get_item(i);
                item.second->accept(this);
                item.first->accept(this);
            }
        }

        void scope_builder::visit(pt::property *ref)
        {
            ref->set_scope(s0);
            ref->get_left()->accept(this);
        }

        void scope_builder::visit(pt::value_true *ref)
        {
            ref->set_scope(s0);
        }

        void scope_builder::visit(pt::value_false *ref)
        {
            ref->set_scope(s0);
        }

        void scope_builder::visit(pt::is_equal_to *ref)
        {
            ref->set_scope(s0);
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
        }

        void scope_builder::visit(pt::is_not_equal_to *ref)
        {
            ref->set_scope(s0);
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
        }

        void scope_builder::visit(pt::statement_while *ref)
        {
            ref->set_scope(s0);
            ref->get_expression()->accept(this);
            ref->get_statement()->accept(this);
        }

        void scope_builder::visit(pt::method_call *ref)
        {
            // TODO: fix it
            ref->set_scope(s0);
            int args_count = ref->get_args_count();
            for (int i = args_count - 1; i > -1; i--)
            {
                ref->get_arg(i)->accept(this);
            }
            ref->get_left()->accept(this);
        }

        void scope_builder::visit(pt::this_ptr *ref)
        {
            // TODO: fix it
            ref->set_scope(s0);
        }

        void scope_builder::visit(pt::node_array *ref)
        {
            ref->set_scope(s0);
            for (int i = 0, count = ref->get_objects_count(); i < count; i++)
            {
                auto item = ref->get_object(i);
                item->accept(this);
            }
        }

        void scope_builder::visit(pt::statement_block *ref)
        {
            lib::pointer<pt::scope> s1 = new pt::scope(s0);
            ref->set_scope(s0);
            scope_builder b(s1);
            int code_size = ref->get_code_size();

            for (int i = 0; i < code_size; i++)
            {
                ref->get_stmt(i)->accept(&b);
            }
        }

        void scope_builder::visit(pt::statement_if *ref)
        {
            ref->set_scope(s0);
            ref->get_expression()->accept(this);
            ref->get_stmt_if()->accept(this);
            auto stmt_else = ref->get_stmt_else();
            if (stmt_else)
            {
                stmt_else->accept(this);
            }
        }

        void scope_builder::visit(pt::statement_throw *ref)
        {
            ref->set_scope(s0);
            auto expr = ref->get_expression();
            if (expr)
            {
                expr->accept(this);
            }
        }

        void scope_builder::visit(pt::statement_try *ref)
        {
            ref->set_scope(s0);
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

        void scope_builder::visit(pt::inheritance *ref)
        {
            ref->set_scope(s0);
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
        }

        void scope_builder::visit(pt::character *ref)
        {
            ref->set_scope(s0);
        }

        void scope_builder::visit(pt::statement_for *ref)
        {
            lib::pointer<pt::scope> s1 = new pt::scope(s0);
            ref->set_scope(s0);
            scope_builder b(s1);
            ref->get_stmt_init()->accept(&b);
            ref->get_condition()->accept(&b);
            ref->get_increment()->accept(&b);
            ref->get_body()->accept(&b);
        }
    };
};
