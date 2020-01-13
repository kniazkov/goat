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
#include "keyword_for.h"

namespace g0at
{
    namespace ast
    {
        class statement_for_in : public statement, public nonterminal
        {
        public:
            statement_for_in(keyword_for *_kw, std::wstring _name, bool _declared, lib::pointer<expression> _expr, lib::pointer<statement> _body);
            void accept(token_visitor *visitor) override;
            statement_for_in *to_statement_for_in() override;

            std::wstring get_name() { return name; }
            bool is_declared() { return declared; }
            lib::pointer<expression> get_expression() { return expr; }
            lib::pointer<statement> get_body() { return body; };

        protected:
            std::wstring name;
            bool declared;
            lib::pointer<expression> expr;
            lib::pointer<statement> body;
        };
    };
};