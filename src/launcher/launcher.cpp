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
#include "lib/new.h"
#include "lib/utils.h"
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
#include "vm/environment.h"
#include "vm/gc.h"
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

    class memory_leak : public lib::exception
    {
    public:
        memory_leak(int blocks_count, size_t size)
            : exception(global::resource->memory_leak(blocks_count, size))
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
            assert(lib::get_ref_counter_instances_count() == 0);
            if (lib::get_used_memory_size() != 0)
                throw memory_leak(lib::get_allocated_blocks_count(), lib::get_used_memory_size());
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

    static void dump_memory_usage_report(const char *file_name, vm::vm_report &vmr)
    {
        lib::dump_file(file_name, "memory.txt", global::resource->memory_usage_report(
            lib::get_heap_size(),
            lib::get_max_used_memory_size(),
            vmr.gcr.name ? vmr.gcr.name : L"none",
            vmr.gcr.count_of_launches,
            vmr.opr.new_count + vmr.opr.reinit_count,
            vmr.opr.new_count,
            vmr.opr.reinit_count,
            100.0 * vmr.opr.reinit_count / ( vmr.opr.new_count + vmr.opr.reinit_count )
        ));
    }

    int launcher::go()
    {
        if (opt.prog_name == nullptr)
        {
            throw no_input_file();
        }
        
        vm::environment env;
        env.gct = vm::gc_type::serial;
        if (opt.gc_type_str)
        {
            if (0 == strcmp(opt.gc_type_str, "debug"))
                env.gct = vm::gc_type::debug;
            else if (0 == strcmp(opt.gc_type_str, "disabled"))
                env.gct = vm::gc_type::disabled;
        }

        vm::vm_report vmr = {0};
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
                lib::dump_file(opt.prog_name, "asm", code::disasm::to_string(code));
            }
            vm::vm vm(code);
            vmr = vm.run(&env);
            if (opt.dump_memory_usage_report)
            {
                dump_memory_usage_report(opt.prog_name, vmr);
            }
        }
        else
        {
            source_file src(opt.prog_name);
            scanner scan(&src);
            auto tok_root = parser::parser::parse(&scan, opt.dump_abstract_syntax_tree, opt.prog_name);
            if (opt.dump_abstract_syntax_tree)
            {
                lib::dump_file(opt.prog_name, "tokens.txt", ast::dbg_output::to_string(tok_root.get()));
            }
            auto node_root = analyzer::analyzer::analyze(tok_root);
            tok_root.reset();
            if (opt.dump_parse_tree)
            {
                lib::dump_file(opt.prog_name, "ptree.txt", pt::dbg_output::to_string(node_root.get()));
            }
            auto code = codegen::generator::generate(node_root);
            node_root.reset();
            std::vector<uint8_t> binary;
            code::serializer::serialize(code, binary);
            code.reset();
            auto code_2 = code::deserializer::deserialize(binary);
            if (opt.dump_assembler_code)
            {
                lib::dump_file(opt.prog_name, "asm", code::disasm::to_string(code_2));
            }
            if (!opt.compile)
            {
                vm::vm vm(code_2);
                vmr = vm.run(&env);
            }
            else
            {
                char *full_file_name = lib::file_name_postfix(opt.prog_name, "bin");
                std::ofstream bin_file(full_file_name);
                bin_file.write((const char*)(&binary[0]), binary.size());
                bin_file.close();
                delete full_file_name;
            }
            if (opt.dump_memory_usage_report)
            {
                dump_memory_usage_report(opt.prog_name, vmr);
            }
        }
        return vmr.ret_value;
    }
};
