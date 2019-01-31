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
#include "lib/ref_counter.h"
#include "lib/exception.cpp"
#include "global/global.h"
#include "compiler/source/source_file.h"
#include "compiler/scanner/scanner.h"
#include "compiler/parser/parser.h"
#include "compiler/analyzer/analyzer.h"
#include "compiler/ast/dbg_output.h"
#include "compiler/pt/dbg_output.h"
#include "compiler/codegen/generator.h"
#include "code/disasm.h"
#include "vm/vm.h"
#include <iostream>
#include <exception>
#include <assert.h>

namespace g0at
{
    class no_input_file : public lib::exception
    {
    public:
        no_input_file()
            : exception(global::resource->no_input_file())
        {
        }
    };

    int launcher::go(int argc, char **argv)
    {
        launcher l_obj(argc, argv);
        int ret_val = l_obj.go();
        assert(g0at::lib::__obj_count == 0);
        return ret_val;
    }

    launcher::launcher(int argc, char **argv)
    {
        options::parse(argc, argv, opt);
    }

    int launcher::go()
    {
        try
        {
            if (opt.prog_name == nullptr)
            {
                throw no_input_file();
            }
            source_file src(opt.prog_name);
            scanner scan(&src);
            auto tok_root = g0at::parser::parser::parse(&scan);
            if (opt.dump_abstract_syntax_tree)
            {
                std::cout << global::char_encoder->encode(g0at::ast::dbg_output::to_string(tok_root)) << std::endl;
            }
            auto node_root = g0at::analyzer::analyzer::analyze(tok_root);
            tok_root.reset();
            if (opt.dump_parse_tree)
            {
                std::cout << global::char_encoder->encode(g0at::pt::dbg_output::to_string(node_root)) << std::endl;
            }
            auto code = g0at::codegen::generator::generate(node_root);
            if (opt.dump_assembler_code)
            {
                std::cout << global::char_encoder->encode(g0at::code::disasm::to_string(code)) << std::endl;
            }
            vm::vm vm(code);
            vm.run();
        }
        catch (std::exception &ex)
        {
            std::cerr << ex.what() << std::endl;
        }
        return 0;
    }
};
