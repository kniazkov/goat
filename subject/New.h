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
#include "Keyword.h"
#include "FunctionCall.h"

namespace goat {

	class New : public Expression {
	protected:
		class StateImpl : public State {
		public:
			enum Step {
				GET_ARGUMENTS,
				CREATE_OBJECT,
				INIT_OBJECT,
				DONE
			};

			New *expr;
			Step step;
			Token *arg;
			Object *retObj;
			ObjectArray *arguments;
			PlainVector<Object* > chain;
			unsigned int index;

			StateImpl(State *_prev, New *_expr);
			State *next() override;
			void ret(Object *obj) override;
			void trace() override;
		};

	public:
		FunctionCall *fcall;

		New(Keyword *tokKeyword, FunctionCall *tokFcall);
		~New();
		void trace() override;
		New *toNew() override;
		State * createState(State *_prev) override;
		String toString() override;
	};

}
