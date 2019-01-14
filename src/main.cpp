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

#include "compiler/source/source_string.h"
#include "compiler/scanner/scanner.h"
#include "compiler/ast/token_list.h"
#include <iostream>

int main(int argc, char **argv)
{
    g0at::source_string src(L"print(\"hello, world\");");
    g0at::scanner scan(&src);
    g0at::ast::token_list tok_list;
    while(true)
    {
        auto tok = scan.get_token();
        if (!tok)
            break;
        tok_list.add(tok);
    }
    return 0;
}
