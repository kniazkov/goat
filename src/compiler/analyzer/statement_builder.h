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
#include "compiler/pt/statement.h"
#include "lib/pointer.h"

namespace g0at
{
    namespace analyzer
    {
        class statement_builder : public ast::token_visitor
        {
        public:
            void visit(ast::statement_expression *ref) override;
            void visit(ast::declare_variable *ref) override;

            bool has_stmt() { return stmt != nullptr; }
            lib::pointer<pt::statement> get_stmt() { return stmt; }

        protected:
            lib::pointer<pt::statement> stmt;
        };
    };
};
