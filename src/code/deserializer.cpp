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

#include "deserializer.h"
#include "opcode.h"
#include "lib/utils.h"
#include "lib/utf8_encoder.h"
#include "lib/rle.h"
#include "lib/exception.h"
#include <assert.h>
#include <memory.h>
#include "load_string.h"
#include "load_var.h"
#include "call.h"
#include "pop.h"
#include "end.h"
#include "load_integer.h"
#include "decl_var.h"
#include "store.h"
#include "load_real.h"
#include "load_func.h"
#include "create.h"
#include "load_prop.h"
#include "if_not.h"
#include "jmp.h"
#include "add.h"
#include "sub.h"
#include "neg.h"
#include "load_void.h"
#include "load_undefined.h"
#include "load_null.h"
#include "ret.h"
#include "ret_val.h"
#include "load_true.h"
#include "load_false.h"
#include "eq.h"
#include "neq.h"
#include "vcall.h"
#include "this_ptr.h"
#include "clone.h"
#include "instance_of.h"
#include "array.h"
#include "enter.h"
#include "leave.h"
#include "raise.h"
#include "try.h"
#include "catch.h"
#include "finally.h"

namespace g0at
{
    namespace code
    {
        class is_not_binary_file : public lib::exception
        {
        public:
            is_not_binary_file()
                : exception(global::resource->is_not_binary_file())
            {
            }
        };

        class file_is_corrupted : public lib::exception
        {
        public:
            file_is_corrupted()
                : exception(global::resource->file_is_corrupted())
            {
            }
        };

        lib::pointer<code> deserializer::deserialize(std::vector<uint8_t> &buff)
        {
            lib::pointer<code> dst = new code();
            std::vector<uint8_t> buff_decoded;

            if (buff.size() < sizeof(signature))
                throw is_not_binary_file();

            bool uncompressed = memcmp((void*)&buff[0], (void*)signature, sizeof(signature)) == 0;
            bool rle = memcmp((void*)&buff[0], (void*)sign_rle, sizeof(signature)) == 0;
            if (!uncompressed && !rle)
                throw is_not_binary_file();

            if (rle)
            {
                if (false == lib::decode_rle(buff, sizeof(signature), buff_decoded))
                    throw file_is_corrupted();
            }

            source src(rle ? buff_decoded : buff, rle ? 0 : sizeof(signature));
            int32_t i_list_size = pop_int32(&src);
            std::vector<std::wstring> &i_list = dst->get_identifiers_list();
            while(src.has_data() && i_list_size > 0)
            {
                i_list.push_back(pop_wstring(&src));
                i_list_size--;
            }
            if (i_list_size != 0)
                throw file_is_corrupted();
            
            deserializer d_obj;
            while(src.has_data())
            {
                opcode op = pop_opcode(&src);
                creator c = d_obj.get_creator(op);
                c(&src, dst.get());
            }
            return dst;
        }

        deserializer::creator deserializer::get_creator(opcode op)
        {
            auto iter = cc.find(op);
            if(iter == cc.end())
                throw file_is_corrupted();
            return iter->second;
        }

        opcode deserializer::pop_opcode(source *src)
        {
            lib::uint16_converter c;
            c.buff[0] = src->pop();
            c.buff[1] = src->pop();
            return (opcode)c.val;
        }

        uint16_t deserializer::pop_uint16(source *src)
        {
            lib::uint16_converter c;
            c.buff[0] = src->pop();
            c.buff[1] = src->pop();
            return c.val;
        }

        int32_t deserializer::pop_int32(source *src)
        {
            lib::int32_converter c;
            for (int i = 0; i < 4; i++)
            {
                c.buff[i] = src->pop();
            }
            return c.val;
        }

        int64_t deserializer::pop_int64(source *src)
        {
            lib::int64_converter c;
            for (int i = 0; i < 8; i++)
            {
                c.buff[i] = src->pop();
            }
            return c.val;
        }

