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

#include "launcher.h"
#include "../compiler/source/source_file.h"
#include "../compiler/scanner/scanner.h"
#include "../compiler/parser/parser.h"
#include "../compiler/analyzer/analyzer.h"
#include "../compiler/ast/dbg_output.h"
#include "../compiler/pt/dbg_output.h"
#include "../compiler/codegen/generator.h"
#include "../code/disasm.h"
#include "../vm/vm.h"
#include <iostream>
#include <assert.h>

namespace g0at
{
    int launcher::go(int argc, char **argv)
    {
        launcher l_obj(argc, argv);
        int ret_val = l_obj.go();
        assert(g0at::ast::__tok_count == 0);
        return ret_val;
    }

    launcher::launcher(int argc, char **argv)
    {
        options::parse(argc, argv, opt);
    }

    int launcher::go()
    {
        assert(opt.prog_name != nullptr);
        source_file src(opt.prog_name);
        scanner scan(&src);
        auto tok_root = g0at::parser::parser::parse(&scan);
        if (opt.dump_abstract_syntax_tree)
            std::wcout << g0at::ast::dbg_output::to_string(tok_root) << L"\n";
        auto node_root = g0at::analyzer::analyzer::analyze(tok_root);
        tok_root.reset();
        if (opt.dump_parse_tree)
            std::wcout << g0at::pt::dbg_output::to_string(node_root) << L"\n";
        auto code = g0at::codegen::generator::generate(node_root);
        if (opt.dump_assembler_code)
            std::wcout << g0at::code::disasm::to_string(code) << L"\n";    
        vm::vm vm(code);
        vm.run();
        return 0;
    }
};
