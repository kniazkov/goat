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
    };
};