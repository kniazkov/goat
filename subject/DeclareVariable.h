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
#include "Identifier.h"
#include "Expression.h"

namespace goat {

	class DeclareVariable : public Statement {
	protected:
		class StateImpl : public State {
		public:
			DeclareVariable *decl;
			Object *init;

			StateImpl(State *_prev, DeclareVariable *_decl) : State(_prev), decl(_decl), init(nullptr) {
			}
			State * next() override;
			void ret(Object *obj) override;
			void trace() override;
			Token * token() override;
		};

	public:
		String name;
		Expression *init;

		DeclareVariable(Identifier *_tokName, Expression *_initExpr = nullptr);
		void trace() override;
		DeclareVariable *toDeclareVariable() override;
		State * createState(State *_prev) override;
		String toString() override;
	};

}
