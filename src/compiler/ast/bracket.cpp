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

#include "bracket.h"

namespace g0at
{
    bracket::bracket(wchar_t _symbol, wchar_t _inverse_symbol, bool _closed)
        : symbol(_symbol), inverse_symbol(_inverse_symbol), closed(_closed)
    {
    }

    void bracket::accept(token_visitor *visitor)
    {
        visitor->visit(this);
    }

    bracket *bracket::to_bracket()
    {
        return this;
    }
};
