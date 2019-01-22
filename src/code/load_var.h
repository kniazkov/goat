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

#include "instruction.h"
#include <string>

namespace g0at
{
    namespace code
    {
        class load_var : public instruction
        {
        public:
            load_var(std::wstring _name);
            void accept(instruction_visitor *visitor) override;
            void exec(model::thread *thr) override;

            std::wstring get_name() { return name; }

        protected:
            std::wstring name;
        };
    };
};
