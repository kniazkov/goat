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

#include "op.h"
#include "code.h"
#include "lib/pointer.h"
#include <vector>
#include <cstdint>
#include <map>

namespace g0at
{
    namespace code
    {
        class deserializer
        {
        public:
            static lib::pointer<code> deserialize(std::vector<uint8_t> &buff);

        protected:
            class source
            {
            public:
                source(std::vector<uint8_t> &_buff, int _idx)
                    : buff(_buff), idx(_idx)
                {
                    max_idx = (int)_buff.size();
                }
                
                bool has_data() { return idx < max_idx; }
                uint8_t pop() { return has_data() ? buff[idx++] : 0; }

            protected:
                std::vector<uint8_t> &buff;
                int idx;
                int max_idx;
            };

            typedef void (*creator)(source*, code*);

            deserializer();
            creator get_creator(op o);
            static op pop_opcode(source *src);
            static uint16_t pop_uint16(source *src);
            static int32_t pop_int32(source *src);
            static int64_t pop_int64(source *src);
            static double pop_double(source *src);
            static wchar_t pop_wchar(source *src);
            static std::wstring pop_wstring(source *src);
            
            static void c_nop(source *src, code *dst);
            static void c_string(source *src, code *dst);
            static void c_load(source *src, code *dst);
            static void c_call(source *src, code *dst);
            static void c_pop(source *src, code *dst);
            static void c_end(source *src, code *dst);
            static void c_add(source *src, code *dst);
            static void c_integer(source *src, code *dst);
            static void c_sub (source *src, code *dst);
            static void c_neg(source *src, code *dst);
            static void c_void(source *src, code *dst);
            static void c_undef(source *src, code *dst);
            static void c_null(source *src, code *dst);
            static void c_var(source *src, code *dst);
            static void c_store(source *src, code *dst);
            static void c_real(source *src, code *dst);
            static void c_func(source *src, code *dst);
            static void c_ret(source *src, code *dst);
            static void c_retv(source *src, code *dst);
            static void c_object(source *src, code *dst);
            static void c_read(source *src, code *dst);
            static void c_true(source *src, code *dst);
            static void c_false(source *src, code *dst);
            static void c_eq(source *src, code *dst);
            static void c_neq(source *src, code *dst);
            static void c_ifnot(source *src, code *dst);
            static void c_jmp(source *src, code *dst);
            static void c_vcall(source *src, code *dst);
            static void c_this(source *src, code *dst);
            static void c_clone(source *src, code *dst);
            static void c_insof(source *src, code *dst);
            static void c_array(source *src, code *dst);
            static void c_enter(source *src, code *dst);
            static void c_leave(source *src, code *dst);
            static void c_throw(source *src, code *dst);
            static void c_try(source *src, code *dst);
            static void c_catch(source *src, code *dst);
            static void c_finally(source *src, code *dst);
            static void c_inherit(source *src, code *dst);
            static void c_flat(source *src, code *dst);
            static void c_char(source *src, code *dst);
            static void c_less(source *src, code *dst);
            static void c_new(source *src, code *dst);
            static void c_write(source *src, code *dst);
            static void c_inc(source *src, code *dst);

            std::map<op, creator> cc;
        };
    };
};
