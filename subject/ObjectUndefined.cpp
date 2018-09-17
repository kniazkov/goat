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

#include "ObjectUndefined.h"
#include "ObjectVoid.h"
#include "ObjectBoolean.h"
#include "Resource.h"

namespace goat {

	ObjectUndefined::ObjectUndefined() : Object(true) {
		proto.pushBack(ObjectVoid::getInstance());
	}

	ObjectVoid * ObjectUndefined::toObjectVoid() {
		return (ObjectVoid*)ObjectVoid::getInstance();
	}

	ObjectUndefined * ObjectUndefined::toObjectUndefined() {
		return this;
	}

	ObjectBoolean * ObjectUndefined::toObjectBoolean() {
		return new ObjectBoolean(false);
	}

	WideString ObjectUndefined::toWideString(Set<Object*> &set) {
		return Resource::w_undefined;
	}

	bool ObjectUndefined::equals(Object *obj) {
		return obj == this || obj == ObjectVoid::getInstance();
	}

	Object *  ObjectUndefined::getInstance() {
		static ObjectUndefined __this;
		return &__this;
	}
}
