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

#include "new.h"
#include <cstdlib>

namespace g0at
{
    namespace lib
    {
        static unsigned long int __used_memory_size = 0;
        static unsigned long int __max_used_memory_size = 0;
        
        unsigned long int get_used_memory_size()
        {
            return __used_memory_size;
        }

        unsigned long int get_max_used_memory_size()
        {
            return __max_used_memory_size;
        }

        const char* out_of_memory::what() const throw()
        {
            return "out of memory";
        }

        struct memory_descriptor
        {
            size_t size;
        };

        static void * alloc(size_t size)
        {
            memory_descriptor *d = (memory_descriptor*)std::malloc(sizeof(memory_descriptor) + size);
            if (!d)
                throw out_of_memory();
            d->size = size;
            __used_memory_size += sizeof(memory_descriptor) + size;
            if (__max_used_memory_size < __used_memory_size)
                __max_used_memory_size = __used_memory_size;
            return (void*)(d + 1);
        }

        static void free(void *p)
        {
            if (!p)
                return;
            memory_descriptor *d = (memory_descriptor*)p - 1;
            __used_memory_size -= sizeof(memory_descriptor) + d->size;
            std::free(d);
        }
    };
};

void *operator new(size_t size)
{
    return g0at::lib::alloc(size);
}

void operator delete(void *p)
{
    g0at::lib::free(p);
}

void *operator new[](size_t size)
{
    return g0at::lib::alloc(size);
}

void operator delete[](void *p)
{
    g0at::lib::free(p);
}
