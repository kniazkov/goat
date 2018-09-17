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

#pragma once

#include "Object.h"
#include "ObjectBuiltIn.h"

namespace goat {

	class ObjectBoolean;
	class ObjectVoid;

	class ObjectNull : public Object {
	public:
		ObjectNull();
		ObjectVoid * toObjectVoid() override;
		ObjectNull * toObjectNull() override;
		ObjectBoolean * toObjectBoolean() override;
		WideString toWideString(Set<Object*> &set) override;
		bool equals(Object *obj) override;
		static Object *getInstance();
	};

}
