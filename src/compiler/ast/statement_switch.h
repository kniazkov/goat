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
#include "token_with_list.h"
#include "nonterminal.h"
#include "expression.h"
#include "keyword_switch.h"
#include "brackets_pair.h"
#include <vector>

namespace g0at
{
    namespace ast
    {
        class case_block : public lib::ref_counter
        {
        public:
            case_block(lib::pointer<expression> _expr)
                : expr(_expr)
            {
            }

            lib::pointer<expression> get_expression() { return expr; }
            token_list *get_body() { return &body; }

        protected:
            lib::pointer<expression> expr;
            token_list body;
        };

        class statement_switch : public statement, public token_with_list
        {
        public:
            statement_switch(keyword_switch *_kw, lib::pointer<expression> _expr, brackets_pair *_body);
            void accept(token_visitor *visitor) override;
            statement_switch *to_statement_switch() override;

            lib::pointer<expression> get_expression() { return expr; }
            void add_block(lib::pointer<case_block> block) { case_blocks.push_back(block); }
            int get_count() { return (int)case_blocks.size(); }
            lib::pointer<case_block> get_block(int index) { return case_blocks.at(index); }
            token_list *get_default_block() { return &default_block; }

        protected:
            lib::pointer<expression> expr;
            std::vector<lib::pointer<case_block>> case_blocks;
            token_list default_block;
        };
    };
};