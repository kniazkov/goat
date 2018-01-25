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

#include "CreateFunction.h"
#include "ObjectFunction.h"
#include "ObjectThread.h"

namespace goat {

	CreateFunction::CreateFunction(Function *_func, bool _thread) {
		loc = _func->loc;
		func = _func;
		thread = _thread;
	}

	CreateFunction * CreateFunction::toCreateFunction() {
		return this;
	}

	State * CreateFunction::createState(State *prev) {
		return new StateImpl(prev, this);
	}

	void CreateFunction::trace() {
		func->mark();
	}

	State * CreateFunction::StateImpl::next() {
		State *p = prev;
		if (!expr->thread)
			p->ret(new ObjectFunction(expr->func, scope));
		else
			p->ret(new ObjectThread(expr->func, scope));
		delete this;
		return p;
	}

	String CreateFunction::toString() {
		return func->toString();
	}

	Token * CreateFunction::StateImpl::token() {
		return expr;
	}

}