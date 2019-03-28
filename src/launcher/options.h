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

#include <vector>

namespace g0at
{
    class options
    {
    public:
        options();
        static void parse(int argc, char **argv, options &opt);

        char *prog_name;
        std::vector<char*> args;
        bool dump_abstract_syntax_tree;
        bool dump_parse_tree;
        bool dump_assembler_code;
        bool compile;
        bool bin;
        bool dump_memory_usage_report;
        char *gc_type_str;
        bool do_not_compress;
    };
};
