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

#include "State.h"
#include "Thread.h"

namespace goat {

	void * State::operator new(MemorySize size) {
		return Thread::current->allocator.alloc((unsigned int)size);
	}

	void State::operator delete(void *ptr) {
		Thread::current->allocator.free(ptr);
	}

	State::State(State *_prev) : mode(RUN), thru(nullptr), prev(_prev) {
		if (prev) {
			scope = prev->scope;
		}
		else {
			scope = nullptr;
		}
	}

	State * State::execute() {
		switch (mode) 
		{
		case RUN:
			return next();
		case EXCEPTION:
			return throw_(thru);
		case RETURN:
			return return_(thru);
		case BREAK:
			return break_();
		default:
			throw NotImplemented();
		}
	}

	void State::ret(Object *obj) {
		throw NotImplemented();
	}

	void State::mark(bool deep) {
		if (deep && scope) {
			scope->mark();
		}
		if (thru) {
			thru->mark();
		}
		trace();
		if (prev) {
			prev->mark(false);
		}
	}

	void State::trace() {

	}

	State * State::throw_(Object *obj) {
		if (prev) {
			State *p = prev;
			p->mode = EXCEPTION;
			p->thru = obj;
			delete this;
			return p;
		}
		else {
			throw UncaughtException(thru);
		}
	}

	State * State::return_(Object *obj) {
		if (prev) {
			State *p = prev;
			p->mode = RETURN;
			p->thru = obj;
			delete this;
			return p;
		}
		else {
			throw NotImplemented();
		}
	}

	State * State::break_() {
		if (prev) {
			State *p = prev;
			p->mode = BREAK;
			delete this;
			return p;
		}
		else {
			throw NotImplemented();
		}
	}

	RawString State::NotImplemented::toRawString() {
		return L"state is not implemented";
	}

	RawString State::UncaughtException::toRawString() {
		return obj ? obj->toRawString() : RawString();
	}

}
