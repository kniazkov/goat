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
#include "Identifier.h"

namespace goat {

	class Variable : public LeftExpression {
	protected:
		class StateImpl : public State {
		public:
			Variable *var;

			StateImpl(State *_prev, Variable *_var) : State(_prev), var(_var) {
			}
			State *next() override;
			Token *token() override;
		};

		class StateAssignImpl : public State {
		public:
			Object *obj;
			Variable *var;

			StateAssignImpl(State *_prev, Object *_obj, Variable *_var) : State(_prev), obj(_obj), var(_var) {
			}
			State *next() override;
			void trace() override;
			Token *token() override;
		};

	public:
		String name;
		Identifier *identifier;

		Variable(Identifier *tokName);
		Variable *toVariable() override;
		Identifier *toIdentifier() override;
		State * createState(State *_prev) override;
		State * createStateAssign(State *_prev, Object *_obj) override;
		void trace() override;
		String toString() override;
	};

}
