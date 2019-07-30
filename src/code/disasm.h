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
            static std::wstring to_string(lib::pointer<code> _code, bool _print_data_segment);
            void visit(_nop *ref) override;
            void visit(_string *ref) override;
            void visit(_load *ref) override;
            void visit(_call *ref) override;
            void visit(_pop *ref) override;
            void visit(_end *ref) override;
            void visit(_add *ref) override;
            void visit(_integer *ref) override;
            void visit(_sub *ref) override;
            void visit(_neg *ref) override;
            void visit(_void *ref) override;
            void visit(_undef *ref) override;
            void visit(_null *ref) override;
            void visit(_var *ref) override;
            void visit(_store *ref) override;
            void visit(_real *ref) override;
            void visit(_func *ref) override;
            void visit(_ret *ref) override;
            void visit(_retv *ref) override;
            void visit(_object *ref) override;
            void visit(_read *ref) override;
            void visit(_true *ref) override;
            void visit(_false *ref) override;
            void visit(_eq *ref) override;
            void visit(_neq *ref) override;
            void visit(_ifnot *ref) override;
            void visit(_jmp *ref) override;
            void visit(_vcall *ref) override;
            void visit(_this *ref) override;
            void visit(_clone *ref) override;
            void visit(_insof *ref) override;
            void visit(_array *ref) override;
            void visit(_enter *ref) override;
            void visit(_leave *ref) override;
            void visit(_throw *ref) override;
            void visit(_try *ref) override;
            void visit(_catch *ref) override;
            void visit(_finally *ref) override;
            void visit(_inherit *ref) override;
            void visit(_flat *ref) override;
            void visit(_char *ref) override;
            void visit(_less *ref) override;
            void visit(_new *ref) override;
            void visit(_write *ref) override;
            void visit(_inc *ref) override;
            void visit(_thread *ref) override;
            void visit(_rethrow *ref) override;
            void visit(_lock *ref) override;
            void visit(_unlock *ref) override;
            void visit(_get *ref) override;
            void visit(_set *ref) override;
            void visit(_iter *ref) override;
            void visit(_valid *ref) override;
            void visit(_next *ref) override;
            void visit(_dup *ref) override;
            void visit(_if *ref) override;
            void visit(_sector *ref) override;
            void visit(_break *ref) override;
            void visit(_cont *ref) override;
            void visit(_dec *ref) override;
            void visit(_mul *ref) override;
            void visit(_exp *ref) override;
            void visit(_div *ref) override;
            void visit(_mod *ref) override;
            void visit(_pos *ref) override;
            void visit(_not *ref) override;
            void visit(_inv *ref) override;

        protected:
            std::wstringstream &stream;
            std::vector<std::wstring> &identifiers;
        };
    };
};
