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

#include "disasm.h"
#include "lib/utils.h"
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
#include "new.h"
#include "write.h"
#include "thread.h"
#include "get.h"
#include "set.h"
#include "iter.h"
#include "valid.h"
#include "next.h"
#include "if.h"
#include "sector.h"
#include "frame.h"


namespace g0at
{
    namespace code
    {
        disasm::disasm(std::wstringstream &_stream, std::vector<std::wstring> &_identifiers)
            : stream(_stream), identifiers(_identifiers)
        {
        }

        std::wstring disasm::to_string(lib::pointer<code> _code, bool _print_data_segment)
        {
            std::wstringstream tmp;
            int i, size;
            auto i_list = _code->get_identifiers_list();
            if (_print_data_segment)
            {
                tmp << L".data\n";
                for (i = 0, size = (int)i_list.size(); i < size; i++)
                {
                    tmp << L"  " << i << L"\t" << i_list[i] << "\n";
                }
                tmp << "\n";
            }
            tmp << L".code\n";
            disasm da(tmp, i_list);
            iid_t last_iid = _code->get_current_iid();
            for (iid_t j = iid_t(0); j < last_iid; ++j)
            {
                if (j.as_int() > 0)
                {
                    tmp << L"\n";
                    if (j.as_int() % 5 == 0)
                        tmp << L"  " << j.as_int();
                    }
                tmp << L"\t";
                _code->get_instruction(j)->accept(&da);
            }
            tmp << "\n";
            return tmp.str();
        }

        std::wstring disasm::to_string(lib::pointer<code> _code, iid_t first, iid_t last)
        {
            std::wstringstream tmp;
            auto i_list = _code->get_identifiers_list();
            disasm da(tmp, i_list);
            iid_t max_iid = _code->get_current_iid();
            for (iid_t j = first; j < last && j < max_iid; ++j)
            {
                if (j.as_int() < 0)
                    continue;
                if (j == first || j.as_int() > 0)
                {
                    tmp << L"\n";
                    if (j.as_int() % 5 == 0)
                        tmp << L"  " << j.as_int();
                    }
                tmp << L"\t";
                _code->get_instruction(j)->accept(&da);
            }
            tmp << "\n";
            return tmp.str();
        }

        void disasm::visit(_nop *ref)
        {
            stream << L"nop";
        }

        void disasm::visit(_string *ref)
        {
            int id = ref->get_id();
            stream << L"string\t" << id << L"\t; \"" <<  lib::escape_special_chars(identifiers.at(id)) << '"';
        }

        void disasm::visit(_load *ref)
        {
            int id = ref->get_id();
            stream << L"load\t" << id << L"\t; " << identifiers.at(id);
        }

        void disasm::visit(_call *ref)
        {
            stream << L"call\t" << ref->get_arg_count();
        }

        void disasm::visit(_pop *ref)
        {
            stream << L"pop";
        }

        void disasm::visit(_end *ref)
        {
            stream << L"end";
        }

        void disasm::visit(_add *ref)
        {
            stream << L"add";
        }

        void disasm::visit(_integer *ref)
        {
            stream << L"integer\t" << ref->get_value();
        }

        void disasm::visit(_sub *ref)
        {
            stream << L"sub";
        }

        void disasm::visit(_neg *ref)
        {
            stream << L"neg";
        }

        void disasm::visit(_void *ref)
        {
            stream << L"void";
        }

        void disasm::visit(_undef *ref)
        {
            stream << L"undef";
        }

        void disasm::visit(_null *ref)
        {
            stream << L"null";
        }

        void disasm::visit(_var *ref)
        {
            int id = ref->get_id();
            stream << L"var\t" << id << L"\t; " << identifiers.at(id);
        }

        void disasm::visit(_store *ref)
        {
            int id = ref->get_id();
            stream << L"store\t" << id << L"\t; " << identifiers.at(id);
        }

        void disasm::visit(_real *ref)
        {
            stream << L"real\t" << lib::double_to_wstring(ref->get_value());
        }

        void disasm::visit(_func *ref)
        {
            int i, size = ref->get_arg_ids_count();
            stream << L"func\t^" << ref->get_first_iid().as_int();
            for (i = 0; i < size; i++)
            {
                stream  << L", " << ref->get_arg_id(i);
            }
            if (size > 0)
            {
                stream << L"\t; ";
                for (i = 0; i < size; i++)
                {
                    if (i)
                        stream << L", ";
                    stream << identifiers.at(ref->get_arg_id(i));
                }
            }
        }

        void disasm::visit(_ret *ref)
        {
            stream << L"ret";
        }

        void disasm::visit(_retv *ref)
        {
            stream << L"retv";
        }

        void disasm::visit(_object *ref)
        {
            stream << L"object\t" << ref->get_count();
        }

        void disasm::visit(_read *ref)
        {
            int id = ref->get_id();
            stream << L"read\t" << id << L"\t; " << identifiers.at(id);
        }

        void disasm::visit(_true *ref)
        {
            stream << L"true";
        }

        void disasm::visit(_false *ref)
        {
            stream << L"false";
        }

        void disasm::visit(_eq *ref)
        {
            stream << L"eq";
        }

        void disasm::visit(_neq *ref)
        {
            stream << L"neq";
        }

        void disasm::visit(_ifnot *ref)
        {
            stream << L"ifnot\t^" << ref->get_iid().as_int();
        }

        void disasm::visit(_jmp *ref)
        {
            stream << L"jmp\t^" << ref->get_iid().as_int();
        }

        void disasm::visit(_vcall *ref)
        {
            int id = ref->get_id();
            stream << L"vcall\t" << id << L", " << ref->get_arg_count() << L"\t; " << identifiers.at(id);
        }

        void disasm::visit(_this *ref)
        {
            stream << L"this";
        }

