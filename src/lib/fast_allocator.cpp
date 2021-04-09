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

#include "fast_allocator.h"
#include "assert.h"

namespace g0at
{
    namespace lib
    {
        fast_allocator::fast_allocator(size_t size)
        {
            chunk_size = size;
            head = new memory_chunk(size);
        }

        fast_allocator::~fast_allocator()
        {
            memory_chunk *chunk = head;
            while(chunk)
            {
                memory_chunk *next = chunk->next;
                delete chunk;
                chunk = next;
            }
        }

        void * fast_allocator::alloc(size_t size)
        {
            if (size > chunk_size)
            {
                memory_chunk *new_chunk = new memory_chunk(size);
                new_chunk->next = head->next;
                head->next = new_chunk;
                void *block = new_chunk->alloc(size);
                assert(block != nullptr);
                return block;
            }
            else
            {
                void *block = head->alloc(size);
                if (block)
                    return block;
                memory_chunk *new_chunk = new memory_chunk(chunk_size);
                new_chunk->next = head;
                head = new_chunk;
                block = head->alloc(size);
                assert(block != nullptr);
                return block; 
            }
        }

        fast_allocator::memory_chunk::memory_chunk(size_t size)
        {
            next = nullptr;
            data = new char[size];
            ptr = data;
            end_ptr = data + size;
        }

        fast_allocator::memory_chunk::~memory_chunk()
        {
            delete[] data;
        }

        void * fast_allocator::memory_chunk::alloc(size_t size)
        {
            if (ptr + size > end_ptr)
                return nullptr;
            void *block = (void*)ptr;
            ptr += size;
            return block;
        }
    }
};