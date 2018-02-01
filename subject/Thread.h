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

#include "StackAllocator.h"
#include "State.h"
#include "MultiList.h"
#include "Function.h"

namespace goat {

	class Thread {
	protected:
		Thread(const Thread &) {
		}

		void operator=(const Thread &) {
		}

	public:
		void *list;
		Thread *prev, *next;
		StackAllocator allocator;
		State *state;
		State::DebugMode mode;
		unsigned int level;
		static Thread *current;

		Thread(Function *func, Scope *scope);
		~Thread();
		bool step();
	};

	class ThreadList : public MultiList<Thread, &Thread::prev, &Thread::next, &Thread::list> {
	public:
		static ThreadList global;
		void mark();
	};
}