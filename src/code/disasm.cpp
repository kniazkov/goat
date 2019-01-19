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

namespace g0at
{
    namespace code
    {
        disasm::disasm(std::wstringstream &_stream)
            : stream(_stream)
        {
        }

        std::wstring disasm::to_string(lib::pointer<code> _code)
        {
            std::wstringstream tmp;
            disasm da(tmp);
            for (int i = 0, size = _code->get_code_size(); i < size; i++)
            {
                tmp << L"\n";
                _code->get_instruction(i)->accept(&da);
            }
            return tmp.str();
        }

        void disasm::visit(load_string *ref)
        {
            stream << L"loads \"" << ref->get_text() << L'\"';
        }

        void disasm::visit(load_var *ref)
        {
            stream << L"load " << ref->get_name();
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
    };
};