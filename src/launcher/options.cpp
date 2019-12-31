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

#include "options.h"
#include "global/global.h"
#include "lib/exception.h"
#include "lib/new.h"
#include <cstring>
#include <cstdlib>
#include <sstream>

namespace g0at
{
    class incorrect_command_line_parameter : public lib::exception
    {
    public:
        incorrect_command_line_parameter(const char *parameter)
            : exception(global::resource->incorrect_command_line_parameter(parameter))
        {
        }
    };

    options::options()
        : prog_name(nullptr),
            dump_abstract_syntax_tree(false),
            dump_parse_tree(false),
            dump_assembler_code(false),
            compile(false),
            bin(false),
            dump_memory_usage_report(false),
            gc_type_str(nullptr),
            do_not_compress(false),
            debug(false),
            run(false),
            from_stdin(false)
    {
        lib_path.push_back(".");
    }

    void options::parse(int argc, char **argv, options &opt)
    {
        const char *env_language = std::getenv("GOAT_LANGUAGE");
        if (env_language)
        {
            global::resource = resource::resource::get_instance(env_language);
        }

        const char *env_debug = std::getenv("GOAT_DEBUG");
        if (env_debug && 0 == std::strcmp(env_debug, "1"))
        {
            opt.debug = true;
        }

        for (int i = 1; i < argc; i++)
        {
            char *arg = argv[i];
            if (arg[0] == '-' && arg[1] == '-')
            {
                if (0 == std::strcmp(arg + 2, "dump-abstract-syntax-tree"))
                {
                    opt.dump_abstract_syntax_tree = true;
                }
                else if (0 == std::strcmp(arg + 2, "dump-parse-tree"))
                {
                    opt.dump_parse_tree = true;
                }
                else if (0 == std::strcmp(arg + 2, "dump-assembler-code"))
                {
                    opt.dump_assembler_code = true;
                }
                else if (0 == std::strncmp(arg + 2, "language=", 9))
                {
                    global::resource = resource::resource::get_instance(arg + 11);
                }
                else if (0 == std::strncmp(arg + 2, "lang=", 5))
                {
                    global::resource = resource::resource::get_instance(arg + 7);
                }
                else if (0 == std::strcmp(arg + 2, "compile"))
                {
                    opt.compile = true;
                }
                else if (0 == std::strcmp(arg + 2, "bin"))
                {
                    opt.bin = true;
                }
                else if (0 == std::strcmp(arg + 2, "debug"))
                {
                    opt.debug = true;
                }
                else if (0 == std::strcmp(arg + 2, "run"))
                {
                    opt.run = true;
                }
                else if (0 == std::strcmp(arg + 2, "stdin"))
                {
                    opt.from_stdin = true;
                    if (opt.prog_name != nullptr) {
                        opt.args.insert(opt.args.begin(), opt.prog_name);
                        opt.prog_name = nullptr;
                    }
                }
                else if (0 == std::strcmp(arg + 2, "dump-memory-usage-report"))
                {
                    opt.dump_memory_usage_report = true;
                }
                else if (0 == std::strncmp(arg + 2, "heap-size=", 10))
                {
                    int heap_size = std::atoi(arg + 12);
                    if (heap_size <= 0)
                        throw incorrect_command_line_parameter(arg);
                    lib::set_heap_size((unsigned long int)heap_size * 1024 * 1024);
                }
                else if (0 == std::strncmp(arg + 2, "gc=", 3))
                {
                    opt.gc_type_str = arg + 5;
                }
                else if (0 == std::strcmp(arg + 2, "do-not-compress"))
                {
                    opt.do_not_compress = true;
                }
                else if (0 == std::strncmp(arg + 2, "lib=", 4))
                {
                    opt.parse_lib_path(arg + 6);
                }
                else
                {
                    throw incorrect_command_line_parameter(arg);
                }
            }
            else
            {
                if (opt.prog_name == nullptr && !opt.from_stdin)
                    opt.prog_name = arg;
                else
                    opt.args.push_back(arg);
            }
        }

        const char *env_lib_path = std::getenv("GOAT_LIBRARY_PATH");
        if (env_lib_path)
        {
            opt.parse_lib_path(env_lib_path);
        }
    }

    void options::parse_lib_path(const char *pathes)
    {
        const char *p = pathes - 1;
        std::stringstream ss;
        do
        {
            ++p;
            if (*p == ';' || !(*p))
            {
                std::string path = ss.str();
                if (path.length() > 0 && lib_path_set.find(path) == lib_path_set.end())
                {
                    lib_path.push_back(path);
                    lib_path_set.insert(path);
                }
                ss.str(std::string());
            }
            else
            {
                ss << *p;
            }
            
        } while(*p);
    }
};
