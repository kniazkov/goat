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
        static size_t __cached_memory_size = 0;
        static size_t __max_used_memory_size = 0;
        static size_t __heap_size = UINTPTR_MAX;
        static gc *__gc = nullptr;
        
        int get_allocated_blocks_count()
        {
            return __allocated_blocks_count;
        }

        size_t get_used_memory_size()
        {
            return __used_memory_size;
        }

        size_t get_cached_memory_size()
        {
            return __cached_memory_size;
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

        void set_garbage_collector(gc *gc_ptr)
        {
            __gc = gc_ptr;
        }

        const char* out_of_memory::what() const throw()
        {
            return "out of memory";
        }

        struct memory_descriptor
        {
            size_t size;
        };

        inline static size_t size_of_memory_block(void *p)
        {
            if (!p)
                return 0;
            memory_descriptor *d = (memory_descriptor*)p - 1;
            return d->size;
        }

        static void * alloc(size_t size)
        {
            memory_descriptor *d = nullptr;
            size_t total_size = sizeof(memory_descriptor) + size;
            do
            {
                if (total_size + __used_memory_size > __heap_size)
                {
                    if (!__gc)
                        break;
                    __gc->collect_garbage();
                    if (total_size + __used_memory_size > __heap_size)
                        break;
                    d = (memory_descriptor*)std::malloc(total_size);
                }
                else
                {
                    d = (memory_descriptor*)std::malloc(total_size);
                    if (!d)
                    {
                        if (!__gc)
                            break;
                        __gc->collect_garbage();
                        d = (memory_descriptor*)std::malloc(total_size);
                    }
                }
            } while(false);
            if(!d)
                throw out_of_memory();
            d->size = size;
            __used_memory_size += total_size;
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

        void it_is_a_cached_block(void *p)
        {
            __cached_memory_size += size_of_memory_block(p);
        }

        void it_is_a_not_cached_block(void *p)
        {
            __cached_memory_size -= size_of_memory_block(p);
        }
    };
};

void *operator new(size_t size)
{
    return g0at::lib::alloc(size);
}

void operator delete(void *p) noexcept
{
    g0at::lib::free(p);
}

void *operator new[](size_t size)
{
    return g0at::lib::alloc(size);
}

void operator delete[](void *p) noexcept
{
    g0at::lib::free(p);
}
