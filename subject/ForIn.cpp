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

#include "ForIn.h"
#include "StringBuilder.h"

namespace goat {

	ForIn::ForIn(Keyword *_kw, In *_in, Expression *_obj, Statement *_body) {
		loc = _kw->loc;
		in = _in;
		obj = _obj;
		body = _body;
	}

	void ForIn::trace() {
		in->mark();
		obj->mark();
		body->mark();
	}

	ForIn * ForIn::toForIn() {
		return this;
	}

	State * ForIn::createState(State *_prev) {
		return new StateImpl(_prev, this);
	}

	ForIn::StateImpl::StateImpl(State *_prev, ForIn *_stmt) : State(_prev), stmt(_stmt), step(GET_OBJECT) {
		if (stmt->in->declVar) {
			cloneScope();
			scope->objects.insert(Object::createIndex(stmt->in->name->name), nullptr);
		}
		index = 0;
	}

	State * ForIn::StateImpl::execute() {
		switch (mode)
		{
		case RUN:
			return next();
		case EXCEPTION:
			return throw_(thru);
		case RETURN:
			return return_(thru);
		case BREAK: {
			State *p = prev;
			delete this;
			return p;
		}
		case CONTINUE:
			mode = RUN;
			step = EXECUTE;
			return next();
		default:
			throw NotImplemented();
		}
	}

	State * ForIn::StateImpl::next() {
		switch (step) {
		case GET_OBJECT:
			return stmt->obj->createState(this);
		case EXECUTE:
			if (index < vector.len()) {
				Object::Pair pair = vector[index];
				index++;
				scope->replace(stmt->in->name->name, pair.key);
				return stmt->body->createState(this);
			}
			else {
				step = DONE;
			}
		case DONE: {
			State *p = prev;
			delete this;
			return p;
		}
		default:
			throw NotImplemented();
		}
	}

	void ForIn::StateImpl::ret(Object *obj) {
		switch (step) {
		case GET_OBJECT:
			if (obj) {
				obj->enumerate(&vector);
			}
			step = EXECUTE;
			break;
		default:
			throw NotImplemented();
		}
	}

	void ForIn::StateImpl::trace() {
		vector.forEach([](Object::Pair &pair) {
			if (pair.key)
				pair.key->mark();
			if (pair.value)
				pair.value->mark();
		});
	}

	String ForIn::toString() {
		return (StringBuilder() << "for (" << in->toString() << " in " << 
			obj->toString() << ") " << body->toString()).toString();
	}

	Token * ForIn::StateImpl::token() {
		return stmt;
	}
}
