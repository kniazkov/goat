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

#include "pattern.h"
#include "compiler/ast/token.h"
#include "lib/assert.h"

namespace g0at
{
    namespace parser
    {
        pattern::pattern(ast::token_2nd_list *_list, parser_data *_data)
            : list(_list), data(_data)
        {
        }

        pattern::~pattern()
        {
        }

        bool pattern::pass()
        {
            assert(list != nullptr);
            assert(data != nullptr);

            ast::token *tok = list->first;
            bool flag = false;
            while(tok)
            {
                ast::token *next = tok->next_2;
                flag = check(tok);
                if (flag)
                    break;
                tok = next;
            }
            return flag;
        }
    };
};