        void disasm::visit(_clone *ref)
        {
            stream << L"clone\t" << ref->get_arg_count();
        }

        void disasm::visit(_insof *ref)
        {
            stream << L"insof\t" << ref->get_arg_count();
        }

        void disasm::visit(_array *ref)
        {
            stream << L"array\t" << ref->get_count();
        }

        void disasm::visit(_enter *ref)
        {
            stream << L"enter";
        }

        void disasm::visit(_leave *ref)
        {
            stream << L"leave";
        }

        void disasm::visit(_throw *ref)
        {
            stream << L"throw";
        }

        void disasm::visit(_try *ref)
        {
            stream << L"try\t^" << ref->get_iid().as_int();
        }

        void disasm::visit(_catch *ref)
        {
            int id = ref->get_id();
            stream << L"catch\t" << id << L"\t; " << identifiers.at(id);
        }

        void disasm::visit(_finally *ref)
        {
            stream << L"finally\t^" << ref->get_iid().as_int();
        }

        void disasm::visit(_inherit *ref)
        {
            stream << L"inherit";
        }

        void disasm::visit(_flat *ref)
        {
            stream << L"flat\t" << ref->get_arg_count();
        }

        void disasm::visit(_char *ref)
        {
            wchar_t value = ref->get_value();
            stream << L"char\t'" << lib::escape_special_chars(&value, 1) << L'\'';
        }

        void disasm::visit(_less *ref)
        {
            stream << L"less";
        }

        void disasm::visit(_new *ref)
        {
            stream << L"new\t" << ref->get_arg_count();
        }

        void disasm::visit(_write *ref)
        {
            int id = ref->get_id();
            stream << L"write\t" << id << L"\t; " << identifiers.at(id);
        }

        void disasm::visit(_inc *ref)
        {
            stream << L"inc";
        }

        void disasm::visit(_thread *ref)
        {
            int i, size = ref->get_arg_ids_count();
            stream << L"thread\t^" << ref->get_first_iid().as_int();
            for (i = 0; i < size; i++)
            {
                stream  << L", " << ref->get_arg_id(i);
            }
            if (size > 0)
            {
                stream << L"\t; ";
                for (i = 0; i < size; i++)
                {
                    if (i)
                        stream << L", ";
                    stream << identifiers.at(ref->get_arg_id(i));
                }
            }
        }

        void disasm::visit(_rethrow *ref)
        {
            stream << L"rethrow";
        }

        void disasm::visit(_lock *ref)
        {
            stream << L"lock";
        }

        void disasm::visit(_unlock *ref)
        {
            stream << L"unlock";
        }

        void disasm::visit(_get *ref)
        {
            stream << L"get\t" << ref->get_arg_count();
        }

        void disasm::visit(_set *ref)
        {
            stream << L"set\t" << ref->get_arg_count();
        }

        void disasm::visit(_iter *ref)
        {
            stream << L"iter\t" << ref->get_arg_count();
        }

        void disasm::visit(_valid *ref)
        {
            stream << L"valid\t" << ref->get_arg_count();
        }

        void disasm::visit(_next *ref)
        {
            stream << L"next\t" << ref->get_arg_count();
        }

        void disasm::visit(_dup *ref)
        {
            stream << L"dup";
        }

        void disasm::visit(_if *ref)
        {
            stream << L"if\t^" << ref->get_iid().as_int();
        }

        void disasm::visit(_sector *ref)
        {
            stream << L"sector\t^" << ref->get_begin().as_int() << ", ^" << ref->get_end().as_int();
        }

        void disasm::visit(_break *ref)
        {
            stream << L"break";
        }

        void disasm::visit(_cont *ref)
        {
            stream << L"cont";
        }

        void disasm::visit(_dec *ref)
        {
            stream << L"dec";
        }

        void disasm::visit(_mul *ref)
        {
            stream << L"mul";
        }

        void disasm::visit(_exp *ref)
        {
            stream << L"exp";
        }

        void disasm::visit(_div *ref)
        {
            stream << L"div";
        }

        void disasm::visit(_mod *ref)
        {
            stream << L"mod";
        }

        void disasm::visit(_pos *ref)
        {
            stream << L"pos";
        }

        void disasm::visit(_not *ref)
        {
            stream << L"not";
        }

        void disasm::visit(_inv *ref)
        {
            stream << L"inv";
        }

        void disasm::visit(_bool *ref)
        {
            stream << L"bool";
        }

        void disasm::visit(_shl *ref)
        {
            stream << L"shl";
        }

        void disasm::visit(_shr *ref)
        {
            stream << L"shr";
        }

        void disasm::visit(_shrz *ref)
        {
            stream << L"shrz";
        }

        void disasm::visit(_leq *ref)
        {
            stream << L"leq";
        }

        void disasm::visit(_great *ref)
        {
            stream << L"great";
        }

        void disasm::visit(_greq *ref)
        {
            stream << L"greq";
        }

        void disasm::visit(_swap *ref)
        {
            stream << L"swap";
        }

        void disasm::visit(_and *ref)
        {
            stream << L"and";
        }

        void disasm::visit(_or *ref)
        {
            stream << L"or";
        }

        void disasm::visit(_bitand *ref)
        {
            stream << L"bitand";
        }

        void disasm::visit(_bitor *ref)
        {
            stream << L"bitor";
        }

        void disasm::visit(_xor *ref)
        {
            stream << L"xor";
        }

        void disasm::visit(_frame *ref)
        {
            stream << L"frame\t" << ref->get_begin() << L", " << ref->get_end();
        }

        void disasm::visit(_debug *ref)
        {
            stream << L"debug";
        }

        void disasm::visit(_protect *ref)
        {
            stream << L"protect";
        }
    };
};