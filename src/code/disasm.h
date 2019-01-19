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
#include <sstream>
#include <string>
#include "../lib/pointer.h"

namespace g0at
{
    namespace code
    {
        class disasm : public instruction_visitor
        {
        public:
            disasm(std::wstringstream &_stream);
            static std::wstring to_string(lib::pointer<code> _code);
            void visit(load_string *ref) override;
            void visit(load_var *ref) override;
            void visit(call *ref) override;
            void visit(pop *ref) override;
            void visit(end *ref) override;

        protected:
            std::wstringstream &stream;
        };
    };
};
