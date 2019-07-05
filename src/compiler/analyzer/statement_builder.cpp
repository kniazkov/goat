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

#include "statement_builder.h"
#include "expression_builder.h"
#include "lib/assert.h"
#include "compiler/ast/statement_expression.h"
#include "compiler/pt/statement_expression.h"
#include "compiler/ast/declare_variable.h"
#include "compiler/pt/declare_variable.h"
#include "compiler/ast/statement_return.h"
#include "compiler/pt/statement_return.h"
#include "compiler/ast/statement_while.h"
#include "compiler/pt/statement_while.h"
#include "compiler/ast/statement_block.h"
#include "compiler/pt/statement_block.h"
#include "compiler/ast/statement_if.h"
#include "compiler/pt/statement_if.h"
#include "compiler/ast/statement_throw.h"
#include "compiler/pt/statement_throw.h"
#include "compiler/ast/statement_try.h"
#include "compiler/pt/statement_try.h"
#include "compiler/ast/statement_for.h"
#include "compiler/pt/statement_for.h"
#include "compiler/ast/statement_empty.h"
#include "compiler/pt/statement_empty.h"
#include "compiler/ast/statement_lock.h"
#include "compiler/pt/statement_lock.h"
#include "compiler/ast/statement_for_in.h"
#include "compiler/pt/statement_for_in.h"
#include "compiler/ast/statement_do_while.h"
#include "compiler/pt/statement_do_while.h"
#include "compiler/ast/statement_break.h"
#include "compiler/pt/statement_break.h"
#include "compiler/ast/statement_continue.h"
#include "compiler/pt/statement_continue.h"
#include "compiler/ast/statement_switch.h"
#include "compiler/pt/statement_switch.h"

namespace g0at
{
    namespace analyzer
    {
        void statement_builder::visit(ast::statement_expression *ref)
        {
            expression_builder visitor;
            ref->get_expression()->accept(&visitor);
            assert(visitor.has_expr());
            stmt = new pt::statement_expression(ref->get_position(), visitor.get_expr());
        }
        
        void statement_builder::visit(ast::declare_variable *ref)
        {
            lib::pointer<pt::declare_variable> result = new pt::declare_variable(ref->get_position());
            for (int i = 0, count = ref->get_count(); i < count; i++)
            {
                ast::variable_info src = ref->get_variable(i);
                pt::variable_info dst;
                dst.name = src.name;
                if (src.init_val)
                {
                    expression_builder visitor;
                    src.init_val->accept(&visitor);
                    assert(visitor.has_expr());
                    dst.init_val = visitor.get_expr();
                }
                result->add_variable(dst);
            }
            stmt = result.cast<pt::statement>();
        }

        void statement_builder::visit(ast::statement_return *ref)
        {
            expression_builder visitor;
            auto tok_expr = ref->get_expression();
            if (tok_expr)
            {
                tok_expr->accept(&visitor);
                assert(visitor.has_expr());
                stmt = new pt::statement_return(ref->get_position(), visitor.get_expr());
            }
            else
            {
                stmt = new pt::statement_return(ref->get_position(), nullptr);
            }
        }

        void statement_builder::visit(ast::statement_while *ref)
        {
            expression_builder expr_visitor;
            ref->get_expression()->accept(&expr_visitor);
            assert(expr_visitor.has_expr());
            statement_builder stmt_visitor;
            ref->get_statement()->accept(&stmt_visitor);
            assert(stmt_visitor.has_stmt());
            stmt = new pt::statement_while(ref->get_position(), expr_visitor.get_expr(), stmt_visitor.get_stmt());
        }

        void statement_builder::visit(ast::statement_block *ref)
        {
            lib::pointer<pt::statement_block> result = new pt::statement_block(ref->get_position());
            auto body = ref->get_body();
            auto tok = body->first;
            while(tok)
            {
                statement_builder visitor;
                tok->accept(&visitor);
                assert(visitor.has_stmt());
                result->add_stmt(visitor.get_stmt());
                tok = tok->next;
            }
            stmt = result.cast<pt::statement>();
        }

