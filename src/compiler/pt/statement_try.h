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
#include "expression.h"

namespace g0at
{
    namespace pt
    {
        class statement_try : public statement
        {
        public:
            statement_try(lib::pointer<position> _pos, lib::pointer<statement> _stmt_try, lib::pointer<statement> _stmt_catch, std::wstring _var_name);
            statement_try(lib::pointer<position> _pos, lib::pointer<statement> _stmt_try, lib::pointer<statement> _stmt_finally);
            statement_try(lib::pointer<position> _pos, lib::pointer<statement> _stmt_try, lib::pointer<statement> _stmt_catch, std::wstring _var_name, lib::pointer<statement> _stmt_finally);
            void accept(node_visitor *visitor) override;
            statement_try *to_statement_try() override;

            lib::pointer<statement> get_stmt_try() { return stmt_try; }
            lib::pointer<statement> get_stmt_catch() { return stmt_catch; }
            bool has_var() { return var_name.size() != 0; }
            std::wstring get_var_name() { return var_name; }
            lib::pointer<statement> get_stmt_finally() { return stmt_finally; }

        protected:
            lib::pointer<statement> stmt_try;
            lib::pointer<statement> stmt_catch;
            std::wstring var_name;
            lib::pointer<statement> stmt_finally;
        };
    };
};
