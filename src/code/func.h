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
#include <vector>

namespace g0at
{
    namespace code
    {
        class _func : public instruction
        {
        public:
            _func(int _first_iid);
            void accept(instruction_visitor *visitor) override;
            void exec(model::thread *thr) override;

            int get_first_iid() { return first_iid; }
            int *get_first_iid_ptr() { return &first_iid; }
            void add_arg_id(int id) { arg_ids.push_back(id); }
            int get_arg_ids_count() { return (int)arg_ids.size(); }
            int get_arg_id(int n) { return arg_ids.at(n); }

        protected:
            int first_iid;
            std::vector<int> arg_ids;
        };
    };
};
