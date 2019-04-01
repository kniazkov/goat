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

#include <cstdint>

namespace g0at
{
    namespace code
    {
        enum class op : uint16_t
        {
            // various
            _nop,
            _end,

            // stack
            _pop,

            // load instructions
            _load,
            _prop,
            _string,
            _integer,
            _real,
            _func,

            _void,
            _undef,
            _null,
            _true,
            _false,

            _this,

            // store instructions
            _var,
            _store,

            // creators
            _object,
            _array,

            // branching
            _call,
            _vcall,
            _ret,
            _retv,
            _throw,
            _ifnot,
            _jmp,
            _enter,
            _leave,
            _try,
            _catch,
            _finally,

            // methods
            _clone,
            _insof,

            // operators
            _add,
            _sub,
            _neg,

            _eq,
            _neq
        };
    };
};