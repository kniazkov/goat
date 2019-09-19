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

#include "lib/ref_counter.h"
#include "instruction.h"
#include "iid_t.h"
#include <vector>
#include <string>

namespace g0at
{
    namespace code
    {
        const char signature[] = "goat ;) bin";
        const char sign_rle[]  = "goat ;) rle";

        class code : public lib::ref_counter
        {
        public:
            code();
            ~code();

            void add_instruction(instruction *instr);
            iid_t get_current_iid() { return iid_t(instructions.size()); }
            instruction *get_instruction(iid_t iid) { return instructions.at(iid.as_int()); }
            std::vector<std::wstring> &get_identifiers_list() { return identifiers; }
            void set_identifiers_list(std::vector<std::wstring> &_identifiers) { identifiers = _identifiers; }

        protected:
            code(const code &) { }
            void operator=(const code&) { }

            std::vector<instruction*> instructions;
            std::vector<std::wstring> identifiers;
        };
    };
};
