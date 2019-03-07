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

#include "gc.h"

namespace g0at
{
    namespace vm
    {
        class gc_disabled : public lib::gc
        {
        public:
            gc_disabled()
            {    
            }

            void collect_garbage() override
            {
                // relax
            }

            void collect_garbage_if_necessary() override
            {
                // relax again
            }

            lib::gc_report get_report() override
            {
                lib::gc_report r;
                r.name = L"disabled";
                r.count_of_launches = 0;
                return r;
            }
        };

        lib::pointer<lib::gc> create_grabage_collector_disabled()
        {
            return new gc_disabled();
        }
    };
};
