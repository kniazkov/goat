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

#include "If.h"
#include "ObjectBoolean.h"
#include "StringBuilder.h"

namespace goat {

	If::If(Expression *_expr, Statement *_stmtIf, Statement *_stmtElse) {
		loc = _expr->loc;
		expr = _expr;
		stmtIf = _stmtIf;
		stmtElse = _stmtElse;
	}

	void If::trace() {
		expr->mark();
		stmtIf->mark();
		if (stmtElse) {
			stmtElse->mark();
		}
	}

	If * If::toIf() {
		return this;
	}

	State * If::createState(State *_prev) {
		return new StateImpl(_prev, this);
	}

	State * If::StateImpl::next() {
		if (!executed) {
			if (!result) {
				return stmt->expr->createState(this);
			}
			executed = true;
			if (result->toObjectBoolean()->value) {
				return stmt->stmtIf->createState(this);
			}
			else if (stmt->stmtElse) {
				return stmt->stmtElse->createState(this);
			}
			State *p = prev;
			delete this;
			return p;
		}
		else {
			State *p = prev;
			delete this;
			return p;
		}
	}

	void If::StateImpl::ret(Object *obj) {
		if (!result) {
			result = obj;
		}
		// just ignore the object
	}

	void If::StateImpl::trace() {
		if (result) {
			result->mark();
		}
	}

	String If::toString() {
		StringBuilder b;
		b << "if (" << expr->toString() << ") " << stmtIf->toString();
		if (stmtElse) {
			b << " else " << stmtElse->toString();
		}
		return b.toString();
	}
}
