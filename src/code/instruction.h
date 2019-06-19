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

#include "instruction_visitor.h"
#include "../model/thread.h"
#include <cstdint>

namespace g0at
{
    namespace code
    {
        /**
         * @brief Atomic instruction
         * 
         * An instruction executed by the virtual machine in one clock cycle. 
         */
        class instruction
        {
        public:
            instruction();
            virtual ~instruction();
            /**
             * @brief An acceptor for visitors
             * @param visitor A visitor
             */
            virtual void accept(instruction_visitor *visitor) = 0;
            /**
             * @brief Executes an instruction
             * @param thr A thread
             */
            virtual void exec(model::thread *thr) = 0;
        };
    };
};
