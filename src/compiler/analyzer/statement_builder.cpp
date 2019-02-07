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
#include "compiler/ast/statement_expression.h"
#include "compiler/pt/statement_expression.h"
#include "compiler/ast/declare_variable.h"
#include "compiler/pt/declare_variable.h"
#include <assert.h>

namespace g0at
{
    namespace analyzer
    {
        void statement_builder::visit(ast::statement_expression *ref)
        {
            expression_builder visitor;
            ref->get_expression()->accept(&visitor);
            assert(visitor.has_expr()); // TODO: exception ?
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
                    assert(visitor.has_expr()); // TODO: exception ?
                    dst.init_val = visitor.get_expr();
                }
                result->add_variable(dst);
            }
            stmt = result.cast<pt::statement>();
        }
    };
};