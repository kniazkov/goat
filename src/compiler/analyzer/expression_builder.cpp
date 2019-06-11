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
#include "statement_builder.h"
#include "lib/assert.h"
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
#include "compiler/ast/assignment.h"
#include "compiler/pt/assignment.h"
#include "compiler/ast/real.h"
#include "compiler/pt/real.h"
#include "compiler/ast/function.h"
#include "compiler/pt/function.h"
#include "compiler/ast/declare_function.h"
#include "compiler/pt/declare_function.h"
#include "compiler/ast/token_object.h"
#include "compiler/pt/node_object.h"
#include "compiler/ast/property.h"
#include "compiler/pt/property.h"
#include "compiler/ast/value_true.h"
#include "compiler/pt/value_true.h"
#include "compiler/ast/value_false.h"
#include "compiler/pt/value_false.h"
#include "compiler/ast/is_equal_to.h"
#include "compiler/pt/is_equal_to.h"
#include "compiler/ast/is_not_equal_to.h"
#include "compiler/pt/is_not_equal_to.h"
#include "compiler/ast/method_call.h"
#include "compiler/pt/method_call.h"
#include "compiler/ast/this_ptr.h"
#include "compiler/pt/this_ptr.h"
#include "compiler/ast/token_array.h"
#include "compiler/pt/node_array.h"
#include "compiler/ast/inheritance.h"
#include "compiler/pt/inheritance.h"
#include "compiler/ast/character.h"
#include "compiler/pt/character.h"
#include "compiler/ast/is_less_than.h"
#include "compiler/pt/is_less_than.h"
#include "compiler/ast/operator_new.h"
#include "compiler/pt/operator_new.h"
#include "compiler/ast/prefix_increment.h"
#include "compiler/pt/prefix_increment.h"

