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

#include "binary.h"

namespace g0at
{
    namespace pt
    {
        class is_greater_than_or_equal_to : public binary
        {
        public:
            is_greater_than_or_equal_to(lib::pointer<position> _pos, lib::pointer<expression> _left, lib::pointer<expression> _right);
            void accept(node_visitor *visitor) override;
            is_greater_than_or_equal_to *to_is_greater_than_or_equal_to() override;
        };
    };
};