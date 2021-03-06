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

#include "expression.h"
#include <vector>

namespace g0at
{
    namespace pt
    {
        class index_access : public expression
        {
        public:
            index_access(fragment _frag, lib::pointer<expression> _object);
            void accept(node_visitor *visitor) override;
            index_access *to_index_access() override;

            lib::pointer<expression> get_object() { return object; }
            void add_arg(lib::pointer<expression> arg) { args.push_back(arg); }
            int get_args_count() { return (int)args.size(); }
            lib::pointer<expression> get_arg(int index) { return args.at(index); }

        protected:
            lib::pointer<expression> object;
            std::vector<lib::pointer<expression>> args;
        };
    };
};
