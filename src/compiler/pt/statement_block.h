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

#include "statement.h"
#include <vector>

namespace g0at
{
    namespace pt
    {
        class statement_block : public statement
        {
        public:
            statement_block(lib::pointer<position> _pos);
            void accept(node_visitor *visitor) override;
            statement_block *to_statement_block() override;

            void add_stmt(lib::pointer<statement> stmt) { code.push_back(stmt); }
            int get_code_size() { return (int)code.size(); }
            lib::pointer<statement> get_stmt(int index) { return code.at(index); }

        protected:
            std::vector<lib::pointer<statement>> code;
        };
    };
};
