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
#include "ObjectBoolean.h"
#include "Resource.h"

namespace goat {

	ObjectNull::ObjectNull() : Object(true) {
		objects.insert(createIndex("!"), OperatorNot::getInstance());
		objects.insert(createIndex("||"), OperatorOr::getInstance());

		proto.pushBack(SuperObject::getInstance());
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

	Object *  ObjectNull::getInstance() {
		static ObjectNull __this;
		return &__this;
	}


	Object * ObjectNull::OperatorNot::run(Scope *scope) {
		return new ObjectBoolean(true);
	}

	Object * ObjectNull::OperatorNot::getInstance() {
		static OperatorNot __this;
		return &__this;
	}


	Object * ObjectNull::OperatorOr::run(Scope *scope) {
		return scope->arguments->vector[0];
	}

	Object * ObjectNull::OperatorOr::getInstance() {
		static OperatorOr __this;
		return &__this;
	}
}
