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

#include "State.h"
#include "Thread.h"
#include "ObjectArray.h"
#include "ObjectString.h"
#include "Resource.h"

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
		case CONTINUE:
			return continue_();
		default:
			throw NotImplemented();
		}
	}

	void State::ret(Object *obj) {
		throw NotImplemented();
	}

	void State::mark() {
		if (scope) {
			scope->mark();
		}
		old.forEach([](Scope *o) {
			o->mark();
		});
		if (thru) {
			thru->mark();
		}
		trace();

		State *st = prev;
		while (st) {
			if (st->thru) {
				st->thru->mark();
			}
			st->old.forEach([](Scope *o) {
				o->mark();
			});
			st->trace();
			st = st->prev;
		}
	}

	void State::trace() {

	}

	State * State::throw_(Object *obj) {
		if (obj) {
			Location *l = location();
			if (l) {
				// write stack trace information
				ObjectArray *t = nullptr;
				Object *f = obj->find(Resource::trace);
				if (f) {
					t = f->toObjectArray();
				}
				if (!t) {
					t = new ObjectArray();
					obj->insert(Resource::trace, t);
				}
				t->vector.pushBack(new ObjectString(l->toString().toWideString()));
			}
		}
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

	State * State::continue_() {
		if (prev) {
			State *p = prev;
			p->mode = CONTINUE;
			delete this;
			return p;
		}
		else {
			throw NotImplemented();
		}
	}

	Location * State::location() {
		return nullptr;
	}

	String State::toString() {
		return nullptr;
	}

	RawString State::NotImplemented::toRawString() {
		return L"state is not implemented";
	}

	RawString State::UncaughtException::toRawString() {
		return obj ? obj->toRawString() : RawString();
	}

}
