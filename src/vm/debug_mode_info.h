/*

Copyright (C) 2017-2020 Ivan Kniazkov

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

#include "compiler/source/breakpoint.h"
#include "lib/pointer.h"
#include <vector>

namespace g0at
{
    namespace vm
    {
        class debug_mode_info
        {
        public:
            debug_mode_info() :
                frame_begin(-1),
                frame_end(-1)
            {
            }

            int frame_begin;
            int frame_end;
            std::vector<lib::pointer<breakpoint>> breakpoints;

        private:
            debug_mode_info(const debug_mode_info&) { }
            void operator=(const debug_mode_info&) { }
        };
    };
};
