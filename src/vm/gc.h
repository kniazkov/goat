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

#include "lib/pointer.h"
#include "lib/gc.h"
#include "process.h"

namespace g0at
{
    namespace vm
    {
        enum class gc_type
        {
            serial,
            debug,
            disabled
        };

        lib::gc * create_grabage_collector_serial(process *proc);
        lib::gc * create_grabage_collector_debug(process *proc);
        lib::gc * create_grabage_collector_disabled();

        static inline lib::gc * create_garbage_collector(gc_type type, process *proc)
        {
            switch(type)
            {
                case gc_type::serial:
                    return create_grabage_collector_serial(proc);
                case gc_type::debug:
                    return create_grabage_collector_debug(proc);
                case gc_type::disabled:
                    return create_grabage_collector_disabled();
            }
        }
    };
};