        double deserializer::pop_double(source *src)
        {
            lib::double_converter c;
            for (int i = 0; i < 8; i++)
            {
                c.buff[i] = src->pop();
            }
            return c.val;
        }

        std::wstring deserializer::pop_wstring(source *src)
        {
            int32_t size = pop_int32(src);
            if (size > 0)
            {
                char *tmp = new char[size + 1];
                tmp[size] = 0;
                for (int32_t i = 0; i < size; i++)
                {
                    tmp[i] = src->pop();
                }
                std::wstring result = lib::decode_utf8(tmp);
                delete tmp;
                return result;
            }
            else
            {
                return L"";                
            }
        }

        void deserializer::c_nop(source *src, code *dst)
        {

        }

        void deserializer::c_load_string(source *src, code *dst)
        {
            int id = pop_int32(src);
            dst->add_instruction(new load_string(id));
        }

        void deserializer::c_load_var(source *src, code *dst)
        {
            int id = pop_int32(src);
            dst->add_instruction(new load_var(id));
        }

        void deserializer::c_call(source *src, code *dst)
        {
            int arg_count = pop_int32(src);
            dst->add_instruction(new call(arg_count));
        }

        void deserializer::c_pop(source *src, code *dst)
        {
            dst->add_instruction(new pop());
        }

        void deserializer::c_end(source *src, code *dst)
        {
            dst->add_instruction(new end());
        }

        void deserializer::c_add(source *src, code *dst)
        {
            dst->add_instruction(new add());
        }

        void deserializer::c_load_integer(source *src, code *dst)
        {
            int64_t value = pop_int64(src);
            dst->add_instruction(new load_integer(value));
        }

        void deserializer::c_sub(source *src, code *dst)
        {
            dst->add_instruction(new sub());
        }

        void deserializer::c_neg(source *src, code *dst)
        {
            dst->add_instruction(new neg());
        }

        void deserializer::c_load_void(source *src, code *dst)
        {
            dst->add_instruction(new load_void());
        }

        void deserializer::c_load_undefined(source *src, code *dst)
        {
            dst->add_instruction(new load_undefined());
        }

        void deserializer::c_load_null(source *src, code *dst)
        {
            dst->add_instruction(new load_null());
        }

        void deserializer::c_decl_var(source *src, code *dst)
        {
            int id = pop_int32(src);
            dst->add_instruction(new decl_var(id));
        }

        void deserializer::c_store(source *src, code *dst)
        {
            int id = pop_int32(src);
            dst->add_instruction(new store(id));
        }

        void deserializer::c_load_real(source *src, code *dst)
        {
            double value = pop_double(src);
            dst->add_instruction(new load_real(value));
        }

        void deserializer::c_load_func(source *src, code *dst)
        {
            int iid = pop_int32(src);
            auto instr = new load_func(iid);
            int arg_count = pop_int32(src);
            for (int i = 0; i < arg_count; i++)
            {
                int arg_id = pop_int32(src);
                instr->add_arg_id(arg_id);
            }
            dst->add_instruction(instr);
        }

        void deserializer::c_ret(source *src, code *dst)
        {
            dst->add_instruction(new ret());
        }

        void deserializer::c_ret_val(source *src, code *dst)
        {
            dst->add_instruction(new ret_val());
        }

        void deserializer::c_create(source *src, code *dst)
        {
            int count = pop_int32(src);
            dst->add_instruction(new create(count));
        }

        void deserializer::c_load_prop(source *src, code *dst)
        {
            int id = pop_int32(src);
            dst->add_instruction(new load_prop(id));
        }

        void deserializer::c_load_true(source *src, code *dst)
        {
            dst->add_instruction(new load_true());
        }

        void deserializer::c_load_false(source *src, code *dst)
        {
            dst->add_instruction(new load_false());
        }

        void deserializer::c_eq(source *src, code *dst)
        {
            dst->add_instruction(new eq());
        }