        void statement_builder::visit(ast::statement_if *ref)
        {
            expression_builder expr_visitor;
            ref->get_expression()->accept(&expr_visitor);
            assert(expr_visitor.has_expr());
            statement_builder stmt_if_visitor;
            ref->get_stmt_if()->accept(&stmt_if_visitor);
            assert(stmt_if_visitor.has_stmt());
            auto stmt_else = ref->get_stmt_else();
            if (stmt_else)
            {
                statement_builder stmt_else_visitor;
                stmt_else->accept(&stmt_else_visitor);
                assert(stmt_else_visitor.has_stmt());
                stmt = new pt::statement_if(ref->get_position(), expr_visitor.get_expr(),
                    stmt_if_visitor.get_stmt(), stmt_else_visitor.get_stmt());
            }
            else
            {
                stmt = new pt::statement_if(ref->get_position(), expr_visitor.get_expr(), stmt_if_visitor.get_stmt());
            }
        }

        void statement_builder::visit(ast::statement_throw *ref)
        {
            expression_builder visitor;
            auto tok_expr = ref->get_expression();
            if (tok_expr)
            {
                tok_expr->accept(&visitor);
                assert(visitor.has_expr());
                stmt = new pt::statement_throw(ref->get_position(), visitor.get_expr());
            }
            else
            {
                stmt = new pt::statement_throw(ref->get_position(), nullptr);
            }
        }

        void statement_builder::visit(ast::statement_try *ref)
        {
            statement_builder stmt_try_visitor;
            ref->get_stmt_try()->accept(&stmt_try_visitor);
            assert(stmt_try_visitor.has_stmt());
            auto stmt_catch = ref->get_stmt_catch();
            if (stmt_catch)
            {
                statement_builder stmt_catch_visitor;
                stmt_catch->accept(&stmt_catch_visitor);
                assert(stmt_catch_visitor.has_stmt());
                auto stmt_finally = ref->get_stmt_finally();
                if(stmt_finally)
                {
                    statement_builder stmt_finally_visitor;
                    stmt_finally->accept(&stmt_finally_visitor);
                    assert(stmt_finally_visitor.has_stmt());
                    stmt = new pt::statement_try(ref->get_position(), stmt_try_visitor.get_stmt(),
                        stmt_catch_visitor.get_stmt(), ref->get_var_name(), stmt_finally_visitor.get_stmt());
                }
                else
                {
                    stmt = new pt::statement_try(ref->get_position(), stmt_try_visitor.get_stmt(),
                        stmt_catch_visitor.get_stmt(), ref->get_var_name());
                }
            }
            else
            {
                auto stmt_finally = ref->get_stmt_finally();
                assert(stmt_finally != nullptr);
                statement_builder stmt_finally_visitor;
                stmt_finally->accept(&stmt_finally_visitor);
                assert(stmt_finally_visitor.has_stmt());
                stmt = new pt::statement_try(ref->get_position(), stmt_try_visitor.get_stmt(), stmt_finally_visitor.get_stmt());
            }
        }

        void statement_builder::visit(ast::statement_for *ref)
        {
            lib::pointer<pt::statement> node_stmt_init = nullptr;
            auto tok_stmt_init = ref->get_stmt_init();
            if (tok_stmt_init)
            {
                statement_builder stmt_init_visitor;
                tok_stmt_init->accept(&stmt_init_visitor);
                assert(stmt_init_visitor.has_stmt());
                node_stmt_init = stmt_init_visitor.get_stmt();
            }

            lib::pointer<pt::expression> node_condition = nullptr;
            auto tok_condition = ref->get_condition();
            if (tok_condition)
            {
                expression_builder condition_visitor;
                tok_condition->accept(&condition_visitor);
                assert(condition_visitor.has_expr());
                node_condition = condition_visitor.get_expr();
            }

            lib::pointer<pt::statement> node_increment = nullptr;
            auto tok_increment = ref->get_increment();
            if (tok_increment)
            {
                statement_builder increment_visitor;
                tok_increment->accept(&increment_visitor);
                assert(increment_visitor.has_stmt());
                node_increment = increment_visitor.get_stmt();
            }

            statement_builder body_visitor;
            ref->get_body()->accept(&body_visitor);
            assert(body_visitor.has_stmt());
            lib::pointer<pt::statement> node_body = body_visitor.get_stmt();

            stmt = new pt::statement_for(ref->get_position(), node_stmt_init, node_condition, node_increment, node_body);
        }

