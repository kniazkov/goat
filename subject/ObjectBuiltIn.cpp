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

#include "ObjectBuiltIn.h"

namespace goat {

	ObjectBuiltIn::ObjectBuiltIn() : Object(true) {
		proto.pushBack(Proto::getInstance());
	}

	ObjectBuiltIn * ObjectBuiltIn::toObjectBuiltIn() {
		return this;
	}

	WideString ObjectBuiltIn::toWideString() {
		return L"Function";
	}

	State * ObjectBuiltIn::createState(State *prev) {
		return new StateImpl(prev, this);;
	}

	State * ObjectBuiltIn::StateImpl::next() {
		Object *r = obj->run(scope);
		if (r && r->toObjectException()) {
			return throw_(r);
		}
		State *p = prev;
		p->ret(r);
		delete this;
		return p;
	}

	void ObjectBuiltIn::StateImpl::trace() {
		obj->mark();
	}

	Token * ObjectBuiltIn::StateImpl::token() {
		return nullptr;
	}

	class BuiltInClone : public ObjectBuiltIn {
	public:
		Object * run(Scope *scope) override;
		static Object *getInstance();
	};

	ObjectBuiltIn::Proto::Proto() : Object(true) {
		status = PERMANENT;
		initialized = false;
	}

	void ObjectBuiltIn::Proto::init() {
		if (!initialized) {
			initialized = true;
			objects.insert("clone", BuiltInClone::getInstance());
		}
	}

	Object * ObjectBuiltIn::Proto::getInstance() {
		static Proto __this;
		__this.init();
		return &__this;
	}

	Object * BuiltInClone::run(Scope *scope) {
		return scope->this_;
	}

	Object * BuiltInClone::getInstance() {
		static BuiltInClone __this;
		return &__this;
	}
}
