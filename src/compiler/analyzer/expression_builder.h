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

            bool has_expr() { return expr != nullptr; }
            lib::pointer<pt::expression> get_expr() { return expr; }

        protected:
            lib::pointer<pt::expression> expr;
            std::pair<lib::pointer<pt::expression>, lib::pointer<pt::expression>> build_expr_for_binary(ast::binary *ref);
            lib::pointer<pt::expression> build_expr_for_unary_prefix(ast::unary_prefix *ref);
        };
    };
};
