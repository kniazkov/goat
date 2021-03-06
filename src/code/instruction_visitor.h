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
        class _inc;
        class _thread;
        class _rethrow;
        class _lock;
        class _unlock;
        class _get;
        class _set;
        class _iter;
        class _valid;
        class _next;
        class _dup;
        class _if;
        class _sector;
        class _break;
        class _cont;
        class _dec;
        class _mul;
        class _div;
        class _mod;
        class _exp;
        class _pos;
        class _not;
        class _inv;
        class _bool;
        class _shl;
        class _shr;
        class _shrz;
        class _leq;
        class _great;
        class _greq;
        class _swap;
        class _and;
        class _or;
        class _bitand;
        class _bitor;
        class _xor;
        class _frame;
        class _debug;
        class _protect;

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
            virtual void visit(_inc *ref) = 0;
            virtual void visit(_thread *ref) = 0;
            virtual void visit(_rethrow *ref) = 0;
            virtual void visit(_lock *ref) = 0;
            virtual void visit(_unlock *ref) = 0;
            virtual void visit(_get *ref) = 0;
            virtual void visit(_set *ref) = 0;
            virtual void visit(_iter *ref) = 0;
            virtual void visit(_valid *ref) = 0;
            virtual void visit(_next *ref) = 0;
            virtual void visit(_dup *ref) = 0;
            virtual void visit(_if *ref) = 0;
            virtual void visit(_sector *ref) = 0;
            virtual void visit(_break *ref) = 0;
            virtual void visit(_cont *ref) = 0;
            virtual void visit(_dec *ref) = 0;
            virtual void visit(_mul *ref) = 0;
            virtual void visit(_exp *ref) = 0;
            virtual void visit(_div *ref) = 0;
            virtual void visit(_mod *ref) = 0;
            virtual void visit(_pos *ref) = 0;
            virtual void visit(_not *ref) = 0;
            virtual void visit(_inv *ref) = 0;
            virtual void visit(_bool *ref) = 0;
            virtual void visit(_shl *ref) = 0;
            virtual void visit(_shr *ref) = 0;
            virtual void visit(_shrz *ref) = 0;
            virtual void visit(_leq *ref) = 0;
            virtual void visit(_great *ref) = 0;
            virtual void visit(_greq *ref) = 0;
            virtual void visit(_swap *ref) = 0;
            virtual void visit(_and *ref) = 0;
            virtual void visit(_or *ref) = 0;
            virtual void visit(_bitand *ref) = 0;
            virtual void visit(_bitor *ref) = 0;
            virtual void visit(_xor *ref) = 0;
            virtual void visit(_frame *ref) = 0;
            virtual void visit(_debug *ref) = 0;
            virtual void visit(_protect *ref) = 0;
        };
    };
};
