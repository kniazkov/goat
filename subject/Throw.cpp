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

#include "Throw.h"
#include "StringBuilder.h"

namespace goat {

	Throw::Throw(Keyword *_kw, Expression *_expr) {
		loc = _kw->loc;
		expr = _expr;
	}

	void Throw::trace() {
		if (expr) {
			expr->mark();
		}
	}

	Throw * Throw::toThrow() {
		return this;
	}

	State * Throw::createState(State *_prev) {
		return new StateImpl(_prev, this);
	}

	State * Throw::StateImpl::next() {
		if (!executed) {
			executed = true;
			return stmt->expr->createState(this);
		}
		else {
			return throw_(obj);
		}
	}

	void Throw::StateImpl::ret(Object *obj) {
		this->obj = obj;
	}

	void Throw::StateImpl::trace() {
		if (obj) {
			obj->mark();
		}
	}

	String Throw::toString() {
		return (StringBuilder() << "throw " << expr->toString() << ';').toString();
	}

	Token * Throw::StateImpl::token() {
		return stmt;
	}
}
