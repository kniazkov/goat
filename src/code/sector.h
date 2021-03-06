/*

Copyright (C) 2017-2020 Ivan Kniazkov

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
        class _sector : public instruction
        {
        public:
            _sector(iid_t _begin, iid_t _end);
            void accept(instruction_visitor *visitor) override;
            void exec(model::thread *thr) override;

            iid_t get_begin() { return begin; }
            iid_ptr_t get_begin_ptr() { return iid_ptr_t(&begin); }
            iid_t get_end() { return end; }
            iid_ptr_t get_end_ptr() { return iid_ptr_t(&end); }

        protected:
            iid_t begin;
            iid_t end;
        };
    };
};
