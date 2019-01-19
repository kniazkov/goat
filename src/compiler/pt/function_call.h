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

#include "expression.h"
#include <string>
#include <vector>

namespace g0at
{
    namespace pt
    {
        class function_call : public expression
        {
        public:
            function_call(lib::pointer<position> _pos, std::wstring _name);
            void accept(node_visitor *visitor) override;
            function_call *to_function_call() override;

            std::wstring get_name() { return name; }
            void add_arg(lib::pointer<expression> arg) { args.push_back(arg); }
            int get_args_count() { return args.size(); }
            lib::pointer<expression> get_arg(int index) { return args.at(index); }

        protected:
            std::wstring name;
            std::vector<lib::pointer<expression>> args;
        };
    };
};
