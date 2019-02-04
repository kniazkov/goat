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
            disasm da(tmp, _code->get_identifiers_list());
            for (int i = 0, size = _code->get_code_size(); i < size; i++)
            {
                tmp << L"\n";
                _code->get_instruction(i)->accept(&da);
            }
            return tmp.str();
        }

        void disasm::visit(load_string *ref)
        {
            stream << L"sload \"" << ref->get_text() << L'\"';
        }

        void disasm::visit(load_var *ref)
        {
            int id = ref->get_id();
            stream << L"load " << identifiers.at(id) << " (" << id << ")";
        }

        void disasm::visit(call *ref)
        {
            stream << L"call";
        }

        void disasm::visit(pop *ref)
        {
            stream << L"pop " << ref->get_count();
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
            stream << L"iload " << ref->get_value();
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
    };
};