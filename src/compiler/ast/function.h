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
#include "token_with_list.h"
#include "keyword.h"
#include "brackets_pair.h"

namespace g0at
{
    namespace ast
    {
        enum class function_type
        {
            unknown,
            function,
            thread
        };

        class function : public token_with_list
        {
        public:
            function();
            function(keyword *_kw, brackets_pair *_args, brackets_pair *_body, function_type _type);
            void accept(token_visitor *visitor) override;
            function *to_function() override;

            token_list *get_raw_args_list() { return &args_raw; }
            token_list *get_args_list() { return &args; }
            token_list *get_body() { return &body; }
            function_type get_type() { return type; }

        protected:
            token_list args_raw;
            token_list args;
            token_list body;
            function_type type;
        };
    };
};
