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
#include <cstdint>

namespace g0at
{
    namespace lib
    {
        static int __allocated_blocks_count = 0;
        static size_t __used_memory_size = 0;
        static size_t __max_used_memory_size = 0;
        static size_t __heap_size = UINTPTR_MAX;
        
        int get_allocated_blocks_count()
        {
            return __allocated_blocks_count;
        }

        size_t get_used_memory_size()
        {
            return __used_memory_size;
        }

        size_t get_max_used_memory_size()
        {
            return __max_used_memory_size;
        }

        void set_heap_size(size_t size)
        {
            __heap_size = size;
        }

        size_t get_heap_size()
        {
            return __heap_size;
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
            if (sizeof(memory_descriptor) + size + __used_memory_size > __heap_size)
                throw out_of_memory();
            memory_descriptor *d = (memory_descriptor*)std::malloc(sizeof(memory_descriptor) + size);
            if (!d)
                throw out_of_memory();
            d->size = size;
            __used_memory_size += sizeof(memory_descriptor) + size;
            __allocated_blocks_count++;
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
            __allocated_blocks_count--;
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