        void statement_builder::visit(ast::statement_empty *ref)
        {
            stmt = new pt::statement_empty(ref->get_position());
        }

        void statement_builder::visit(ast::statement_lock *ref)
        {
            statement_builder visitor;
            ref->get_statement()->accept(&visitor);
            assert(visitor.has_stmt());
            stmt = new pt::statement_lock(ref->get_position(), visitor.get_stmt());
        }

        void statement_builder::visit(ast::statement_for_in *ref)
        {
            expression_builder expr_visitor;
            ref->get_expression()->accept(&expr_visitor);
            assert(expr_visitor.has_expr());
            statement_builder body_visitor;
            ref->get_body()->accept(&body_visitor);
            assert(body_visitor.has_stmt());
            stmt = new pt::statement_for_in(ref->get_position(), ref->get_name(), ref->is_declared(),
                expr_visitor.get_expr(), body_visitor.get_stmt());
        }

        void statement_builder::visit(ast::statement_do_while *ref)
        {
            expression_builder expr_visitor;
            ref->get_expression()->accept(&expr_visitor);
            assert(expr_visitor.has_expr());
            statement_builder stmt_visitor;
            ref->get_statement()->accept(&stmt_visitor);
            assert(stmt_visitor.has_stmt());
            stmt = new pt::statement_do_while(ref->get_position(), expr_visitor.get_expr(), stmt_visitor.get_stmt());
        }

        void statement_builder::visit(ast::statement_break *ref)
        {
            stmt = new pt::statement_break(ref->get_position());
        }

        void statement_builder::visit(ast::statement_continue *ref)
        {
            stmt = new pt::statement_continue(ref->get_position());
        }

        void statement_builder::visit(ast::statement_switch *ref)
        {
            expression_builder expr_visitor;
            ref->get_expression()->accept(&expr_visitor);
            assert(expr_visitor.has_expr());
            lib::pointer<pt::statement_switch> result = new pt::statement_switch(ref->get_position(), expr_visitor.get_expr());
            int i, n;
            for (i = 0, n = ref->get_count(); i < n; i++)
            {
                auto src_block = ref->get_block(i);
                expression_builder block_expr_visitor;
                src_block->get_expression()->accept(&block_expr_visitor);
                assert(block_expr_visitor.has_expr());
                lib::pointer<pt::case_block> dst_block = new pt::case_block(block_expr_visitor.get_expr());
                result->add_block(dst_block);
                auto tok = src_block->get_body()->first;
                while(tok)
                {
                    statement_builder stmt_visitor;
                    tok->accept(&stmt_visitor);
                    assert(stmt_visitor.has_stmt());
                    dst_block->add_statement(stmt_visitor.get_stmt());
                    tok = tok->next;
                }
            }
            auto tok_list_default = ref->get_default_block();
            if (tok_list_default->first)
            {
                lib::pointer<pt::default_block> dst_block = new pt::default_block();
                result->set_default_block(dst_block);
                auto tok = tok_list_default->first;
                while(tok)
                {
                    statement_builder stmt_visitor;
                    tok->accept(&stmt_visitor);
                    assert(stmt_visitor.has_stmt());
                    dst_block->add_statement(stmt_visitor.get_stmt());
                    tok = tok->next;
                }
            }
            stmt = result.cast<pt::statement>();
        }
    };
};