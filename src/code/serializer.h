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
#include "opcode.h"
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
            void visit(load_string *ref) override;
            void visit(load_var *ref) override;
            void visit(call *ref) override;
            void visit(pop *ref) override;
            void visit(end *ref) override;
            void visit(add *ref) override;
            void visit(load_integer *ref) override;
            void visit(sub *ref) override;
            void visit(neg *ref) override;
            void visit(load_void *ref) override;
            void visit(load_undefined *ref) override;
            void visit(load_null *ref) override;
            void visit(decl_var *ref) override;
            void visit(store *ref) override;
            void visit(load_real *ref) override;
            void visit(load_func *ref) override;
            void visit(ret *ref) override;
            void visit(ret_val *ref) override;
            void visit(create *ref) override;
            void visit(load_prop *ref) override;
            void visit(load_true *ref) override;
            void visit(load_false *ref) override;
            void visit(eq *ref) override;
            void visit(neq *ref) override;
            void visit(if_not *ref) override;
            void visit(jmp *ref) override;
            void visit(vcall *ref) override;
            void visit(this_ptr *ref) override;
            void visit(clone *ref) override;
            void visit(instance_of *ref) override;
            void visit(array *ref) override;
            void visit(enter *ref) override;
            void visit(leave *ref) override;
            void visit(raise *ref) override;
            void visit(_try *ref) override;
            void visit(_catch *ref) override;
            void visit(_finally *ref) override;

        protected:
            std::vector<uint8_t> &buff;

            static void push_opcode(std::vector<uint8_t> &buff, opcode val);
            void push_opcode(opcode val) { push_opcode(buff, val); }
            
            static void push_uint16(std::vector<uint8_t> &buff, uint16_t val);
            void push_uint16(uint16_t val) { push_uint16(buff, val); }

            static void push_int32(std::vector<uint8_t> &buff, int32_t val);
            void push_int32(int32_t val) { push_int32(buff, val); }

            static void push_int64(std::vector<uint8_t> &buff, int64_t val);
            void push_int64(int64_t val) { push_int64(buff, val); }
            
            static void push_double(std::vector<uint8_t> &buff, double val);
            void push_double(double val) { push_double(buff, val); }

            static void push_wstring(std::vector<uint8_t> &buff, std::wstring val);
            void push_wstring(std::wstring val) { push_wstring(buff, val); }
        };
    };
};
