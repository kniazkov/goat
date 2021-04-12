/*

Copyright (C) 2021 Ivan Kniazkov

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

namespace g0at
{
    namespace lib
    {
        /**
         * @brief Fast memory allocator that allocates memory blocks during code block execution 
         *        and frees all of them at the end
         */
        class fast_allocator
        {
        public:
            /**
             * @brief Constructor
             * @param chunk_size Memory is allocated from the system heap by chunks of the same size. It's the size of one
             */
            fast_allocator(size_t chunk_size);

            /**
             * @bried Destuctor, frees all of allocated memory
             */
            ~fast_allocator();

            /**
             * @brief Allocates a memory block
             * @param size The required size
             * @return Allocated memory
             */
            void * alloc(size_t size);
            
        private:
 
            class memory_chunk
            {
            public:
                memory_chunk *next;
                char *data;
                char *ptr, *end_ptr;

                memory_chunk(size_t size);
                ~memory_chunk();
                void *alloc(size_t size);
            };

            size_t chunk_size;
            memory_chunk *head;

            fast_allocator(const fast_allocator &)
            {
            }

            void operator=(const fast_allocator &)
            {
            }
        };
    }
}