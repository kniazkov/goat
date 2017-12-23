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

#include "LeftExpression.h"
#include "Operator.h"

namespace goat {

	class PrefixIncrement : public Expression {
	protected:
		class StateImpl : public State {
		public:
			enum Step {
				GET_RIGHT,
				EXECUTE,
				SET_RIGHT,
				DONE
			};

			PrefixIncrement *expr;
			Object *oldValue,
				*newValue;
			Step step;

			StateImpl(State *_prev, PrefixIncrement *_expr) :
				State(_prev), expr(_expr), oldValue(nullptr), newValue(nullptr), step(GET_RIGHT) {
			}
			State *next() override;
			void ret(Object *obj) override;
			void trace() override;
		};

	public:
		Operator * oper;
		LeftExpression *right;

		PrefixIncrement(Operator *_oper, LeftExpression *_right);
		PrefixIncrement *toPrefixIncrement() override;
		void trace() override;
		State * createState(State *_prev) override;
	};

}