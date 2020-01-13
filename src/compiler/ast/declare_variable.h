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

#include "statement.h"
#include "nonterminal.h"
#include "expression.h"
#include "keyword_var.h"
#include <string>
#include <vector>

namespace g0at
{
    namespace ast
    {
        struct variable_info
        {
            std::wstring name;
            lib::pointer<expression> init_val;
        };

        class declare_variable : public statement, public nonterminal
        {
        public:
            declare_variable(keyword_var *kw);
            void accept(token_visitor *visitor) override;
            declare_variable *to_declare_variable() override;

            void add_variable(variable_info &item);
            int get_count() { return (int)items.size(); }
            variable_info get_variable(int idx) { return items.at(idx); }

        protected:
            std::vector<variable_info> items;
        };
    };
};