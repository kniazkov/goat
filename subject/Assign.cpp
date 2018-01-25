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

#include "Assign.h"
#include "StringBuilder.h"

namespace goat {

	Assign::Assign(Operator *_oper, LeftExpression *_left, Expression *_right) {
		loc = _oper->loc;
		left = _left;
		right = _right;
	}

	Assign * Assign::toAssign() {
		return this;
	}

	void Assign::trace() {
		left->mark();
		right->mark();
	}

	State *Assign::createState(State *_prev) {
		return new StateImpl(_prev, this);
	}

	State * Assign::StateImpl::next() {
		if (!executed) {
			if (!right) {
				return expr->right->createState(this);
			}
			executed = true;
			return expr->left->createStateAssign(this, right);
		}
		else {
			State *p = prev;
			delete this;
			return p;
		}
	}

	void Assign::StateImpl::ret(Object *obj) {
		if (!right) {
			right = obj;
		}
		else {
			prev->ret(obj);
		}
	}

	void Assign::StateImpl::trace() {
		if (right) {
			right->mark();
		}
	}

	String Assign::toString() {
		return (StringBuilder() << left->toString() << " = " << right->toString()).toString();
	}
}
