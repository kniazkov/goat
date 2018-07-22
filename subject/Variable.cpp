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

#include "Variable.h"

namespace goat {

	Variable::Variable(Identifier *tokName) {
		loc = tokName->loc;
		name = tokName->name;
		nameIndex = Object::createIndex(name);
		identifier = tokName;
	}

	Variable * Variable::toVariable() {
		return this;
	}

	Identifier * Variable::toIdentifier() {
		return identifier;
	}

	State * Variable::createState(State *prev) {
		return new StateImpl(prev, this);
	}

	State * Variable::createStateAssign(State *prev, Object *obj) {
		return new StateAssignImpl(prev, obj, this);
	}

	void Variable::trace() {
		identifier->mark();
	}

	State * Variable::StateImpl::next() {
		State *p = prev;
		p->ret(scope->find(var->nameIndex));
		delete this;
		return p;
	}

	Token * Variable::StateImpl::token() {
		return var;
	}

	State * Variable::StateAssignImpl::next() {
		State *p = prev;
		scope->replace(var->name, obj);
		p->ret(obj);
		delete this;
		return p;
	}

	Token * Variable::StateAssignImpl::token() {
		return var;
	}

	void Variable::StateAssignImpl::trace() {
		if (obj) {
			obj->mark();
		}
	}

	String Variable::toString() {
		return name;
	}
}
