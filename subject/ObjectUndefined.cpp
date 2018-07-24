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
#include "ObjectBoolean.h"
#include "Resource.h"

namespace goat {

	ObjectUndefined::ObjectUndefined() : Object(true) {
		objects.insert(createIndex("!"), OperatorNot::getInstance());
		objects.insert(createIndex("||"), OperatorOr::getInstance());

		proto.pushBack(SuperObject::getInstance());
	}

	ObjectUndefined * ObjectUndefined::toObjectUndefined() {
		return this;
	}

	ObjectBoolean * ObjectUndefined::toObjectBoolean() {
		return new ObjectBoolean(false);
	}

	WideString ObjectUndefined::toWideString() {
		return Resource::w_undefined;
	}

	Object *  ObjectUndefined::getInstance() {
		static ObjectUndefined __this;
		return &__this;
	}


	Object * ObjectUndefined::OperatorNot::run(Scope *scope) {
		return new ObjectBoolean(true);
	}

	Object * ObjectUndefined::OperatorNot::getInstance() {
		static OperatorNot __this;
		return &__this;
	}


	Object * ObjectUndefined::OperatorOr::run(Scope *scope) {
		return scope->arguments->vector[0];
	}

	Object * ObjectUndefined::OperatorOr::getInstance() {
		static OperatorOr __this;
		return &__this;
	}
}
