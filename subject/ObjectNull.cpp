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

#include "ObjectNull.h"
#include "ObjectVoid.h"
#include "ObjectBoolean.h"
#include "Resource.h"

namespace goat {

	ObjectNull::ObjectNull() : Object(true) {
		proto.pushBack(ObjectVoid::getInstance());
	}

	ObjectVoid * ObjectNull::toObjectVoid() {
		return (ObjectVoid*)ObjectVoid::getInstance();
	}

	ObjectNull * ObjectNull::toObjectNull() {
		return this;
	}

	ObjectBoolean * ObjectNull::toObjectBoolean() {
		return new ObjectBoolean(false);
	}

	WideString ObjectNull::toWideString() {
		return Resource::w_null;
	}

	bool ObjectNull::equals(Object *obj) {
		return obj == this || obj == ObjectVoid::getInstance();
	}

	Object *  ObjectNull::getInstance() {
		static ObjectNull __this;
		return &__this;
	}
}
