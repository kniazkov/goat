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
            _dup,
            _swap,

            // load instructions
            _load,
            _read,
            _char,
            _string,
            _integer,
            _real,
            _func,
            _thread,

            _void,
            _undef,
            _null,
            _true,
            _false,

            _this,

            // store instructions
            _var,
            _store,
            _write,

            // creators
            _new,
            _object,
            _array,

            // branching
            _call,
            _vcall,
            _ret,
            _retv,
            _throw,
            _rethrow,
            _if,
            _ifnot,
            _jmp,

            // context
            _enter,
            _leave,
            _try,
            _catch,
            _finally,
            _sector,
            _break,
            _cont,

            // methods
            _get,
            _set,
            _clone,
            _insof,
            _flat,
            _iter,
            _valid,
            _next,

            // operators
            _add,
            _sub,
            _pos,
            _neg,
            _inc,
            _dec,
            _mul,
            _exp,
            _div,
            _mod,
            _bool,
            _not,
            _inv,
            _shl,
            _shr,
            _shrz,
            _eq,
            _neq,
            _less,
            _leq,
            _great,
            _greq,
            _and,
            _or,
            _bitand,
            _bitor,
            _xor,
            _inherit,
            _protect,

            // multithreading
            _lock,
            _unlock,

            // debug
            _frame,
            _debug
        };
    };
};
