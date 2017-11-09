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

#include "StatementExpression.h"

namespace goat {

	StatementExpression::StatementExpression(Expression *_expr) {
		loc = _expr->loc;
		expr = _expr;
	}

	void StatementExpression::trace() {
		expr->mark();
	}

	StatementExpression * StatementExpression::toStatementExpression() {
		return this;
	}

	State * StatementExpression::createState(State *_prev) {
		return new StateImpl(_prev, this);
	}

	State * StatementExpression::StateImpl::next() {
		if (!executed) {
			executed = true;
			return stexpr->expr->createState(this);
		}
		else {
			State *p = prev;
			delete this;
			return p;
		}
	}

	void StatementExpression::StateImpl::ret(Object *obj) {
		// just ignore the object
	}
}
