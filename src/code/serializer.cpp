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
#include "opcode.h"
#include "lib/utils.h"
#include "lib/utf8_encoder.h"
#include <assert.h>
#include "load_string.h"
#include "load_var.h"
#include "call.h"
#include "pop.h"
#include "load_integer.h"
#include "decl_var.h"
#include "store.h"
#include "load_real.h"
#include "load_func.h"
#include "create.h"
#include "load_prop.h"
#include "if_not.h"
#include "jmp.h"
#include "vcall.h"
#include "clone.h"
#include "instance_of.h"
#include "array.h"

namespace g0at
{
    namespace code
    {
        void serializer::serialize(lib::pointer<code> code, std::vector<uint8_t> &buff)
        {
            buff.clear();
            for (unsigned k = 0; k < sizeof(signature); k++)
            {
                buff.push_back(signature[k]);
            }
            int i;

            auto i_list = code->get_identifiers_list();
            int i_list_size = (int)i_list.size();
            push_int32(buff, i_list_size);
            for (i = 0; i < i_list_size; i++)
            {
                push_wstring(buff, i_list[i]);
            }

            serializer visitor(buff);
            int code_size = code->get_code_size();
            for (i = 0; i < code_size; i++)
            {
                code->get_instruction(i)->accept(&visitor);
            }
        }

        serializer::serializer(std::vector<uint8_t> &_buff)
            : buff(_buff)
        {
        }

        void serializer::push_opcode(std::vector<uint8_t> &buff, opcode val)
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

        void serializer::visit(load_string *ref)
        {
            push_opcode(opcode::sload);
            push_int32(ref->get_id());
        }

        void serializer::visit(load_var *ref)
        {
            push_opcode(opcode::load);
            push_int32(ref->get_id());
        }

        void serializer::visit(call *ref)
        {
            push_opcode(opcode::call);
            push_int32(ref->get_arg_count());
        }

        void serializer::visit(pop *ref)
        {
            push_opcode(opcode::pop);
        }

        void serializer::visit(end *ref)
        {
            push_opcode(opcode::end);
        }

        void serializer::visit(add *ref)
        {
            push_opcode(opcode::add);
        }

        void serializer::visit(load_integer *ref)
        {
            push_opcode(opcode::iload);
            push_int64(ref->get_value());
        }

        void serializer::visit(sub *ref)
        {
            push_opcode(opcode::sub);
        }

        void serializer::visit(neg *ref)
        {
            push_opcode(opcode::neg);
        }

        void serializer::visit(load_void *ref)
        {
            push_opcode(opcode::void_);
        }

        void serializer::visit(load_undefined *ref)
        {
            push_opcode(opcode::undefined);
        }

        void serializer::visit(load_null *ref)
        {
            push_opcode(opcode::null);
        }

        void serializer::visit(decl_var *ref)
        {
            push_opcode(opcode::var);
            push_int32(ref->get_id());
        }

        void serializer::visit(store *ref)
        {
            push_opcode(opcode::store);
            push_int32(ref->get_id());
        }

        void serializer::visit(load_real *ref)
        {
            push_opcode(opcode::rload);
            push_double(ref->get_value());
        }

        void serializer::visit(load_func *ref)
        {
            push_opcode(opcode::func);
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

        void serializer::visit(ret *ref)
        {
            push_opcode(opcode::ret);
        }

        void serializer::visit(ret_val *ref)
        {
            push_opcode(opcode::retv);
        }

        void serializer::visit(create *ref)
        {
            push_opcode(opcode::create);
            push_int32(ref->get_count());
        }

        void serializer::visit(load_prop *ref)
        {
            push_opcode(opcode::prop);
            push_int32(ref->get_id());
        }

        void serializer::visit(load_true *ref)
        {
            push_opcode(opcode::true_);
        }

        void serializer::visit(load_false *ref)
        {
            push_opcode(opcode::false_);
        }

        void serializer::visit(eq *ref)
        {
            push_opcode(opcode::eq);
        }

        void serializer::visit(neq *ref)
        {
            push_opcode(opcode::neq);
        }

        void serializer::visit(if_not *ref)
        {
            push_opcode(opcode::ifnot);
            push_int32(ref->get_iid());
        }

        void serializer::visit(jmp *ref)
        {
            push_opcode(opcode::jmp);
            push_int32(ref->get_iid());
        }

        void serializer::visit(vcall *ref)
        {
            push_opcode(opcode::vcall);
            push_int32(ref->get_id());
            push_int32(ref->get_arg_count());
        }

        void serializer::visit(this_ptr *ref)
        {
            push_opcode(opcode::this_);
        }

        void serializer::visit(clone *ref)
        {
            push_opcode(opcode::clone);
            push_int32(ref->get_arg_count());
        }

        void serializer::visit(instance_of *ref)
        {
            push_opcode(opcode::insof);
            push_int32(ref->get_arg_count());
        }

        void serializer::visit(array *ref)
        {
            push_opcode(opcode::array);
            push_int32(ref->get_count());
        }

        void serializer::visit(enter *ref)
        {
            push_opcode(opcode::enter);
        }

        void serializer::visit(leave *ref)
        {
            push_opcode(opcode::leave);
        }

        void serializer::visit(raise *ref)
        {
            push_opcode(opcode::raise);
        }
    };
};