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

#include "DoWhile.h"
#include "ObjectBoolean.h"
#include "StringBuilder.h"

namespace goat {

	DoWhile::DoWhile(Expression *_expr, Statement *_stmt) {
		loc = _stmt->loc;
		expr = _expr;
		stmt = _stmt;
	}

	void DoWhile::trace() {
		expr->mark();
		stmt->mark();
	}

	DoWhile * DoWhile::toDoWhile() {
		return this;
	}

	State * DoWhile::createState(State *_prev) {
		return new StateImpl(_prev, this);
	}

	State * DoWhile::StateImpl::execute() {
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
			return stmt->stmt->createState(this); // see below : case EXECUTE
		default:
			throw NotImplemented();
		}
	}

	State * DoWhile::StateImpl::next() {
		switch (step) {
		case EXECUTE:
			step = CHECK_CONDITION;
			return stmt->stmt->createState(this);
		case CHECK_CONDITION:
			return stmt->expr->createState(this);
		case DONE: {
			State *p = prev;
			delete this;
			return p;
		}
		default:
			throw NotImplemented();
		}
	}

	void DoWhile::StateImpl::ret(Object *obj) {
		switch (step) {
		case CHECK_CONDITION:
			if (obj->toObjectBoolean()->value)
				step = EXECUTE;
			else
				step = DONE;
			break;
		default:
			throw NotImplemented();
		}
	}

	String DoWhile::toString() {
		return (StringBuilder() << "do " << stmt->toString() << " while(" << expr->toString() << ')').toString();
	}
}
