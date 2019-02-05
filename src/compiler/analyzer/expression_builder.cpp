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

#include "expression_builder.h"
#include "compiler/ast/variable.h"
#include "compiler/pt/variable.h"
#include "compiler/ast/static_string.h"
#include "compiler/pt/static_string.h"
#include "compiler/ast/function_call.h"
#include "compiler/pt/function_call.h"
#include "compiler/ast/addition.h"
#include "compiler/pt/addition.h"
#include "compiler/ast/integer.h"
#include "compiler/pt/integer.h"
#include "compiler/ast/subtraction.h"
#include "compiler/pt/subtraction.h"
#include "compiler/ast/negation.h"
#include "compiler/pt/negation.h"
#include "compiler/ast/value_void.h"
#include "compiler/pt/value_void.h"
#include "compiler/ast/value_undefined.h"
#include "compiler/pt/value_undefined.h"
#include "compiler/ast/value_null.h"
#include "compiler/pt/value_null.h"
#include <assert.h>

namespace g0at
{
    namespace analyzer
    {
        void expression_builder::visit(ast::variable *ref)
        {
            expr = new pt::variable(ref->get_position(), ref->get_name());
        }

        void expression_builder::visit(ast::static_string *ref)
        {
            expr = new pt::static_string(ref->get_position(), ref->get_text());
        }

        void expression_builder::visit(ast::function_call *ref)
        {
            lib::pointer<pt::function_call> fcall = 
                new pt::function_call(ref->get_position(), ref->get_name());
            auto args = ref->get_raw_list();
            auto tok_arg = args->first;
            while(tok_arg)
            {
                expression_builder visitor;
                tok_arg->accept(&visitor);
                assert(visitor.has_expr()); // TODO: exception ?
                fcall->add_arg(visitor.get_expr());
                tok_arg = tok_arg->next;
            }
            expr = fcall.cast<pt::expression>();
        }

        void expression_builder::visit(ast::addition *ref)
        {
            auto pair = build_expr_for_binary(ref);
            expr = new pt::addition(ref->get_position(), pair.first, pair.second);
        }

        void expression_builder::visit(ast::integer *ref)
        {
            expr = new pt::integer(ref->get_position(), ref->get_value());
        }

        void expression_builder::visit(ast::subtraction *ref)
        {
            auto pair = build_expr_for_binary(ref);
            expr = new pt::subtraction(ref->get_position(), pair.first, pair.second);
        }

        void expression_builder::visit(ast::negation *ref)
        {
            expr = new pt::negation(ref->get_position(), build_expr_for_unary_prefix(ref));
        }

        void expression_builder::visit(ast::value_void *ref)
        {
            expr = new pt::value_void(ref->get_position());
        }

        void expression_builder::visit(ast::value_undefined *ref)
        {
            expr = new pt::value_undefined(ref->get_position());
        }

        void expression_builder::visit(ast::value_null *ref)
        {
            expr = new pt::value_null(ref->get_position());
        }

        std::pair<lib::pointer<pt::expression>, lib::pointer<pt::expression>>
        expression_builder::build_expr_for_binary(ast::binary *ref)
        {
            expression_builder visitor_left;
            ref->get_left()->accept(&visitor_left);
            assert(visitor_left.has_expr());

            expression_builder visitor_right;
            ref->get_right()->accept(&visitor_right);
            assert(visitor_right.has_expr());

            return std::make_pair<lib::pointer<pt::expression>, lib::pointer<pt::expression>>
                (visitor_left.get_expr(), visitor_right.get_expr());
        }

        lib::pointer<pt::expression> expression_builder::build_expr_for_unary_prefix(ast::unary_prefix *ref)
        {
            expression_builder visitor_right;
            ref->get_right()->accept(&visitor_right);
            assert(visitor_right.has_expr());
            return visitor_right.get_expr();
        }
    };
};