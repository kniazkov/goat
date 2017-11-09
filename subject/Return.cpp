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

#include "Return.h"

namespace goat {

	Return::Return(Expression *_expr) {
		loc = _expr->loc;
		expr = _expr;
	}

	void Return::trace() {
		expr->mark();
	}

	Return * Return::toReturn() {
		return this;
	}

	State * Return::createState(State *_prev) {
		return new StateImpl(_prev, this);
	}

	State * Return::StateImpl::next() {
		if (!executed) {
			executed = true;
			if (expr->expr) {
				return expr->expr->createState(this);
			}
			return return_(nullptr);
		}
		else {
			return return_(obj);
		}
	}

	void Return::StateImpl::ret(Object *obj) {
		this->obj = obj;
	}

	void Return::StateImpl::trace() {
		if (obj) {
			obj->mark();
		}
	}
}
