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

#include "Boolean.h"
#include "ObjectBoolean.h"
#include "Resource.h"

namespace goat {
	Boolean::Boolean(bool _value) : value(_value) {
	}

	Boolean * Boolean::toBoolean() {
		return this;
	}

	State * Boolean::createState(State *prev) {
		return new StateImpl(prev, this);
	}

	State * Boolean::StateImpl::next() {
		State *p = prev;
		p->ret(new ObjectBoolean(expr->value));
		delete this;
		return p;
	}

	String Boolean::toString() {
		return value ? Resource::s_true : Resource::s_false;
	}

	Token * Boolean::StateImpl::token() {
		return expr;
	}
}
