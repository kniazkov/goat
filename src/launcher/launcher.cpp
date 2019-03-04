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
#include "lib/exception.h"
#include "global/global.h"
#include "compiler/source/source_file.h"
#include "compiler/scanner/scanner.h"
#include "compiler/parser/parser.h"
#include "compiler/analyzer/analyzer.h"
#include "compiler/ast/dbg_output.h"
#include "compiler/pt/dbg_output.h"
#include "compiler/codegen/generator.h"
#include "code/disasm.h"
#include "code/serializer.h"
#include "code/deserializer.h"
#include "vm/vm.h"
#include <iostream>
#include <exception>
#include <assert.h>
#include <fstream>
#include <streambuf>
#include <cstring>
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

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
#ifdef _WIN32
        _setmode(_fileno(stdout), O_BINARY);
        _setmode(_fileno(stdin), O_BINARY);
        _setmode(_fileno(stderr), O_BINARY);
#endif
        try
        {
            launcher l_obj(argc, argv);
            int ret_val = l_obj.go();
            assert(g0at::lib::get_ref_counter_instances_count() == 0);
            return ret_val;
        }
        catch (std::exception &ex)
        {
            std::cerr << ex.what() << std::endl;
        }
    }

    launcher::launcher(int argc, char **argv)
    {
        options::parse(argc, argv, opt);
    }

    int launcher::go()
    {
        if (opt.prog_name == nullptr)
        {
            throw no_input_file();
        }
        if (opt.bin)
        {
            std::vector<uint8_t> binary;
            std::ifstream bin_file(opt.prog_name);
            if (bin_file.good() == false)
            {
                throw lib::file_not_found(opt.prog_name);
            }
            bin_file.seekg(0, std::ios::end);
            binary.reserve((size_t)bin_file.tellg());
            bin_file.seekg(0, std::ios::beg);
            binary.assign((std::istreambuf_iterator<char>(bin_file)),
                        std::istreambuf_iterator<char>());
            auto code = code::deserializer::deserialize(binary);
            if (opt.dump_assembler_code)
            {
                std::cout << global::char_encoder->encode(code::disasm::to_string(code)) << std::endl;
            }
            vm::vm vm(code);
            vm.run();
        }
        else
        {
            source_file src(opt.prog_name);
            scanner scan(&src);
            auto tok_root = parser::parser::parse(&scan, opt.dump_abstract_syntax_tree);
            if (opt.dump_abstract_syntax_tree)
            {
                std::cout << global::char_encoder->encode(ast::dbg_output::to_string(tok_root.get())) << std::endl;
            }
            auto node_root = analyzer::analyzer::analyze(tok_root);
            tok_root.reset();
            if (opt.dump_parse_tree)
            {
                std::cout << global::char_encoder->encode(pt::dbg_output::to_string(node_root)) << std::endl;
            }
            auto code = codegen::generator::generate(node_root);
            std::vector<uint8_t> binary;
            code::serializer::serialize(code, binary);
            auto code_2 = code::deserializer::deserialize(binary);
            if (opt.dump_assembler_code)
            {
                std::cout << global::char_encoder->encode(code::disasm::to_string(code_2)) << std::endl;
            }
            if (!opt.compile)
            {
                vm::vm vm(code_2);
                vm.run();
            }
            else
            {
                auto name_len = std::strlen(opt.prog_name);
                char *tmp = new char[name_len + 4 + 1];
                strcpy(tmp, opt.prog_name);
                strcat(tmp, ".bin");
                std::ofstream bin_file(tmp);
                bin_file.write((const char*)(&binary[0]), binary.size());
                bin_file.close();
                delete tmp;
            }
        }
        return 0;
    }
};
