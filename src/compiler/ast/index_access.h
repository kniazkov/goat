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

#include "token_with_list.h"
#include "left_expression.h"
#include "brackets_pair.h"

namespace g0at
{
    namespace ast
    {
        class index_access : public left_expression, public token_with_list
        {
        public:
            index_access(lib::pointer<expression> _expr,  brackets_pair *_args);
            void accept(token_visitor *visitor) override;
            index_access *to_index_access() override;

            void set_expression(lib::pointer<expression> _expr) { expr = _expr; }
            lib::pointer<expression> get_expression() { return expr; }
            token_list *get_args_list() { return &args; }

        protected:
            lib::pointer<expression> expr;
            token_list args;
        };
    };
};
