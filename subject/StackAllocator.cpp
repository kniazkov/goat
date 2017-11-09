/*

Copyright (C) 2017 Ivan Kniazkov

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

#include "StackAllocator.h"
#include "Utils.h"

#if 0
#include <ctime>
#include <iostream>
#endif

namespace goat {

	StackAllocator::StackAllocator() {
		top = new Chunk(nullptr);
	}

	StackAllocator::~StackAllocator() {
		while (top) {
			Chunk *next = top->next;
			delete top;
			top = next;
		}
	}

	void * StackAllocator::alloc(unsigned int size) {
		size = Utils::alignUp(size, (unsigned int)sizeof(int));
		void * mem = top->alloc(size);
		if (!mem) {
			top = new Chunk(top);
			mem = top->alloc(size);
		}
		return mem;
	}

	void * StackAllocator::alloc0(unsigned int size) {
		size = Utils::alignUp(size, (unsigned int)sizeof(int));
		int *mem = (int*)alloc(size);
		Utils::memSet(mem, 0, size / sizeof(int));
		return (void*)mem;
	}

	void StackAllocator::free(void *ptr) {
		if (top->data != top->ptr) {
			top->free(ptr);
		}
		else {
			Chunk *next = top->next;
			delete top;
			top = next;
			if (top) {
				top->free(ptr);
			}
			else {
				throw IncorrectPointer();
			}
 		}
	}

	StackAllocator::Chunk::Chunk(Chunk *next) {
		this->next = next;
		ptr = data;
		end = data + chunkSize;
	}

	void * StackAllocator::Chunk::alloc(unsigned int size) {
		if (size > maxObjectSize) {
			throw ObjectTooBig();
		}

		if (ptr + size + sizeof(unsigned int) > end) {
			return nullptr;
		}

		void *mem = ptr;
		ptr += size;
		*((unsigned int*)ptr) = size;
		ptr += sizeof(unsigned int);
		return mem;
	}

	void StackAllocator::Chunk::free(void *mem) {
		int *size = (int*)(ptr - sizeof(unsigned int));
		char *begin = (char*)size - *size;
		if (begin != mem) {
			throw IncorrectPointer();
		}
		ptr = begin;
	}

	RawString StackAllocator::ObjectTooBig::toRawString() {
		return L"stack allocator: object is too big";
	}

	RawString StackAllocator::IncorrectPointer::toRawString() {
		return L"stack allocator: incorrect pointer, attempt to delete not the last object";
	}

#if 0
	void StackAllocator::unitTest() {
		static const int count = 1000000;
		StackAllocator a;
		char **m = new char*[count];
		int i;
		for (i = 0; i < count; i++) {
			m[i] = (char*)a.alloc0(i % 32);
		}
		for (i = count - 1; i >= 0; i--) {
			a.free(m[i]);
		}
		delete[] m;
	}

	void StackAllocator::unitTest2() {
		clock_t x = clock();
		{
			static const int count = 1000000;
			StackAllocator a;
			char **m = new char*[count];
			int i;
			for (i = 0; i < count; i++) {
				m[i] = (char*)a.alloc(i % 32);
			}
			for (i = count - 1; i >= 0; i--) {
				a.free(m[i]);
			}
			delete[] m;
		}
		std::cout << clock() - x << "\n";

		x = clock();
		{
			static const int count = 1000000;
			StackAllocator a;
			char **m = new char*[count];
			int i;
			for (i = 0; i < count; i++) {
				m[i] = new char[i % 32];
			}
			for (i = count - 1; i >= 0; i--) {
				delete[] m[i];
			}
			delete[] m;
		}
		std::cout << clock() - x << "\n";
	}
#endif
}