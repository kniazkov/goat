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
#include "bracket.h"

namespace g0at
{
    namespace parser
    {
        class parser;
    };

    namespace ast
    {
        class brackets_pair : public token_with_list
        {
            friend class g0at::parser::parser;
        public:
            brackets_pair(bracket *open_bracket);
            void accept(token_visitor *visitor) override;
            brackets_pair *to_brackets_pair() override;

            wchar_t get_symbol() { return symbol; }
            wchar_t get_inverse_symbol() { return inverse_symbol; }

        protected:
            void set_end_position(lib::pointer<position> pos)
            {
                frag.end = pos;
            }

            wchar_t symbol;
            wchar_t inverse_symbol;
        };
    };
};
