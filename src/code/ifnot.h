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
#include "iid_t.h"

namespace g0at
{
    namespace code
    {
        class _ifnot : public instruction
        {
        public:
            _ifnot(iid_t _iid);
            void accept(instruction_visitor *visitor) override;
            void exec(model::thread *thr) override;

            iid_t get_iid() { return iid; }
            iid_ptr_t get_iid_ptr() { return iid_ptr_t(&iid); }

        protected:
            iid_t iid;
        };
    };
};
