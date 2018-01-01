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

#include "For.h"
#include "ObjectBoolean.h"

namespace goat {

	For::For(Keyword *_kw, Statement *_init, Expression *_condition, Statement *_increment, Statement *_body) {
		loc = _kw->loc;
		init = _init;
		condition = _condition;
		increment = _increment;
		body = _body;
	}

	void For::trace() {
		init->mark();
		condition->mark();
		increment->mark();
		body->mark();
	}

	For * For::toFor() {
		return this;
	}

	State * For::createState(State *_prev) {
		return new StateImpl(_prev, this);
	}

	For::StateImpl::StateImpl(State *_prev, For *_stmt) : State(_prev), stmt(_stmt), condition(nullptr), step(INIT) {
		cloneScope();
	}

	State * For::StateImpl::execute() {
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
			step = CHECK_CONDITION;
			return stmt->increment->createState(this); // see below : case INCREMENT
		default:
			throw NotImplemented();
		}
	}

	State * For::StateImpl::next() {
		switch (step) {
		case INIT:
			step = CHECK_CONDITION;
			return stmt->init->createState(this);
		case CHECK_CONDITION:
			return stmt->condition->createState(this);
		case EXECUTE:
			if (condition->toObjectBoolean()->value) {
				condition = nullptr;
				step = INCREMENT;
				return stmt->body->createState(this);
			}
			else {
				State *p = prev;
				delete this;
				return p;
			}
		case INCREMENT:
			step = CHECK_CONDITION;
			return stmt->increment->createState(this);
		default:
			throw NotImplemented();
		}
	}

	void For::StateImpl::ret(Object *obj) {
		switch (step) {
		case CHECK_CONDITION:
			condition = obj;
			step = EXECUTE;
			break;
		default:
			throw NotImplemented();
		}
	}

	void For::StateImpl::trace() {
		if (condition) {
			condition->mark();
		}
	}
}
