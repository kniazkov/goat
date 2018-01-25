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

namespace goat {

	class InlineIf : public Expression {
	protected:
		class StateImpl : public State {
		public:
			enum Step {
				CHECK_CONDITION,
				EXECUTE,
				DONE
			};

			InlineIf * expr;
			Object *condition;
			Step step;

			StateImpl(State *_prev, InlineIf *_expr) : State(_prev), expr(_expr), condition(nullptr), step(CHECK_CONDITION) {
			}
			State * next() override;
			void ret(Object *obj) override;
			void trace() override;
		};

	public:
		Expression * condition, *exprIf, *exprElse;

		InlineIf(Expression *_condition, Expression *_exprIf, Expression *_exprElse);
		void trace() override;
		InlineIf *toInlineIf() override;
		State * createState(State *_prev) override;
		String toString() override;
	};

}
