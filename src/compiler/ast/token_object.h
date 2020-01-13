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

#include "token_with_list.h"
#include "expression.h"
#include "brackets_pair.h"
#include <utility>
#include <vector>

namespace g0at
{
    namespace ast
    {
        class token_object : public expression, public token_with_list
        {
        public:
            typedef std::pair<lib::pointer<expression>, lib::pointer<expression>> token_object_item;

            token_object(brackets_pair *_body);
            void accept(token_visitor *visitor) override;
            token_object *to_token_object() override;

            void add_item(lib::pointer<expression> key, lib::pointer<expression> value)
            {
                 items.push_back(std::make_pair(key, value));
            }
            int get_items_count() { return (int)items.size(); }
            token_object_item get_item(int index) { return items.at(index); }

        protected:
            std::vector<token_object_item> items;
        };
    };
};
