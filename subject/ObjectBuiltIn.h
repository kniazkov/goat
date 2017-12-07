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
#include "Scope.h"
#include "State.h"

namespace goat {

	class ObjectBuiltIn : public Object {
	public:
		ObjectBuiltIn();
		ObjectBuiltIn * toObjectBuiltIn() override;
		WideString toWideString() override;
		virtual Object *run(Scope *scope) = 0;
		virtual State * createState(State *_prev);

		class StateImpl : public State {
		public:
			ObjectBuiltIn *obj;

			StateImpl(State *_prev, ObjectBuiltIn *_obj) : State(_prev), obj(_obj) {
			}
			State *next() override;
			void trace() override;
		};

		class Proto : public Object {
		protected:
			bool initialized;
			void init();
		public:
			Proto();
			static Object *getInstance();
		};

	};

}
