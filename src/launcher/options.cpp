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

namespace g0at
{
    options::options()
        : prog_name(nullptr)
    {
    }

    void options::parse(int argc, char **argv, options &opt)
    {
        for (int i = 1; i < argc; i++)
        {
            char *arg = argv[i];
            if (arg[0] == '-' && arg[1] == '-')
            {
                // TODO: parameter of the interpreter
            }
            else
            {
                if (opt.prog_name == nullptr)
                    opt.prog_name = arg;
                else
                    opt.args.push_back(arg);
            }
        }
    }
};