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

#pragma once

#include "instruction_visitor.h"
#include "code.h"
#include "lib/pointer.h"
#include <sstream>
#include <string>
#include <vector>

namespace g0at
{
    namespace code
    {
        class disasm : public instruction_visitor
        {
        public:
            disasm(std::wstringstream &_stream, std::vector<std::wstring> &_identifiers);
            static std::wstring to_string(lib::pointer<code> _code);
            void visit(load_string *ref) override;
            void visit(load_var *ref) override;
            void visit(call *ref) override;
            void visit(pop *ref) override;
            void visit(end *ref) override;
            void visit(add *ref) override;
            void visit(load_integer *ref) override;
            void visit(sub *ref) override;
            void visit(neg *ref) override;
            void visit(load_void *ref) override;
            void visit(load_undefined *ref) override;
            void visit(load_null *ref) override;
            void visit(decl_var *ref) override;
            void visit(store *ref) override;
            void visit(load_real *ref) override;
            void visit(load_func *ref) override;
            void visit(ret *ref) override;
            void visit(ret_val *ref) override;
            void visit(create *ref) override;
            void visit(load_prop *ref) override;
            void visit(load_true *ref) override;
            void visit(load_false *ref) override;
            void visit(eq *ref) override;
            void visit(neq *ref) override;
            void visit(if_not *ref) override;
            void visit(jmp *ref) override;
            void visit(vcall *ref) override;
            void visit(this_ptr *ref) override;

        protected:
            std::wstringstream &stream;
            std::vector<std::wstring> &identifiers;
        };
    };
};
