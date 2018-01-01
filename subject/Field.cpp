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

#include "Field.h"
#include "Assert.h"
#include "ObjectException.h"

namespace goat {

	Field::Field(Expression *tokLeft, Variable *tokName) {
		loc = tokName->loc;
		left = tokLeft;
		name = tokName->name;
	}

	Field * Field::toField() {
		return this;
	}

	State * Field::createState(State *prev) {
		return new StateImpl(prev, this, nullptr);
	}

	State * Field::createState(State *prev, Object **context) {
		return new StateImpl(prev, this, context);
	}

	State * Field::createStateAssign(State *prev, Object *obj) {
		return new StateAssignImpl(prev, obj, this);
	}

	void Field::trace() {
		left->mark();
	}

	State * Field::StateImpl::next() {
		if (!executed) {
			executed = true;
			return field->left->createState(this);
		}
		else {
			if (!left) {
				return throw_(new CanNotReadPropertyOfUndefined(field->name));
			}
			if (context) {
				*context = left;
			}
			State *p = prev;
			p->ret(left->find(field->name));
			delete this;
			return p;
		}
	}

	void Field::StateImpl::ret(Object *obj) {
		left = obj;
	}

	void Field::StateImpl::trace() {
		if(left) {
			left->mark();
		}
	}

	State * Field::StateAssignImpl::next() {
		if (!executed) {
			if (!left) {
				return field->left->createState(this);
			}
			executed = true;
			State *p = prev;
			left->objects.insert(field->name, obj);
			p->ret(obj);
			delete this;
			return p;
		}
		else {
			State *p = prev;
			delete this;
			return p;
		}
	}

	void Field::StateAssignImpl::ret(Object *obj) {
		left = obj;
	}

	void Field::StateAssignImpl::trace() {
		if (obj) {
			obj->mark();
		}
		if (left) {
			left->mark();
		}
	}

}
