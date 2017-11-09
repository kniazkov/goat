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

#pragma once

#include "Exception.h"

namespace goat {

	class StackAllocator {
	private:
		static const unsigned int chunkSize = 65536 - 1024;
		static const unsigned int maxObjectSize = 16 * sizeof(void*);

		class Chunk {
		public:
			Chunk *next;
			char data[chunkSize],
				*ptr, *end;

			Chunk(Chunk *next);
			void * alloc(unsigned int size);
			void free(void *mem);
		};

		Chunk *top;

	protected:
		StackAllocator(const StackAllocator &) {
		}

		void operator=(const StackAllocator &) {
		}

	public:
		
		class ObjectTooBig : public Exception {
		public:
			RawString toRawString() override;
		};

		class IncorrectPointer : public Exception {
		public:
			RawString toRawString() override;
		};

		StackAllocator();
		~StackAllocator();
		void * alloc(unsigned int size);
		void * alloc0(unsigned int size);
		void free(void *ptr);

#if 0
		static void unitTest();
		static void unitTest2();
#endif
	};

}