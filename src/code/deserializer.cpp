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
#include "op.h"
#include "lib/utils.h"
#include "lib/utf8_encoder.h"
#include "lib/rle.h"
#include "lib/exception.h"
#include "lib/assert.h"
#include <memory.h>
#include "nop.h"
#include "string.h"
#include "load.h"
#include "call.h"
#include "pop.h"
#include "end.h"
#include "integer.h"
#include "var.h"
#include "store.h"
#include "real.h"
#include "func.h"
#include "object.h"
#include "read.h"
#include "ifnot.h"
#include "jmp.h"
#include "add.h"
#include "sub.h"
#include "neg.h"
#include "void.h"
#include "undef.h"
#include "null.h"
#include "ret.h"
#include "retv.h"
#include "true.h"
#include "false.h"
#include "eq.h"
#include "neq.h"
#include "vcall.h"
#include "this.h"
#include "clone.h"
#include "insof.h"
#include "array.h"
#include "enter.h"
#include "leave.h"
#include "throw.h"
#include "try.h"
#include "catch.h"
#include "finally.h"
#include "inherit.h"
#include "flat.h"
#include "char.h"
#include "less.h"
#include "new.h"
#include "write.h"
#include "inc.h"
#include "thread.h"
#include "rethrow.h"
#include "lock.h"
#include "unlock.h"
#include "get.h"
#include "set.h"
#include "iter.h"
#include "valid.h"
#include "next.h"
#include "dup.h"
#include "if.h"
#include "cycle.h"

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
                op o = pop_opcode(&src);
                creator c = d_obj.get_creator(o);
                c(&src, dst.get());
            }
            return dst;
        }

        deserializer::creator deserializer::get_creator(op o)
        {
            auto iter = cc.find(o);
            if(iter == cc.end())
                throw file_is_corrupted();
            return iter->second;
        }

        op deserializer::pop_opcode(source *src)
        {
            lib::uint16_converter c;
            c.buff[0] = src->pop();
            c.buff[1] = src->pop();
            return (op)c.val;
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

        wchar_t deserializer::pop_wchar(source *src)
        {
            lib::wchar_converter c;
            for (int i = 0; i < 4; i++)
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
            dst->add_instruction(new _nop());
        }

        void deserializer::c_string(source *src, code *dst)
        {
            int id = pop_int32(src);
            dst->add_instruction(new _string(id));
        }

        void deserializer::c_load(source *src, code *dst)
        {
            int id = pop_int32(src);
            dst->add_instruction(new _load(id));
        }

        void deserializer::c_call(source *src, code *dst)
        {
            int arg_count = pop_int32(src);
            dst->add_instruction(new _call(arg_count));
        }

        void deserializer::c_pop(source *src, code *dst)
        {
            dst->add_instruction(new _pop());
        }

        void deserializer::c_end(source *src, code *dst)
        {
            dst->add_instruction(new _end());
        }

        void deserializer::c_add(source *src, code *dst)
        {
            dst->add_instruction(new _add());
        }

        void deserializer::c_integer(source *src, code *dst)
        {
            int64_t value = pop_int64(src);
            dst->add_instruction(new _integer(value));
        }

        void deserializer::c_sub(source *src, code *dst)
        {
            dst->add_instruction(new _sub());
        }

        void deserializer::c_neg(source *src, code *dst)
        {
            dst->add_instruction(new _neg());
        }

        void deserializer::c_void(source *src, code *dst)
        {
            dst->add_instruction(new _void());
        }

        void deserializer::c_undef(source *src, code *dst)
        {
            dst->add_instruction(new _undef());
        }

        void deserializer::c_null(source *src, code *dst)
        {
            dst->add_instruction(new _null());
        }

        void deserializer::c_var(source *src, code *dst)
        {
            int id = pop_int32(src);
            dst->add_instruction(new _var(id));
        }

        void deserializer::c_store(source *src, code *dst)
        {
            int id = pop_int32(src);
            dst->add_instruction(new _store(id));
        }

        void deserializer::c_real(source *src, code *dst)
        {
            double value = pop_double(src);
            dst->add_instruction(new _real(value));
        }

        void deserializer::c_func(source *src, code *dst)
        {
            iid_t iid(pop_int32(src));
            auto instr = new _func(iid);
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
            dst->add_instruction(new _ret());
        }

        void deserializer::c_retv(source *src, code *dst)
        {
            dst->add_instruction(new _retv());
        }

        void deserializer::c_object(source *src, code *dst)
        {
            int count = pop_int32(src);
            dst->add_instruction(new _object(count));
        }

        void deserializer::c_read(source *src, code *dst)
        {
            int id = pop_int32(src);
            dst->add_instruction(new _read(id));
        }

        void deserializer::c_true(source *src, code *dst)
        {
            dst->add_instruction(new _true());
        }

        void deserializer::c_false(source *src, code *dst)
        {
            dst->add_instruction(new _false());
        }

        void deserializer::c_eq(source *src, code *dst)
        {
            dst->add_instruction(new _eq());
        }

        void deserializer::c_neq(source *src, code *dst)
        {
            dst->add_instruction(new _neq());
        }

        void deserializer::c_ifnot(source *src, code *dst)
        {
            iid_t iid(pop_int32(src));
            dst->add_instruction(new _ifnot(iid));
        }

        void deserializer::c_jmp(source *src, code *dst)
        {
            iid_t iid(pop_int32(src));
            dst->add_instruction(new _jmp(iid));
        }

        void deserializer::c_vcall(source *src, code *dst)
        {
            int id = pop_int32(src);
            int arg_count = pop_int32(src);
            dst->add_instruction(new _vcall(id, arg_count));
        }

        void deserializer::c_this(source *src, code *dst)
        {
            dst->add_instruction(new _this());
        }

        void deserializer::c_clone(source *src, code *dst)
        {
            int arg_count = pop_int32(src);
            dst->add_instruction(new _clone(arg_count));
        }

        void deserializer::c_insof(source *src, code *dst)
        {
            int arg_count = pop_int32(src);
            dst->add_instruction(new _insof(arg_count));
        }

        void deserializer::c_array(source *src, code *dst)
        {
            int count = pop_int32(src);
            dst->add_instruction(new _array(count));
        }

        void deserializer::c_enter(source *src, code *dst)
        {
            dst->add_instruction(new _enter());
        }

        void deserializer::c_leave(source *src, code *dst)
        {
            dst->add_instruction(new _leave());
        }

        void deserializer::c_throw(source *src, code *dst)
        {
            dst->add_instruction(new _throw());
        }

        void deserializer::c_try(source *src, code *dst)
        {
            iid_t iid(pop_int32(src));
            dst->add_instruction(new _try(iid));
        }

        void deserializer::c_catch(source *src, code *dst)
        {
            int id = pop_int32(src);
            dst->add_instruction(new _catch(id));
        }

        void deserializer::c_finally(source *src, code *dst)
        {
            iid_t iid(pop_int32(src));
            dst->add_instruction(new _finally(iid));
        }

        void deserializer::c_inherit(source *src, code *dst)
        {
            dst->add_instruction(new _inherit());
        }

        void deserializer::c_flat(source *src, code *dst)
        {
            int arg_count = pop_int32(src);
            dst->add_instruction(new _flat(arg_count));
        }

        void deserializer::c_char(source *src, code *dst)
        {
            wchar_t value = pop_wchar(src);
            dst->add_instruction(new _char(value));
        }

        void deserializer::c_less(source *src, code *dst)
        {
            dst->add_instruction(new _less());
        }

        void deserializer::c_new(source *src, code *dst)
        {
            int arg_count = pop_int32(src);
            dst->add_instruction(new _new(arg_count));
        }

        void deserializer::c_write(source *src, code *dst)
        {
            int id = pop_int32(src);
            dst->add_instruction(new _write(id));
        }

        void deserializer::c_inc(source *src, code *dst)
        {
            dst->add_instruction(new _inc());
        }

        void deserializer::c_thread(source *src, code *dst)
        {
            iid_t iid(pop_int32(src));
            auto instr = new _thread(iid);
            int arg_count = pop_int32(src);
            for (int i = 0; i < arg_count; i++)
            {
                int arg_id = pop_int32(src);
                instr->add_arg_id(arg_id);
            }
            dst->add_instruction(instr);
        }

        void deserializer::c_rethrow(source *src, code *dst)
        {
            dst->add_instruction(new _rethrow());
        }

        void deserializer::c_lock(source *src, code *dst)
        {
            dst->add_instruction(new _lock());
        }

        void deserializer::c_unlock(source *src, code *dst)
        {
            dst->add_instruction(new _unlock());
        }

        void deserializer::c_get(source *src, code *dst)
        {
            int arg_count = pop_int32(src);
            dst->add_instruction(new _get(arg_count));
        }

        void deserializer::c_set(source *src, code *dst)
        {
            int arg_count = pop_int32(src);
            dst->add_instruction(new _set(arg_count));
        }

        void deserializer::c_iter(source *src, code *dst)
        {
            int arg_count = pop_int32(src);
            dst->add_instruction(new _iter(arg_count));
        }

        void deserializer::c_valid(source *src, code *dst)
        {
            int arg_count = pop_int32(src);
            dst->add_instruction(new _valid(arg_count));
        }

        void deserializer::c_next(source *src, code *dst)
        {
            int arg_count = pop_int32(src);
            dst->add_instruction(new _next(arg_count));
        }

        void deserializer::c_dup(source *src, code *dst)
        {
            dst->add_instruction(new _dup());
        }

        void deserializer::c_if(source *src, code *dst)
        {
            iid_t iid(pop_int32(src));
            dst->add_instruction(new _if(iid));
        }

        void deserializer::c_cycle(source *src, code *dst)
        {
            iid_t begin(pop_int32(src));
            iid_t end(pop_int32(src));
            dst->add_instruction(new _cycle(begin, end));
        }

        deserializer::deserializer()
        {
            cc[op::_nop]     = c_nop;
            cc[op::_string]  = c_string;
            cc[op::_load]    = c_load;
            cc[op::_call]    = c_call;
            cc[op::_pop]     = c_pop;
            cc[op::_end]     = c_end;
            cc[op::_add]     = c_add;
            cc[op::_integer] = c_integer;
            cc[op::_sub]     = c_sub;
            cc[op::_neg]     = c_neg;
            cc[op::_void]    = c_void;
            cc[op::_undef]   = c_undef;
            cc[op::_null]    = c_null;
            cc[op::_var]     = c_var;
            cc[op::_store]   = c_store;
            cc[op::_real]    = c_real;
            cc[op::_func]    = c_func;
            cc[op::_ret]     = c_ret;
            cc[op::_retv]    = c_retv;
            cc[op::_object]  = c_object;
            cc[op::_read]    = c_read;
            cc[op::_true]    = c_true;
            cc[op::_false]   = c_false;
            cc[op::_eq]      = c_eq;
            cc[op::_neq]     = c_neq;
            cc[op::_ifnot]   = c_ifnot;
            cc[op::_jmp]     = c_jmp;
            cc[op::_vcall]   = c_vcall;
            cc[op::_this]    = c_this;
            cc[op::_clone]   = c_clone;
            cc[op::_insof]   = c_insof;
            cc[op::_array]   = c_array;
            cc[op::_enter]   = c_enter;
            cc[op::_leave]   = c_leave;
            cc[op::_throw]   = c_throw;
            cc[op::_try]     = c_try;
            cc[op::_catch]   = c_catch;
            cc[op::_finally] = c_finally;
            cc[op::_inherit] = c_inherit;
            cc[op::_flat]    = c_flat;
            cc[op::_char]    = c_char;
            cc[op::_less]    = c_less;
            cc[op::_new]     = c_new;
            cc[op::_write]   = c_write;
            cc[op::_inc]     = c_inc;
            cc[op::_thread]  = c_thread;
            cc[op::_rethrow] = c_rethrow;
            cc[op::_lock]    = c_lock;
            cc[op::_unlock]  = c_unlock;
            cc[op::_get]     = c_get;
            cc[op::_set]     = c_set;
            cc[op::_iter]    = c_iter;
            cc[op::_valid]   = c_valid;
            cc[op::_next]    = c_next;
            cc[op::_dup]     = c_dup;
            cc[op::_if]      = c_if;
            cc[op::_cycle]   = c_cycle;
        }
    };
};