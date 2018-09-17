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

#include "ObjectVoid.h"
#include "ObjectBoolean.h"
#include "ObjectUndefined.h"
#include "Resource.h"

namespace goat {

	ObjectVoid::ObjectVoid() : Object(true) {
		objects.insert(createIndex("!"), OperatorNot::getInstance());
		objects.insert(createIndex("||"), OperatorOr::getInstance());

		proto.pushBack(SuperObject::getInstance());
	}

	ObjectUndefined * ObjectVoid::toObjectUndefined() {
		return (ObjectUndefined*)ObjectUndefined::getInstance();
	}

	ObjectVoid * ObjectVoid::toObjectVoid() {
		return this;
	}

	ObjectBoolean * ObjectVoid::toObjectBoolean() {
		return new ObjectBoolean(false);
	}

	WideString ObjectVoid::toWideString(Set<Object*> &set) {
		return Resource::w_void;
	}

	bool ObjectVoid::equals(Object *obj) {
		return obj->toObjectVoid() != nullptr;
	}

	Object *  ObjectVoid::getInstance() {
		static ObjectVoid __this;
		return &__this;
	}


	Object * ObjectVoid::OperatorNot::run(Scope *scope) {
		return new ObjectBoolean(true);
	}

	Object * ObjectVoid::OperatorNot::getInstance() {
		static OperatorNot __this;
		return &__this;
	}


	Object * ObjectVoid::OperatorOr::run(Scope *scope) {
		return scope->arguments->vector[0];
	}

	Object * ObjectVoid::OperatorOr::getInstance() {
		static OperatorOr __this;
		return &__this;
	}
}
