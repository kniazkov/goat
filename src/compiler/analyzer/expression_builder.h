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

#include "compiler/ast/token_visitor.h"
#include "compiler/pt/expression.h"
#include "compiler/ast/binary.h"
#include "compiler/ast/unary_prefix.h"
#include "lib/pointer.h"
#include <utility>

namespace g0at
{
    namespace analyzer
    {
        class expression_builder : public ast::token_visitor
        {
        public:
            void visit(ast::variable *ref) override;
            void visit(ast::static_string *ref) override;
            void visit(ast::function_call *ref) override;
            void visit(ast::addition *ref) override;
            void visit(ast::integer *ref) override;
            void visit(ast::subtraction *ref) override;
            void visit(ast::negation *ref) override;
            void visit(ast::value_void *ref) override;
            void visit(ast::value_undefined *ref) override;
            void visit(ast::value_null *ref) override;
            void visit(ast::assignment *ref) override;
            void visit(ast::real *ref) override;
            void visit(ast::declare_function *ref) override;
            void visit(ast::token_object *ref) override;
            void visit(ast::property *ref) override;
            void visit(ast::value_true *ref) override;
            void visit(ast::value_false *ref) override;
            void visit(ast::is_equal_to *ref) override;
            void visit(ast::is_not_equal_to *ref) override;
            void visit(ast::method_call *ref) override;
            void visit(ast::this_ptr *ref) override;
            void visit(ast::token_array *ref) override;
            void visit(ast::inheritance *ref) override;
            void visit(ast::character *ref) override;
            void visit(ast::is_less_than *ref) override;
            void visit(ast::operator_new *ref) override;
            void visit(ast::prefix_increment *ref) override;
            void visit(ast::index_access *ref) override;
            void visit(ast::suffix_increment *ref) override;
            void visit(ast::prefix_decrement *ref) override;
            void visit(ast::suffix_decrement *ref) override;
            void visit(ast::multiplication *ref) override;
            void visit(ast::exponentiation *ref) override;
            void visit(ast::division *ref) override;
            void visit(ast::remainder *ref) override;
            void visit(ast::unary_plus *ref) override;
            void visit(ast::logical_not *ref) override;
            void visit(ast::bitwise_not *ref) override;
            void visit(ast::operator_bool *ref) override;
            void visit(ast::left_shift *ref) override;
            void visit(ast::signed_right_shift *ref) override;
            void visit(ast::zero_fill_right_shift *ref) override;
            void visit(ast::is_less_than_or_equal_to *ref) override;
            void visit(ast::is_greater_than *ref) override;
            void visit(ast::is_greater_than_or_equal_to *ref) override;
            void visit(ast::bitwise_and *ref) override;
            void visit(ast::bitwise_or *ref) override;
            void visit(ast::bitwise_xor *ref) override;
            void visit(ast::logical_and *ref) override;
            void visit(ast::logical_or *ref) override;
            void visit(ast::assignment_by_sum *ref) override;
            void visit(ast::assignment_by_difference *ref) override;
            void visit(ast::assignment_by_product *ref) override;
            void visit(ast::assignment_by_quotient *ref) override;
            void visit(ast::assignment_by_remainder *ref) override;
            void visit(ast::assignment_by_left_shift *ref) override;
            void visit(ast::assignment_by_signed_right_shift *ref) override;
            void visit(ast::assignment_by_zero_fill_right_shift *ref) override;
            void visit(ast::assignment_by_bitwise_and *ref) override;
            void visit(ast::assignment_by_bitwise_or *ref) override;
            void visit(ast::assignment_by_bitwise_xor *ref) override;
            void visit(ast::ternary *ref) override;
            void visit(ast::parenthesized_expression *ref) override;

            bool has_expr() { return expr != nullptr; }
            lib::pointer<pt::expression> get_expr() { return expr; }

        protected:
            lib::pointer<pt::expression> expr;
            
            std::pair<lib::pointer<pt::expression>, lib::pointer<pt::expression>> build_expr_for_binary(ast::binary *ref);
            lib::pointer<pt::expression> build_expr_for_unary_prefix(ast::unary_prefix *ref);
            lib::pointer<pt::expression> build_expr_for_unary_suffix(ast::unary_suffix *ref);
        };
    };
};
