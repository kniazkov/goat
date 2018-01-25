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

#include "Expression.h"
#include "Operator.h"

namespace goat {

	class Prefix : public Expression {
	protected:
		class StateImpl : public State {
		public:
			enum Step {
				GET_RIGHT,
				EXECUTE,
				DONE
			};

			Prefix *expr;
			Object *right;
			Step step;

			StateImpl(State *_prev, Prefix *_expr) :
				State(_prev), expr(_expr), right(nullptr), step(GET_RIGHT) {
			}
			State *next() override;
			void ret(Object *obj) override;
			void trace() override;
			Token * token() override;
		};

	public:
		Operator * oper;
		Expression *right;

		Prefix(Operator *_oper, Expression *_right);
		Prefix *toPrefix() override;
		void trace() override;
		State * createState(State *_prev) override;
		String toString() override;
	};

}
