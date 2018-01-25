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

#include "Statement.h"
#include "Expression.h"
#include "Keyword.h"

namespace goat {

	class Throw : public Statement {
	protected:
		class StateImpl : public State {
		public:
			Throw * stmt;
			Object *obj;
			bool executed;

			StateImpl(State *_prev, Throw *_stmt) : State(_prev), stmt(_stmt), obj(nullptr), executed(false) {
			}
			State * next() override;
			void ret(Object *obj) override;
			void trace() override;
			Location * location() override;
		};

	public:
		Expression * expr;

		Throw(Keyword *_kw, Expression *_expr);
		void trace() override;
		Throw *toThrow() override;
		State * createState(State *_prev) override;
		String toString() override;
	};

}
