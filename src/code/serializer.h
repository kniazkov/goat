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

#include "instruction_visitor.h"
#include "op.h"
#include "code.h"
#include "lib/pointer.h"
#include <vector>
#include <cstdint>
#include <string>

namespace g0at
{
    namespace code
    {
        class serializer : public instruction_visitor
        {
        public:
            serializer(std::vector<uint8_t> &_buff);
            static void serialize(lib::pointer<code> code, std::vector<uint8_t> &buff, bool rle);
            void visit(_nop *ref) override;
            void visit(_string *ref) override;
            void visit(_load *ref) override;
            void visit(_call *ref) override;
            void visit(_pop *ref) override;
            void visit(_end *ref) override;
            void visit(_add *ref) override;
            void visit(_integer *ref) override;
            void visit(_sub *ref) override;
            void visit(_neg *ref) override;
            void visit(_void *ref) override;
            void visit(_undef *ref) override;
            void visit(_null *ref) override;
            void visit(_var *ref) override;
            void visit(_store *ref) override;
            void visit(_real *ref) override;
            void visit(_func *ref) override;
            void visit(_ret *ref) override;
            void visit(_retv *ref) override;
            void visit(_object *ref) override;
            void visit(_read *ref) override;
            void visit(_true *ref) override;
            void visit(_false *ref) override;
            void visit(_eq *ref) override;
            void visit(_neq *ref) override;
            void visit(_ifnot *ref) override;
            void visit(_jmp *ref) override;
            void visit(_vcall *ref) override;
            void visit(_this *ref) override;
            void visit(_clone *ref) override;
            void visit(_insof *ref) override;
            void visit(_array *ref) override;
            void visit(_enter *ref) override;
            void visit(_leave *ref) override;
            void visit(_throw *ref) override;
            void visit(_try *ref) override;
            void visit(_catch *ref) override;
            void visit(_finally *ref) override;
            void visit(_inherit *ref) override;
            void visit(_flat *ref) override;
            void visit(_char *ref) override;
            void visit(_less *ref) override;
            void visit(_new *ref) override;
            void visit(_write *ref) override;
            void visit(_inc *ref) override;
            void visit(_thread *ref) override;
            void visit(_rethrow *ref) override;
            void visit(_lock *ref) override;
            void visit(_unlock *ref) override;
            void visit(_get *ref) override;
            void visit(_set *ref) override;
            void visit(_iter *ref) override;
            void visit(_valid *ref) override;
            void visit(_next *ref) override;
            void visit(_dup *ref) override;
            void visit(_if *ref) override;
            void visit(_sector *ref) override;
            void visit(_break *ref) override;
            void visit(_cont *ref) override;
            void visit(_dec *ref) override;
            void visit(_mul *ref) override;
            void visit(_exp *ref) override;
            void visit(_div *ref) override;
            void visit(_mod *ref) override;
            void visit(_pos *ref) override;
            void visit(_not *ref) override;
            void visit(_inv *ref) override;
            void visit(_bool *ref) override;
            void visit(_shl *ref) override;
            void visit(_shr *ref) override;
            void visit(_shrz *ref) override;
            void visit(_leq *ref) override;
            void visit(_great *ref) override;
            void visit(_greq *ref) override;
            void visit(_swap *ref) override;
            void visit(_and *ref) override;
            void visit(_or *ref) override;
            void visit(_bitand *ref) override;
            void visit(_bitor *ref) override;
            void visit(_xor *ref) override;
            void visit(_frame *ref) override;
            void visit(_debug *ref) override;

        protected:
            std::vector<uint8_t> &buff;

            static void push_opcode(std::vector<uint8_t> &buff, op val);
            void push_opcode(op val) { push_opcode(buff, val); }
            
            static void push_uint16(std::vector<uint8_t> &buff, uint16_t val);
            void push_uint16(uint16_t val) { push_uint16(buff, val); }

            static void push_int32(std::vector<uint8_t> &buff, int32_t val);
            void push_int32(int32_t val) { push_int32(buff, val); }

            static void push_int64(std::vector<uint8_t> &buff, int64_t val);
            void push_int64(int64_t val) { push_int64(buff, val); }
            
            static void push_double(std::vector<uint8_t> &buff, double val);
            void push_double(double val) { push_double(buff, val); }

            static void push_wchar(std::vector<uint8_t> &buff, wchar_t val);
            void push_wchar(wchar_t val) { push_wchar(buff, val); }

            static void push_wstring(std::vector<uint8_t> &buff, std::wstring val);
            void push_wstring(std::wstring val) { push_wstring(buff, val); }
        };
    };
};
