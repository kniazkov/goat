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

#include "Thread.h"

namespace goat {

	Thread * Thread::current;

	ThreadList ThreadList::global;

	Thread::Thread(Function *func, Scope *scope) {
		list = nullptr;
		ThreadList::global.pushBack(this);
		Thread *creator = current;
		current = this;
		state = func->createState(nullptr);
		state->scope = scope;
		if (creator != nullptr) {
			current = creator;
		}
		mode = State::DebugMode::BREAKPOINT;
	}

	Thread::~Thread() {
		if (current == this) {
			current = next;
		}
		ThreadList::global.remove(this);
		if (current == nullptr) {
			current = ThreadList::global.first;
		}
	}

	bool Thread::step() {
		if (!state) {
			return false;
		}

		state = state->execute();
		return true;
	}

	void ThreadList::mark() {
		forEach([](Thread *th) {
			if (th->state) {
				th->state->mark();
			}
		});
	}
	
}