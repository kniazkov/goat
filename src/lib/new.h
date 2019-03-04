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

#include <cstddef>
#include <exception>

namespace g0at
{
    namespace lib
    {
        unsigned long int get_used_memory_size();
        unsigned long int get_max_used_memory_size();

        class out_of_memory : public std::exception
        {
        public:
            const char* what() const throw() override;
        };
    };
};

void *operator new(size_t size);
void operator delete(void *p);
void *operator new[](size_t size);
void operator delete[](void *p);
