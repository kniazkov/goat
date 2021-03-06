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

#include "expression.h"
#include <string>

namespace g0at
{
    namespace pt
    {
        class property : public expression
        {
        public:
            property(fragment _frag, lib::pointer<expression> _left, std::wstring _right, bool _guard);
            void accept(node_visitor *visitor) override;
            property *to_property() override;

            lib::pointer<expression> get_left() { return left; }
            std::wstring get_right() { return right; }
            std::wstring get_name() { return right; }
            bool guarded() { return guard; }

        protected:
            lib::pointer<expression> left;
            std::wstring right;
            bool guard;
        };
    };
};
