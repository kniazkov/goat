/*

Copyright (C) 2017 Ivan Kniazkov

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

	class ObjectChar : public Object {
	public:
		wchar value;

		ObjectChar(wchar _value);
		ObjectChar * toObjectChar() override;
		WideString toWideString() override;
		WideString toWideStringNotation() override;
		bool equals(Object *_obj) override;

		class Proto : public Object {
		public:
			Proto();
			static Object *getInstance();

			class OperatorLess : public ObjectBuiltIn {
			public:
				Object * run(Scope *scope) override;
				static Object *getInstance();
			};

			class OperatorEqual : public ObjectBuiltIn {
			public:
				Object * run(Scope *scope) override;
				static Object *getInstance();
			};

			class OperatorNotEqual : public ObjectBuiltIn {
			public:
				Object * run(Scope *scope) override;
				static Object *getInstance();
			};

			class OperatorIncrement : public ObjectBuiltIn {
			public:
				Object * run(Scope *scope) override;
				static Object *getInstance();
			};

			class OperatorDecrement : public ObjectBuiltIn {
			public:
				Object * run(Scope *scope) override;
				static Object *getInstance();
			};

			class Clone : public ObjectBuiltIn {
			public:
				Object * run(Scope *scope) override;
				static Object *getInstance();
			};
		};
	};

}
