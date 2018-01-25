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
#include "Brackets.h"

namespace goat {

	class Index : public LeftExpression {
	protected:
		class StateImpl : public State {
		public:
			enum Step {
				GET_LEFT_OBJECT,
				GET_INDEX_OBJECT,
				DONE
			};

			Index *expr;
			Step step;
			Object *left;
			Object *index;

			StateImpl(State *_prev, Index *_expr)
				: State(_prev), expr(_expr), step(GET_LEFT_OBJECT), left(nullptr), index(nullptr) {
			}
			State *next() override;
			void ret(Object *obj) override;
			void trace() override;
			Token * token() override;
		};

		class StateAssignImpl : public State {
		public:
			enum Step {
				GET_LEFT_OBJECT,
				GET_INDEX_OBJECT,
				DONE
			};

			Object *obj;
			Index *expr;
			Step step;
			Object *left;
			Object *index;

			StateAssignImpl(State *_prev, Object *_obj, Index *_expr)
				: State(_prev), obj(_obj), expr(_expr), step(GET_LEFT_OBJECT), left(nullptr), index(nullptr) {
			}
			State *next() override;
			void ret(Object *obj) override;
			void trace() override;
			Token * token() override;
		};

	public:
		Expression *left;
		TokenList *tokens;

		Index(Expression *tokLeft, Brackets *tokIndex);
		Index *toIndex() override;
		State * createState(State *_prev) override;
		State * createStateAssign(State *_prev, Object *_obj) override;
		void trace() override;
		String toString() override;
	};

}
