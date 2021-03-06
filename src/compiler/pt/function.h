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
#include "statement.h"
#include <vector>

namespace g0at
{
    namespace pt
    {
        enum class function_type
        {
            unknown,
            function,
            thread
        };

        class function : public node
        {
        public:
            function(fragment _frag, function_type _type);
            void accept(node_visitor *visitor) override;
            function *to_function() override;

            void add_stmt(lib::pointer<statement> stmt) { code.push_back(stmt); }
            int get_code_size() { return (int)code.size(); }
            lib::pointer<statement> get_stmt(int index) { return code.at(index); }
            void add_arg(std::wstring arg) { args.push_back(arg); }
            int get_args_count() { return (int)args.size(); }
            std::wstring get_arg(int index) { return args.at(index); }
            bool is_root_function() { return get_fragment().begin == nullptr; }
            function_type get_type() { return type; }

        protected:
            std::vector<lib::pointer<statement>> code;
            std::vector<std::wstring> args;
            function_type type;
        };
    };
};
