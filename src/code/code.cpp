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

#include "code.h"

namespace g0at
{
    namespace code
    {
        code::code()
            : refs(0)
        {
        }

        code::~code()
        {
            for (instruction *i : i_list)
            {
                delete i;
            }
        }

        void code::add_instruction(instruction *instr)
        {
            uint32_t id = i_list.size();
            instr->id = id;
            i_list.push_back(instr);
        }
    }
};