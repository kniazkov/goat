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

#include "While.h"
#include "ObjectBoolean.h"

namespace goat {

	While::While(Expression *_expr, Statement *_stmt) {
		loc = _expr->loc;
		expr = _expr;
		stmt = _stmt;
	}

	void While::trace() {
		expr->mark();
		stmt->mark();
	}

	While * While::toWhile() {
		return this;
	}

	State * While::createState(State *_prev) {
		return new StateImpl(_prev, this);
	}

	State * While::StateImpl::next() {
		switch (step) {
		case CHECK_CONDITION:
			return stmt->expr->createState(this);
		case EXECUTE:
			if (condition->toObjectBoolean()->value) {
				step = CHECK_CONDITION;
				return stmt->stmt->createState(this);
			}
			else {
				State *p = prev;
				delete this;
				return p;
			}
		default:
			throw NotImplemented();
		}
	}

	void While::StateImpl::ret(Object *obj) {
		switch (step) {
		case CHECK_CONDITION:
			condition = obj;
			step = EXECUTE;
			break;
		default:
			throw NotImplemented();
		}
	}

	void While::StateImpl::trace() {
		if (condition) {
			condition->mark();
		}
	}
}
