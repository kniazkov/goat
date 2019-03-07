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
#include "expression.h"
#include "brackets_pair.h"
#include <string>

namespace g0at
{
    namespace ast
    {
        class method_call : public expression, public token_with_list
        {
        public:
            method_call(lib::pointer<expression> _left, std::wstring _right, brackets_pair *_args);
            void accept(token_visitor *visitor) override;
            method_call *to_method_call() override;

            lib::pointer<expression> get_left() { return left; }
            std::wstring get_right() { return right; }
            std::wstring get_name() { return right; }
            token_list *get_args_list() { return &args; }

        protected:
            lib::pointer<expression> left;
            std::wstring right;
            token_list args;
        };
    };
};