namespace g0at
{
    namespace analyzer
    {
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
            expression_builder func_object_visitor;
            ref->get_func_object()->accept(&func_object_visitor);
            assert(func_object_visitor.has_expr());
            lib::pointer<pt::function_call> fcall = 
                new pt::function_call(ref->get_position(), func_object_visitor.get_expr());
            auto args = ref->get_args_list();
            auto tok_arg = args->first;
            while(tok_arg)
            {
                assert(tok_arg->to_expression() != nullptr);
                expression_builder arg_visitor;
                tok_arg->accept(&arg_visitor);
                assert(arg_visitor.has_expr());
                fcall->add_arg(arg_visitor.get_expr());
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

        void expression_builder::visit(ast::assignment *ref)
        {
            auto pair = build_expr_for_binary(ref);
            expr = new pt::assignment(ref->get_position(), pair.first, pair.second);
        }

        void expression_builder::visit(ast::real *ref)
        {
            expr = new pt::real(ref->get_position(), ref->get_value());
        }

        void expression_builder::visit(ast::declare_function *ref)
        {
            lib::pointer<pt::function> node_func = new pt::function(ref->get_position());
            
            auto tok_func = ref->get_func();

            auto args = tok_func->get_args_list();
            auto tok = args->first;
            while(tok)
            {
                ast::identifier *ident = tok->to_identifier();
                assert(ident != nullptr);
                node_func->add_arg(ident->get_name());
                tok = tok->next;
            }

            auto body = tok_func->get_body();
            tok = body->first;
            while(tok)
            {
                statement_builder visitor;
                tok->accept(&visitor);
                assert(visitor.has_stmt());
                node_func->add_stmt(visitor.get_stmt());
                tok = tok->next;
            }

            expr = new pt::declare_function(node_func);
        }

        void expression_builder::visit(ast::token_object *ref)
        {
            lib::pointer<pt::node_object> result = new pt::node_object(ref->get_position());
            for (int i = 0, count = ref->get_items_count(); i < count; i++)
            {
                auto item = ref->get_item(i);
                expression_builder key_visitor;
                item.first->accept(&key_visitor);
                assert(key_visitor.has_expr());
                expression_builder value_visitor;
                item.second->accept(&value_visitor);
                assert(value_visitor.has_expr());
                result->add_item(key_visitor.get_expr(), value_visitor.get_expr());
            }
            expr = result.cast<pt::expression>();
        }

        void expression_builder::visit(ast::property *ref)
        {
            expression_builder left_visitor;
            ref->get_left()->accept(&left_visitor);
            assert(left_visitor.has_expr());
            expr = new pt::property(ref->get_position(), left_visitor.get_expr(), ref->get_right());
        }

        void expression_builder::visit(ast::value_true *ref)
        {
            expr = new pt::value_true(ref->get_position());
        }

        void expression_builder::visit(ast::value_false *ref)
        {
            expr = new pt::value_false(ref->get_position());
        }

        void expression_builder::visit(ast::is_equal_to *ref)
        {
            auto pair = build_expr_for_binary(ref);
            expr = new pt::is_equal_to(ref->get_position(), pair.first, pair.second);
        }

        void expression_builder::visit(ast::is_not_equal_to *ref)
        {
            auto pair = build_expr_for_binary(ref);
            expr = new pt::is_not_equal_to(ref->get_position(), pair.first, pair.second);
        }

        void expression_builder::visit(ast::method_call *ref)
        {
            expression_builder left_visitor;
            ref->get_left()->accept(&left_visitor);
            assert(left_visitor.has_expr());
            lib::pointer<pt::method_call> vcall = 
                new pt::method_call(ref->get_position(), left_visitor.get_expr(), ref->get_right());
            auto args = ref->get_args_list();
            auto tok_arg = args->first;
            while(tok_arg)
            {
                assert(tok_arg->to_expression() != nullptr);
                expression_builder arg_visitor;
                tok_arg->accept(&arg_visitor);
                assert(arg_visitor.has_expr());
                vcall->add_arg(arg_visitor.get_expr());
                tok_arg = tok_arg->next;
            }
            expr = vcall.cast<pt::expression>();
        }

        void expression_builder::visit(ast::this_ptr *ref)
        {
            expr = new pt::this_ptr(ref->get_position());
        }

        void expression_builder::visit(ast::token_array *ref)
        {
            lib::pointer<pt::node_array> arr = 
                new pt::node_array(ref->get_position());
            auto list = ref->get_object_list();
            auto tok_obj = list->first;
            while(tok_obj)
            {
                assert(tok_obj->to_expression() != nullptr);
                expression_builder expr_visitor;
                tok_obj->accept(&expr_visitor);
                assert(expr_visitor.has_expr());
                arr->add_object(expr_visitor.get_expr());
                tok_obj = tok_obj->next;
            }
            expr = arr.cast<pt::expression>();
        }

        void expression_builder::visit(ast::inheritance *ref)
        {
            auto pair = build_expr_for_binary(ref);
            expr = new pt::inheritance(ref->get_position(), pair.first, pair.second);
        }

        void expression_builder::visit(ast::character *ref)
        {
            expr = new pt::character(ref->get_position(), ref->get_value());
        }

        void expression_builder::visit(ast::is_less_than *ref)
        {
            auto pair = build_expr_for_binary(ref);
            expr = new pt::is_less_than(ref->get_position(), pair.first, pair.second);
        }

        void expression_builder::visit(ast::operator_new *ref)
        {
            expression_builder proto_visitor;
            ref->get_proto()->accept(&proto_visitor);
            assert(proto_visitor.has_expr());
            lib::pointer<pt::operator_new> op_new = 
                new pt::operator_new(ref->get_position(), proto_visitor.get_expr());
            auto args = ref->get_args_list();
            auto tok_arg = args->first;
            while(tok_arg)
            {
                assert(tok_arg->to_expression() != nullptr);
                expression_builder arg_visitor;
                tok_arg->accept(&arg_visitor);
                assert(arg_visitor.has_expr());
                op_new->add_arg(arg_visitor.get_expr());
                tok_arg = tok_arg->next;
            }
            expr = op_new.cast<pt::expression>();
        }

        void expression_builder::visit(ast::prefix_increment *ref)
        {
            expr = new pt::prefix_increment(ref->get_position(), build_expr_for_unary_prefix(ref));
        }
    };
};