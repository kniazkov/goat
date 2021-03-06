/*

Copyright (C) 2017-2020 Ivan Kniazkov

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

#include "compiler/pt/node_visitor.h"
#include "compiler/pt/scope.h"
#include "lib/pointer.h"

namespace g0at
{
    namespace analyzer
    {
        class scope_builder : public pt::node_visitor
        {
        public:
            scope_builder(lib::pointer<pt::scope> _s0);
            void visit(pt::function *ref) override;
            void visit(pt::variable *ref) override;
            void visit(pt::static_string *ref) override;
            void visit(pt::function_call *ref) override;
            void visit(pt::statement_expression *ref) override;
            void visit(pt::addition *ref) override;
            void visit(pt::integer *ref) override;
            void visit(pt::subtraction *ref) override;
            void visit(pt::negation *ref) override;
            void visit(pt::value_void *ref) override;
            void visit(pt::value_undefined *ref) override;
            void visit(pt::value_null *ref) override;
            void visit(pt::declare_variable *ref) override;
            void visit(pt::assignment *ref) override;
            void visit(pt::real *ref) override;
            void visit(pt::declare_function *ref) override;
            void visit(pt::statement_return *ref) override;
            void visit(pt::node_object *ref) override;
            void visit(pt::property *ref) override;
            void visit(pt::value_true *ref) override;
            void visit(pt::value_false *ref) override;
            void visit(pt::is_equal_to *ref) override;
            void visit(pt::is_not_equal_to *ref) override;
            void visit(pt::statement_while *ref) override;
            void visit(pt::method_call *ref) override;
            void visit(pt::this_ptr *ref) override;
            void visit(pt::node_array *ref) override;
            void visit(pt::statement_block *ref) override;
            void visit(pt::statement_if *ref) override;
            void visit(pt::statement_throw *ref) override;
            void visit(pt::statement_try *ref) override;
            void visit(pt::inheritance *ref) override;
            void visit(pt::character *ref) override;
            void visit(pt::statement_for *ref) override;

        protected:
            lib::pointer<pt::scope> s0;
        };
    };
};
