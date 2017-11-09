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

#include "Assert.h"
#include "BracketExpression.h"

namespace goat {

	BracketExpression::BracketExpression(Brackets *_brackets) {
		assert(_brackets->symbol == '(');

		loc = _brackets->loc;
		tokens = _brackets->tokens;
		_brackets->tokens = nullptr;
	}

	BracketExpression::~BracketExpression() {
		delete tokens;
	}

	BracketExpression * BracketExpression::toBracketExpression() {
		return this;
	}

	void BracketExpression::trace() {
		tokens->mark();
	}

	State * BracketExpression::createState(State *_prev) {
		return new StateImpl(_prev, this);
	}

	State * BracketExpression::StateImpl::next() {
		if (!executed) {
			executed = true;
			assert(expr->tokens->count == 1);
			Expression *innerExpr = expr->tokens->first->toExpression();
			assert(innerExpr != nullptr);
			return innerExpr->createState(this);
		}
		else {
			State *p = prev;
			delete this;
			return p;
		}
	}

	void BracketExpression::StateImpl::ret(Object *obj) {
		prev->ret(obj);
	}

}