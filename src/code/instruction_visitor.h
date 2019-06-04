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

namespace g0at
{
    namespace code
    {
        class _nop;
        class _string;
        class _load;
        class _call;
        class _pop;
        class _end;
        class _add;
        class _integer;
        class _sub;
        class _neg;
        class _void;
        class _undef;
        class _null;
        class _var;
        class _store;
        class _real;
        class _func;
        class _ret;
        class _retv;
        class _object;
        class _read;
        class _true;
        class _false;
        class _eq;
        class _neq;
        class _ifnot;
        class _jmp;
        class _vcall;
        class _this;
        class _clone;
        class _insof;
        class _array;
        class _enter;
        class _leave;
        class _throw;
        class _try;
        class _catch;
        class _finally;
        class _inherit;
        class _flat;
        class _char;
        class _less;
        class _new;
        class _write;

        class instruction_visitor
        {
        public:
            instruction_visitor();
            ~instruction_visitor();
            virtual void visit(_nop *ref) = 0;
            virtual void visit(_string *ref) = 0;
            virtual void visit(_load *ref) = 0;
            virtual void visit(_call *ref) = 0;
            virtual void visit(_pop *ref) = 0;
            virtual void visit(_end *ref) = 0;
            virtual void visit(_add *ref) = 0;
            virtual void visit(_integer *ref) = 0;
            virtual void visit(_sub *ref) = 0;
            virtual void visit(_neg *ref) = 0;
            virtual void visit(_void *ref) = 0;
            virtual void visit(_undef *ref) = 0;
            virtual void visit(_null *ref) = 0;
            virtual void visit(_var *ref) = 0;
            virtual void visit(_store *ref) = 0;
            virtual void visit(_real *ref) = 0;
            virtual void visit(_func *ref) = 0;
            virtual void visit(_ret *ref) = 0;
            virtual void visit(_retv *ref) = 0;
            virtual void visit(_object *ref) = 0;
            virtual void visit(_read *ref) = 0;
            virtual void visit(_true *ref) = 0;
            virtual void visit(_false *ref) = 0;
            virtual void visit(_eq *ref) = 0;
            virtual void visit(_neq *ref) = 0;
            virtual void visit(_ifnot *ref) = 0;
            virtual void visit(_jmp *ref) = 0;
            virtual void visit(_vcall *ref) = 0;
            virtual void visit(_this *ref) = 0;
            virtual void visit(_clone *ref) = 0;
            virtual void visit(_insof *ref) = 0;
            virtual void visit(_array *ref) = 0;
            virtual void visit(_enter *ref) = 0;
            virtual void visit(_leave *ref) = 0;
            virtual void visit(_throw *ref) = 0;
            virtual void visit(_try *ref) = 0;
            virtual void visit(_catch *ref) = 0;
            virtual void visit(_finally *ref) = 0;
            virtual void visit(_inherit *ref) = 0;
            virtual void visit(_flat *ref) = 0;
            virtual void visit(_char *ref) = 0;
            virtual void visit(_less *ref) = 0;
            virtual void visit(_new *ref) = 0;
            virtual void visit(_write *ref) = 0;
        };
    };
};
