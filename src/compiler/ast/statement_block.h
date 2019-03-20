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

#include "token_with_list.h"
#include "statement.h"
#include "brackets_pair.h"
#include <utility>
#include <vector>

namespace g0at
{
    namespace ast
    {
        class statement_block : public statement, public token_with_list
        {
        public:
            statement_block(brackets_pair *_body);
            void accept(token_visitor *visitor) override;
            statement_block *to_statement_block() override;

            token_list *get_body() { return &body; }

        protected:
            token_list body;
        };
    };
};