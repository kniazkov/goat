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

#include "serializer.h"
#include "op.h"
#include "lib/utils.h"
#include "lib/utf8_encoder.h"
#include "lib/rle.h"
#include "lib/assert.h"
#include "string.h"
#include "load.h"
#include "call.h"
#include "pop.h"
#include "integer.h"
#include "var.h"
#include "store.h"
#include "real.h"
#include "func.h"
#include "object.h"
#include "read.h"
#include "ifnot.h"
#include "jmp.h"
#include "vcall.h"
#include "clone.h"
#include "insof.h"
#include "array.h"
#include "try.h"
#include "catch.h"
#include "finally.h"
#include "flat.h"
#include "char.h"

namespace g0at
{
    namespace code
    {
        void serializer::serialize(lib::pointer<code> code, std::vector<uint8_t> &buff, bool rle)
        {
            std::vector<uint8_t> buff_raw;
            std::vector<uint8_t> &dst = rle ? buff_raw : buff;
            buff.clear();
            
            const char *start = rle ? sign_rle : signature;
            for (unsigned k = 0; k < sizeof(signature); k++)
            {
                buff.push_back(start[k]);
            }

            int i;

            auto i_list = code->get_identifiers_list();
            int i_list_size = (int)i_list.size();
            push_int32(dst, i_list_size);
            for (i = 0; i < i_list_size; i++)
            {
                push_wstring(dst, i_list[i]);
            }

            serializer visitor(dst);
            int code_size = code->get_code_size();
            for (i = 0; i < code_size; i++)
            {
                code->get_instruction(i)->accept(&visitor);
            }

            if (rle)
            {
                lib::encode_rle(buff_raw, buff);
            }
        }

        serializer::serializer(std::vector<uint8_t> &_buff)
            : buff(_buff)
        {
        }

        void serializer::push_opcode(std::vector<uint8_t> &buff, op val)
        {
            lib::uint16_converter c;
            c.val = (uint16_t)val;
            buff.push_back(c.buff[0]);
            buff.push_back(c.buff[1]);
        }

        void serializer::push_uint16(std::vector<uint8_t> &buff, uint16_t val)
        {
            lib::uint16_converter c;
            c.val = val;
            buff.push_back(c.buff[0]);
            buff.push_back(c.buff[1]);
        }

        void serializer::push_int32(std::vector<uint8_t> &buff, int32_t val)
        {
            lib::int32_converter c;
            c.val = val;
            buff.insert(buff.end(), c.buff, c.buff + 4);
        }

        void serializer::push_int64(std::vector<uint8_t> &buff, int64_t val)
        {
            lib::int64_converter c;
            c.val = val;
            buff.insert(buff.end(), c.buff, c.buff + 8);
        }

        void serializer::push_double(std::vector<uint8_t> &buff, double val)
        {
            lib::double_converter c;
            c.val = val;
            buff.insert(buff.end(), c.buff, c.buff + 8);
        }

        void serializer::push_wchar(std::vector<uint8_t> &buff, wchar_t val)
        {
            lib::wchar_converter c = {0};
            c.val = val;
            buff.insert(buff.end(), c.buff, c.buff + 4);
        }

        void serializer::push_wstring(std::vector<uint8_t> &buff, std::wstring val)
        {
            std::string tmp = lib::encode_utf8(val);
            size_t size = tmp.size();
            assert(size < INT32_MAX);
            push_int32(buff, (int32_t)size);
            for (size_t i = 0; i < size; i++)
            {
                buff.push_back(tmp[i]);
            }
        }

        void serializer::visit(_nop *ref)
        {
            push_opcode(op::_nop);
        }

        void serializer::visit(_string *ref)
        {
            push_opcode(op::_string);
            push_int32(ref->get_id());
        }

        void serializer::visit(_load *ref)
        {
            push_opcode(op::_load);
            push_int32(ref->get_id());
        }

        void serializer::visit(_call *ref)
        {
            push_opcode(op::_call);
            push_int32(ref->get_arg_count());
        }

        void serializer::visit(_pop *ref)
        {
            push_opcode(op::_pop);
        }

        void serializer::visit(_end *ref)
        {
            push_opcode(op::_end);
        }

        void serializer::visit(_add *ref)
        {
            push_opcode(op::_add);
        }

