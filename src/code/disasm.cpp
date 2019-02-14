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
#include "load_string.h"
#include "load_var.h"
#include "call.h"
#include "pop.h"
#include "load_integer.h"
#include "decl_var.h"
#include "store.h"
#include "load_real.h"
#include "load_func.h"
#include "lib/utils.h"

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
#if 0 // we hardly need it
            tmp << L".data\n";
            for (i = 0, size = (int)i_list.size(); i < size; i++)
            {
                tmp << i << L"\t" << i_list[i] << "\n";
            }
#endif
            tmp << L".code\n";
            disasm da(tmp, i_list);
            for (i = 0, size = _code->get_code_size(); i < size; i++)
            {
                if (i > 0)
                {
                    tmp << L"\n";
                    if (i % 8 == 0)
                        tmp << L"  " << i;
                    }
                tmp << L"\t";
                _code->get_instruction(i)->accept(&da);
            }
            return tmp.str();
        }

        void disasm::visit(load_string *ref)
        {
            stream << L"sload \t\"" << ref->get_text() << L'\"';
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

        void disasm::visit(add *ref)
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
    };
};