        void deserializer::c_neq(source *src, code *dst)
        {
            dst->add_instruction(new neq());
        }

        void deserializer::c_if_not(source *src, code *dst)
        {
            int iid = pop_int32(src);
            dst->add_instruction(new if_not(iid));
        }

        void deserializer::c_jmp(source *src, code *dst)
        {
            int iid = pop_int32(src);
            dst->add_instruction(new jmp(iid));
        }

        void deserializer::c_vcall(source *src, code *dst)
        {
            int id = pop_int32(src);
            int arg_count = pop_int32(src);
            dst->add_instruction(new vcall(id, arg_count));
        }

        void deserializer::c_this_ptr(source *src, code *dst)
        {
            dst->add_instruction(new this_ptr());
        }

        void deserializer::c_clone(source *src, code *dst)
        {
            int arg_count = pop_int32(src);
            dst->add_instruction(new clone(arg_count));
        }

        void deserializer::c_instance_of(source *src, code *dst)
        {
            int arg_count = pop_int32(src);
            dst->add_instruction(new instance_of(arg_count));
        }

        void deserializer::c_array(source *src, code *dst)
        {
            int count = pop_int32(src);
            dst->add_instruction(new array(count));
        }

        void deserializer::c_enter(source *src, code *dst)
        {
            dst->add_instruction(new enter());
        }

        void deserializer::c_leave(source *src, code *dst)
        {
            dst->add_instruction(new leave());
        }

        void deserializer::c_raise(source *src, code *dst)
        {
            dst->add_instruction(new raise());
        }

        void deserializer::c_try(source *src, code *dst)
        {
            int iid = pop_int32(src);
            dst->add_instruction(new _try(iid));
        }

        void deserializer::c_catch(source *src, code *dst)
        {
            int id = pop_int32(src);
            dst->add_instruction(new _catch(id));
        }

        void deserializer::c_finally(source *src, code *dst)
        {
            int iid = pop_int32(src);
            dst->add_instruction(new _finally(iid));
        }

        deserializer::deserializer()
        {
            cc[opcode::nop]       = c_nop;
            cc[opcode::sload]     = c_load_string;
            cc[opcode::load]      = c_load_var;
            cc[opcode::call]      = c_call;
            cc[opcode::pop]       = c_pop;
            cc[opcode::end]       = c_end;
            cc[opcode::add]       = c_add;
            cc[opcode::iload]     = c_load_integer;
            cc[opcode::sub]       = c_sub;
            cc[opcode::neg]       = c_neg;
            cc[opcode::void_]     = c_load_void;
            cc[opcode::undefined] = c_load_undefined;
            cc[opcode::null]      = c_load_null;
            cc[opcode::var]       = c_decl_var;
            cc[opcode::store]     = c_store;
            cc[opcode::rload]     = c_load_real;
            cc[opcode::func]      = c_load_func;
            cc[opcode::ret]       = c_ret;
            cc[opcode::retv]      = c_ret_val;
            cc[opcode::create]    = c_create;
            cc[opcode::prop]      = c_load_prop;
            cc[opcode::true_]     = c_load_true;
            cc[opcode::false_]    = c_load_false;
            cc[opcode::eq]        = c_eq;
            cc[opcode::neq]       = c_neq;
            cc[opcode::ifnot]     = c_if_not;
            cc[opcode::jmp]       = c_jmp;
            cc[opcode::vcall]     = c_vcall;
            cc[opcode::this_]     = c_this_ptr;
            cc[opcode::clone]     = c_clone;
            cc[opcode::insof]     = c_instance_of;
            cc[opcode::array]     = c_array;
            cc[opcode::enter]     = c_enter;
            cc[opcode::leave]     = c_leave;
            cc[opcode::raise]     = c_raise;
            cc[opcode::_try]      = c_try;
            cc[opcode::_catch]    = c_catch;
            cc[opcode::_finally]  = c_finally;
        }
    };
};