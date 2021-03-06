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

#include "settings.h"
#include "lib/exception.h"

namespace g0at
{
    namespace vm
    {
        class vm_exception : public lib::exception
        {
        public:
            vm_exception(std::wstring message)
                : exception(global::resource->vm_fault() + L": " + message)
            {
            }
        };

        class stack_is_empty : public vm_exception
        {
        public:
            stack_is_empty()
                : vm_exception(global::resource->stack_is_empty())
            {
            }
        };

#ifdef STACK_DEBUG
        class uninitialized_value_on_the_stack : public vm_exception
        {
        public:
            uninitialized_value_on_the_stack()
                : vm_exception(global::resource->uninitialized_value_on_the_stack())
            {
            }
        };
#endif

#ifdef GC_DEBUG
        class use_of_a_dead_object : public vm_exception
        {
        public:
            use_of_a_dead_object(std::wstring data)
                : vm_exception(global::resource->use_of_a_dead_object(data))
            {
            }
        };
#endif
    };
};
