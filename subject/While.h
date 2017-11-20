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

#include "Statement.h"
#include "Expression.h"

namespace goat {

	class While : public Statement {
	protected:
		class StateImpl : public State {
		public:
			enum Step {
				CHECK_CONDITION,
				EXECUTE
			};

			While *stmt;
			Object *condition;
			Step step;

			StateImpl(State *_prev, While *_stmt) : State(_prev), stmt(_stmt), condition(nullptr), step(CHECK_CONDITION) {
			}
			State * next() override;
			State * execute() override;
			void ret(Object *obj) override;
			void trace() override;
		};

	public:
		Expression *expr;
		Statement *stmt;

		While(Expression *_expr, Statement *_stmt);
		void trace() override;
		While *toWhile() override;
		State * createState(State *_prev) override;
	};

}
