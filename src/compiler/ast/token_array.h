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

#include "token_with_list.h"
#include "expression.h"
#include "brackets_pair.h"
#include <string>

namespace g0at
{
    namespace ast
    {
        class token_array : public expression, public token_with_list
        {
        public:
            token_array(brackets_pair *_objects);
            void accept(token_visitor *visitor) override;
            token_array *to_token_array() override;

            token_list *get_object_list() { return &objects; }

        protected:
            token_list objects;
        };
    };
};
