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
#include "keyword_new.h"
#include "expression.h"
#include "brackets_pair.h"

namespace g0at
{
    namespace ast
    {
        class operator_new : public expression, public token_with_list
        {
        public:
            operator_new(keyword_new *_kw, lib::pointer<expression> _proto);
            operator_new(keyword_new *_kw, lib::pointer<expression> _proto,  brackets_pair *_args);
            void accept(token_visitor *visitor) override;
            operator_new *to_operator_new() override;

            lib::pointer<expression> get_proto() { return proto; }
            token_list *get_args_list() { return &args; }

        protected:
            lib::pointer<expression> proto;
            token_list args;
        };
    };
};
