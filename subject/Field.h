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

#include "LeftExpression.h"
#include "Variable.h"

namespace goat {

	class Field : public LeftExpression {
	protected:
		class StateImpl : public State {
		public:
			Field *field;
			Object *left;
			Object **context;
			bool executed;

			StateImpl(State *_prev, Field *_field, Object **_context) 
				: State(_prev), field(_field), context(_context), left(nullptr), executed(false) {
			}
			State *next() override;
			void ret(Object *obj) override;
			void trace() override;
			Token * token() override;
		};

		class StateAssignImpl : public State {
		public:
			Object *obj;
			Field *field;
			Object *left;
			bool executed;

			StateAssignImpl(State *_prev, Object *_obj, Field *_field) 
				: State(_prev), obj(_obj), field(_field), left(nullptr), executed(false) {
			}
			State *next() override;
			void ret(Object *obj) override;
			void trace() override;
			Token * token() override;
		};

	public:
		Expression *left;
		String name;
		Int32 nameIndex;
		bool guard;

		Field(Expression *tokLeft, Variable *tokName, bool guard);
		Field *toField() override;
		State * createState(State *_prev) override;
		State * createState(State *_prev, Object **_context);
		State * createStateAssign(State *_prev, Object *_obj) override;
		void trace() override;
		String toString() override;
	};

}
