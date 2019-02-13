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
#include "parser_data.h"

namespace g0at
{
    namespace parser
    {
        class parser_data_filler : public ast::token_visitor
        {
        public:
            parser_data_filler(parser_data *_data);
            void visit(ast::identifier *ref) override;
            void visit(ast::plus *ref) override;
            void visit(ast::minus *ref) override;
            void visit(ast::static_string *ref) override;
            void visit(ast::integer *ref) override;
            void visit(ast::keyword_var *ref) override;
            void visit(ast::assign *ref) override;
            void visit(ast::keyword_function *ref) override;

        protected:
            parser_data *data;
        };
    };
};
