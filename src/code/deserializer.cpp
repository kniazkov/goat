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
#include "_try.h"
#include "catch.h"

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
            source src(buff);
            lib::pointer<code> dst = new code();
            
            uint8_t header[sizeof(signature)];
            for (unsigned k = 0; k < sizeof(signature); k++)
            {
                header[k] = src.pop();
            }
            if (memcmp((void*)header, (void*)signature, sizeof(signature)) != 0)
                throw is_not_binary_file();
            
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
            auto iter = creators.find(op);
            if(iter == creators.end())
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

        void deserializer::creator_nop(source *src, code *dst)
        {

        }

        void deserializer::creator_load_string(source *src, code *dst)
        {
            int id = pop_int32(src);
            dst->add_instruction(new load_string(id));
        }

        void deserializer::creator_load_var(source *src, code *dst)
        {
            int id = pop_int32(src);
            dst->add_instruction(new load_var(id));
        }

        void deserializer::creator_call(source *src, code *dst)
        {
            int arg_count = pop_int32(src);
            dst->add_instruction(new call(arg_count));
        }

        void deserializer::creator_pop(source *src, code *dst)
        {
            dst->add_instruction(new pop());
        }

        void deserializer::creator_end(source *src, code *dst)
        {
            dst->add_instruction(new end());
        }

        void deserializer::creator_add(source *src, code *dst)
        {
            dst->add_instruction(new add());
        }

        void deserializer::creator_load_integer(source *src, code *dst)
        {
            int64_t value = pop_int64(src);
            dst->add_instruction(new load_integer(value));
        }

        void deserializer::creator_sub(source *src, code *dst)
        {
            dst->add_instruction(new sub());
        }

        void deserializer::creator_neg(source *src, code *dst)
        {
            dst->add_instruction(new neg());
        }

        void deserializer::creator_load_void(source *src, code *dst)
        {
            dst->add_instruction(new load_void());
        }

        void deserializer::creator_load_undefined(source *src, code *dst)
        {
            dst->add_instruction(new load_undefined());
        }

        void deserializer::creator_load_null(source *src, code *dst)
        {
            dst->add_instruction(new load_null());
        }

        void deserializer::creator_decl_var(source *src, code *dst)
        {
            int id = pop_int32(src);
            dst->add_instruction(new decl_var(id));
        }

        void deserializer::creator_store(source *src, code *dst)
        {
            int id = pop_int32(src);
            dst->add_instruction(new store(id));
        }

        void deserializer::creator_load_real(source *src, code *dst)
        {
            double value = pop_double(src);
            dst->add_instruction(new load_real(value));
        }

        void deserializer::creator_load_func(source *src, code *dst)
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

        void deserializer::creator_ret(source *src, code *dst)
        {
            dst->add_instruction(new ret());
        }

        void deserializer::creator_ret_val(source *src, code *dst)
        {
            dst->add_instruction(new ret_val());
        }

        void deserializer::creator_create(source *src, code *dst)
        {
            int count = pop_int32(src);
            dst->add_instruction(new create(count));
        }

        void deserializer::creator_load_prop(source *src, code *dst)
        {
            int id = pop_int32(src);
            dst->add_instruction(new load_prop(id));
        }

        void deserializer::creator_load_true(source *src, code *dst)
        {
            dst->add_instruction(new load_true());
        }

        void deserializer::creator_load_false(source *src, code *dst)
        {
            dst->add_instruction(new load_false());
        }

        void deserializer::creator_eq(source *src, code *dst)
        {
            dst->add_instruction(new eq());
        }

        void deserializer::creator_neq(source *src, code *dst)
        {
            dst->add_instruction(new neq());
        }

        void deserializer::creator_if_not(source *src, code *dst)
        {
            int iid = pop_int32(src);
            dst->add_instruction(new if_not(iid));
        }

        void deserializer::creator_jmp(source *src, code *dst)
        {
            int iid = pop_int32(src);
            dst->add_instruction(new jmp(iid));
        }

        void deserializer::creator_vcall(source *src, code *dst)
        {
            int id = pop_int32(src);
            int arg_count = pop_int32(src);
            dst->add_instruction(new vcall(id, arg_count));
        }

        void deserializer::creator_this_ptr(source *src, code *dst)
        {
            dst->add_instruction(new this_ptr());
        }

        void deserializer::creator_clone(source *src, code *dst)
        {
            int arg_count = pop_int32(src);
            dst->add_instruction(new clone(arg_count));
        }

        void deserializer::creator_instance_of(source *src, code *dst)
        {
            int arg_count = pop_int32(src);
            dst->add_instruction(new instance_of(arg_count));
        }

        void deserializer::creator_array(source *src, code *dst)
        {
            int count = pop_int32(src);
            dst->add_instruction(new array(count));
        }

        void deserializer::creator_enter(source *src, code *dst)
        {
            dst->add_instruction(new enter());
        }

        void deserializer::creator_leave(source *src, code *dst)
        {
            dst->add_instruction(new leave());
        }

        void deserializer::creator_raise(source *src, code *dst)
        {
            dst->add_instruction(new raise());
        }

        void deserializer::creator_try(source *src, code *dst)
        {
            int iid = pop_int32(src);
            dst->add_instruction(new _try(iid));
        }

        void deserializer::creator_catch(source *src, code *dst)
        {
            int id = pop_int32(src);
            dst->add_instruction(new _catch(id));
        }

        deserializer::deserializer()
        {
            creators[opcode::nop]       = creator_nop;
            creators[opcode::sload]     = creator_load_string;
            creators[opcode::load]      = creator_load_var;
            creators[opcode::call]      = creator_call;
            creators[opcode::pop]       = creator_pop;
            creators[opcode::end]       = creator_end;
            creators[opcode::add]       = creator_add;
            creators[opcode::iload]     = creator_load_integer;
            creators[opcode::sub]       = creator_sub;
            creators[opcode::neg]       = creator_neg;
            creators[opcode::void_]     = creator_load_void;
            creators[opcode::undefined] = creator_load_undefined;
            creators[opcode::null]      = creator_load_null;
            creators[opcode::var]       = creator_decl_var;
            creators[opcode::store]     = creator_store;
            creators[opcode::rload]     = creator_load_real;
            creators[opcode::func]      = creator_load_func;
            creators[opcode::ret]       = creator_ret;
            creators[opcode::retv]      = creator_ret_val;
            creators[opcode::create]    = creator_create;
            creators[opcode::prop]      = creator_load_prop;
            creators[opcode::true_]     = creator_load_true;
            creators[opcode::false_]    = creator_load_false;
            creators[opcode::eq]        = creator_eq;
            creators[opcode::neq]       = creator_neq;
            creators[opcode::ifnot]     = creator_if_not;
            creators[opcode::jmp]       = creator_jmp;
            creators[opcode::vcall]     = creator_vcall;
            creators[opcode::this_]     = creator_this_ptr;
            creators[opcode::clone]     = creator_clone;
            creators[opcode::insof]     = creator_instance_of;
            creators[opcode::array]     = creator_array;
            creators[opcode::enter]     = creator_enter;
            creators[opcode::leave]     = creator_leave;
            creators[opcode::raise]     = creator_raise;
            creators[opcode::_try]      = creator_try;
            creators[opcode::_catch]    = creator_catch;
        }
    };
};