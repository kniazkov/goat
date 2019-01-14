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

#include "token.h"

namespace g0at
{
    token::token()
        : list(nullptr), prev(nullptr), next(nullptr)
    {
    }

    token::~token()
    {
    }

    identifier *token::to_identifier()
    {
        return nullptr;
    }

    bracket *token::to_bracket()
    {
        return nullptr;
    }

    token_static_string *token::to_static_string()
    {
        return nullptr;
    }

    semicolon *token::to_semicolon()
    {
        return nullptr;
    }
};
