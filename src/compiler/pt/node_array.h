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
#include <vector>

namespace g0at
{
    namespace pt
    {
        class node_array : public expression
        {
        public:
            node_array(fragment _frag);
            void accept(node_visitor *visitor) override;
            node_array *to_node_array() override;

            void add_object(lib::pointer<expression> obj) { objects.push_back(obj); }
            int get_objects_count() { return (int)objects.size(); }
            lib::pointer<expression> get_object(int index) { return objects.at(index); }

        protected:
            std::vector<lib::pointer<expression>> objects;
        };
    };
};