        void serializer::visit(_integer *ref)
        {
            push_opcode(op::_integer);
            push_int64(ref->get_value());
        }

        void serializer::visit(_sub *ref)
        {
            push_opcode(op::_sub);
        }

        void serializer::visit(_neg *ref)
        {
            push_opcode(op::_neg);
        }

        void serializer::visit(_void *ref)
        {
            push_opcode(op::_void);
        }

        void serializer::visit(_undef *ref)
        {
            push_opcode(op::_undef);
        }

        void serializer::visit(_null *ref)
        {
            push_opcode(op::_null);
        }

        void serializer::visit(_var *ref)
        {
            push_opcode(op::_var);
            push_int32(ref->get_id());
        }

        void serializer::visit(_store *ref)
        {
            push_opcode(op::_store);
            push_int32(ref->get_id());
        }

        void serializer::visit(_real *ref)
        {
            push_opcode(op::_real);
            push_double(ref->get_value());
        }

        void serializer::visit(_func *ref)
        {
            push_opcode(op::_func);
            int iid = ref->get_first_iid();
            assert(iid >= 0);
            push_int32(iid);
            int count = ref->get_arg_ids_count();
            push_int32(count);
            for (int i = 0; i < count; i++)
            {
                push_int32(ref->get_arg_id(i));
            }
        }

        void serializer::visit(_ret *ref)
        {
            push_opcode(op::_ret);
        }

        void serializer::visit(_retv *ref)
        {
            push_opcode(op::_retv);
        }

        void serializer::visit(_object *ref)
        {
            push_opcode(op::_object);
            push_int32(ref->get_count());
        }

        void serializer::visit(_read *ref)
        {
            push_opcode(op::_read);
            push_int32(ref->get_id());
        }

        void serializer::visit(_true *ref)
        {
            push_opcode(op::_true);
        }

        void serializer::visit(_false *ref)
        {
            push_opcode(op::_false);
        }

        void serializer::visit(_eq *ref)
        {
            push_opcode(op::_eq);
        }

        void serializer::visit(_neq *ref)
        {
            push_opcode(op::_neq);
        }

        void serializer::visit(_ifnot *ref)
        {
            push_opcode(op::_ifnot);
            push_int32(ref->get_iid());
        }

        void serializer::visit(_jmp *ref)
        {
            push_opcode(op::_jmp);
            push_int32(ref->get_iid());
        }

        void serializer::visit(_vcall *ref)
        {
            push_opcode(op::_vcall);
            push_int32(ref->get_id());
            push_int32(ref->get_arg_count());
        }

        void serializer::visit(_this *ref)
        {
            push_opcode(op::_this);
        }

        void serializer::visit(_clone *ref)
        {
            push_opcode(op::_clone);
            push_int32(ref->get_arg_count());
        }

        void serializer::visit(_insof *ref)
        {
            push_opcode(op::_insof);
            push_int32(ref->get_arg_count());
        }

        void serializer::visit(_array *ref)
        {
            push_opcode(op::_array);
            push_int32(ref->get_count());
        }

        void serializer::visit(_enter *ref)
        {
            push_opcode(op::_enter);
        }

        void serializer::visit(_leave *ref)
        {
            push_opcode(op::_leave);
        }

        void serializer::visit(_throw *ref)
        {
            push_opcode(op::_throw);
        }

        void serializer::visit(_try *ref)
        {
            push_opcode(op::_try);
            push_int32(ref->get_iid());
        }

        void serializer::visit(_catch *ref)
        {
            push_opcode(op::_catch);
            push_int32(ref->get_id());
        }

        void serializer::visit(_finally *ref)
        {
            push_opcode(op::_finally);
            push_int32(ref->get_iid());
        }

        void serializer::visit(_inherit *ref)
        {
            push_opcode(op::_inherit);
        }

        void serializer::visit(_flat *ref)
        {
            push_opcode(op::_flat);
            push_int32(ref->get_arg_count());
        }

        void serializer::visit(_char *ref)
        {
            push_opcode(op::_char);
            push_wchar(ref->get_value());
        }

        void serializer::visit(_less *ref)
        {
            push_opcode(op::_less);
        }

        void serializer::visit(_new *ref)
        {
            push_opcode(op::_new);
        }
    };
};