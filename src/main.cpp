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
#include "compiler/parser/parser.h"
#include "compiler/ast/dbg_output.h"
#include <iostream>
#include <assert.h>

void test()
{
    g0at::source_string src(L"print(\"hello, world\");");
    g0at::scanner scan(&src);
    g0at::parser::parser par;
    par.create_root(&scan);
    auto root = par.get_root();
    std::wcout << g0at::ast::dbg_output::to_string(root) << L"\n";
}

int main(int argc, char **argv)
{
    test();
    assert(g0at::ast::__tok_count == 0);
    return 0;
}
