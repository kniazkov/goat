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

#include "disasm.h"
#include "lib/utils.h"
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
#include "try.h"
#include "catch.h"
#include "finally.h"

namespace g0at
{
    namespace code
    {
        disasm::disasm(std::wstringstream &_stream, std::vector<std::wstring> &_identifiers)
            : stream(_stream), identifiers(_identifiers)
        {
        }

        std::wstring disasm::to_string(lib::pointer<code> _code)
        {
            std::wstringstream tmp;
            int i, size;
            auto i_list = _code->get_identifiers_list();
#if 1 // we hardly need it
            tmp << L".data\n";
            for (i = 0, size = (int)i_list.size(); i < size; i++)
            {
                tmp << L"  " << i << L"\t" << i_list[i] << "\n";
            }
            tmp << "\n";
#endif
            tmp << L".code\n";
            disasm da(tmp, i_list);
            for (i = 0, size = _code->get_code_size(); i < size; i++)
            {
                if (i > 0)
                {
                    tmp << L"\n";
                    if (i % 5 == 0)
                        tmp << L"  " << i;
                    }
                tmp << L"\t";
                _code->get_instruction(i)->accept(&da);
            }
            return tmp.str();
        }

        void disasm::visit(load_string *ref)
        {
            int id = ref->get_id();
            stream << L"sload \t" << id << L"\t; \"" <<  lib::escape_special_chars(identifiers.at(id)) << '"';
        }

        void disasm::visit(load_var *ref)
        {
            int id = ref->get_id();
            stream << L"load \t" << id << L"\t; " << identifiers.at(id);
        }

        void disasm::visit(call *ref)
        {
            stream << L"call \t" << ref->get_arg_count();
        }

        void disasm::visit(pop *ref)
        {
            stream << L"pop";
        }

        void disasm::visit(end *ref)
        {
            stream << L"end";
        }

        void disasm::visit(_add *ref)
        {
            stream << L"add";
        }

        void disasm::visit(load_integer *ref)
        {
            stream << L"iload \t" << ref->get_value();
        }

        void disasm::visit(sub *ref)
        {
            stream << L"sub";
        }

        void disasm::visit(neg *ref)
        {
            stream << L"neg";
        }

        void disasm::visit(load_void *ref)
        {
            stream << L"void";
        }

        void disasm::visit(load_undefined *ref)
        {
            stream << L"undef";
        }

        void disasm::visit(load_null *ref)
        {
            stream << L"null";
        }

        void disasm::visit(decl_var *ref)
        {
            int id = ref->get_id();
            stream << L"var \t" << id << L"\t; " << identifiers.at(id);
        }

        void disasm::visit(store *ref)
        {
            int id = ref->get_id();
            stream << L"store \t" << id << L"\t; " << identifiers.at(id);
        }

        void disasm::visit(load_real *ref)
        {
            stream << L"rload \t" << lib::double_to_wstring(ref->get_value());
        }

        void disasm::visit(load_func *ref)
        {
            int i, size = ref->get_arg_ids_count();
            stream << L"func \t" << ref->get_first_iid();
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

        void disasm::visit(ret *ref)
        {
            stream << L"ret";
        }

        void disasm::visit(ret_val *ref)
        {
            stream << L"retv";
        }

        void disasm::visit(create *ref)
        {
            stream << L"create \t" << ref->get_count();
        }

        void disasm::visit(load_prop *ref)
        {
            int id = ref->get_id();
            stream << L"prop \t" << id << L"\t; " << identifiers.at(id);
        }

        void disasm::visit(load_true *ref)
        {
            stream << L"true";
        }

        void disasm::visit(load_false *ref)
        {
            stream << L"false";
        }

        void disasm::visit(eq *ref)
        {
            stream << L"eq";
        }

        void disasm::visit(neq *ref)
        {
            stream << L"neq";
        }

        void disasm::visit(if_not *ref)
        {
            stream << L"ifnot \t" << ref->get_iid();
        }

        void disasm::visit(jmp *ref)
        {
            stream << L"jmp \t" << ref->get_iid();
        }

        void disasm::visit(vcall *ref)
        {
            int id = ref->get_id();
            stream << L"vcall \t" << id << L", " << ref->get_arg_count() << L"\t; " << identifiers.at(id);
        }

        void disasm::visit(this_ptr *ref)
        {
            stream << L"this";
        }

        void disasm::visit(clone *ref)
        {
            stream << L"clone \t" << ref->get_arg_count();
        }

        void disasm::visit(instance_of *ref)
        {
            stream << L"insof \t" << ref->get_arg_count();
        }

        void disasm::visit(array *ref)
        {
            stream << L"array \t" << ref->get_count();
        }

        void disasm::visit(enter *ref)
        {
            stream << L"enter";
        }

        void disasm::visit(leave *ref)
        {
            stream << L"leave";
        }

        void disasm::visit(raise *ref)
        {
            stream << L"raise";
        }

        void disasm::visit(_try *ref)
        {
            stream << L"try\t" << ref->get_iid();
        }

        void disasm::visit(_catch *ref)
        {
            int id = ref->get_id();
            stream << L"catch\t" << id << L"\t; " << identifiers.at(id);
        }

        void disasm::visit(_finally *ref)
        {
            stream << L"finally\t" << ref->get_iid();
        }
    };
};