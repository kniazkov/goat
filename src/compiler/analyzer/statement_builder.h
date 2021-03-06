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
            void visit(ast::statement_return *ref) override;
            void visit(ast::statement_while *ref) override;
            void visit(ast::statement_block *ref) override;
            void visit(ast::statement_if *ref) override;
            void visit(ast::statement_throw *ref) override;
            void visit(ast::statement_try *ref) override;
            void visit(ast::statement_for *ref) override;
            void visit(ast::statement_empty *ref) override;
            void visit(ast::statement_lock *ref) override;
            void visit(ast::statement_for_in *ref) override;
            void visit(ast::statement_do_while *ref) override;
            void visit(ast::statement_break *ref) override;
            void visit(ast::statement_continue *ref) override;
            void visit(ast::statement_switch *ref) override;
            void visit(ast::statement_debug *ref) override;

            bool has_stmt() { return stmt != nullptr; }
            lib::pointer<pt::statement> get_stmt() { return stmt; }

        protected:
            lib::pointer<pt::statement> stmt;
        };
    };
};
