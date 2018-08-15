/*

Copyright (C) 2017, 2018 Ivan Kniazkov

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

#include "Type.h"
#include "Assert.h"

namespace goat {

	template <SizeT Size> class Pool {
	protected:
		Pool(const Pool &) {
		}

		void operator=(const Pool &) {
		}

		union Block
		{
			Block * next;
			char memory[Size];
		};

		Block * top;

	public:
		SizeT used, unused;
		const SizeT size = Size;

		Pool() : top(nullptr), used(0), unused(0) {
		}

		~Pool() {
			Block *b = top,
				*n;

			while (b) {
				n = b->next;
				delete b;
				b = n;
			}
		}

		void * alloc() {
			Block *b;
			if (top) {
				b = top;
				top = top->next;
				unused--;
			}
			else
				b = new Block();
			used++;
			return (void*)b->memory;
		}

		void * alloc(SizeT size) {
			assert(size <= Size);
			return alloc();
		}

		void free(void *ptr) {
			Block * b = (Block*)ptr;
			b->next = top;
			top = b;
			unused++;
			used--;
		}

		/*
		void reduce(MemorySize maxUnused = 0) {
			Block *b = top,
				*n;

			while (b && unused > maxUnused) {
				n = b->next;
				delete b;
				b = n;
				unused--;
			}
		}
		*/

		static Pool & getInstance();
	};
}
