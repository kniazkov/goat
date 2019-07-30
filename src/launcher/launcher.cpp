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
#include "compiler/source/source_string.h"
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
#include "lib/assert.h"
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
            int ret_val = 0;
            lib::reset_memory_allocator();
            // we use 'do..while' wrapper to release memory before checking for memory leaks
            do
            {
                launcher l_obj(argc, argv);
                ret_val = l_obj.go();
            } while(false);
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

    static void dump_memory_usage_report(const char *file_name, vm::environment *env)
    {
        auto gcr = env->get_gc()->get_report();
        auto opr = env->get_pool()->get_report();
        lib::dump_file(file_name, "memory.txt", global::resource->memory_usage_report(
            lib::get_heap_size(),
            lib::get_max_used_memory_size(),
            gcr.name ? gcr.name : L"none",
            gcr.count_of_launches,
            opr.new_count + opr.reinit_count,
            opr.new_count,
            opr.reinit_count,
            100.0 * opr.reinit_count / ( opr.new_count + opr.reinit_count )
        ));
    }

    int launcher::go()
    {
        if (opt.bin && opt.prog_name == nullptr)
            throw no_input_file();

        lib::pointer<vm::environment> env;
        int ret_val = 0;

        vm::gc_type gct = vm::gc_type::serial;
        if (opt.gc_type_str)
        {
            if (0 == strcmp(opt.gc_type_str, "debug"))
                gct = vm::gc_type::debug;
            else if (0 == strcmp(opt.gc_type_str, "disabled"))
                gct = vm::gc_type::disabled;
        }

        if (opt.prog_name == nullptr)
        {
            model::name_cache name_cache;
            while(true)
            {
                std::cout << "> ";
                std::string line;
                std::getline(std::cin, line);
                if (line == "quit" || line == "q")
                {
                    return ret_val;
                }
                source_string src(global::char_encoder->decode(line));
                try
                {
                    scanner scan(&src);
                    auto tok_root = parser::parser::parse(&scan, false, "shell", opt.lib_path);
                    auto node_root = analyzer::analyzer::analyze(tok_root);
                    tok_root.reset();
                    auto code = codegen::generator::generate(&name_cache, node_root);
                    node_root.reset();
                    if (opt.dump_assembler_code)
                    {
                        std::cout << global::char_encoder->encode(code::disasm::to_string(code, false));
                    }
                    if (!env)
                    {
                        env = new vm::environment(gct, code->get_identifiers_list());
                    }
                    else
                    {
                        env->get_pool()->merge_strings_list(code->get_identifiers_list());
                    }
                    vm::vm vm(code);
                    ret_val = vm.run(env.get());
                    std::cout << std::endl;
                    name_cache.reinit(env->get_pool()->get_strings_list());
                }
                catch (std::exception &ex)
                {
                    std::cerr << ex.what() << std::endl;
                }
            }
        }
        else if (opt.bin)
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
                lib::dump_file(opt.prog_name, "asm", code::disasm::to_string(code, true));
            }
            vm::vm vm(code);
            env = new vm::environment(gct, code->get_identifiers_list());
            ret_val = vm.run(env.get());
            if (opt.dump_memory_usage_report)
            {
                dump_memory_usage_report(opt.prog_name, env.get());
            }
            return ret_val;
        }
        else
        {
            source_file src(opt.prog_name);
            scanner scan(&src);
            auto tok_root = parser::parser::parse(&scan, opt.dump_abstract_syntax_tree, opt.prog_name, opt.lib_path);
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
            model::name_cache name_cache;
            auto code = codegen::generator::generate(&name_cache, node_root);
            node_root.reset();
            std::vector<uint8_t> binary;
            code::serializer::serialize(code, binary, !opt.do_not_compress);
            code.reset();
            auto code_2 = code::deserializer::deserialize(binary);
            if (opt.dump_assembler_code)
            {
                lib::dump_file(opt.prog_name, "asm", code::disasm::to_string(code_2, true));
            }
            if (!opt.compile)
            {
                vm::vm vm(code_2);
                env = new vm::environment(gct, code_2->get_identifiers_list());
                ret_val = vm.run(env.get());
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
                dump_memory_usage_report(opt.prog_name, env.get());
            }
            return ret_val;
        }
    }
};
