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

#include "InlineIf.h"
#include "ObjectBoolean.h"

namespace goat {

	InlineIf::InlineIf(Expression *_condition, Expression *_exprIf, Expression *_exprElse) {
		loc = _condition->loc;
		condition = _condition;
		exprIf = _exprIf;
		exprElse = _exprElse;
	}

	void InlineIf::trace() {
		condition->mark();
		exprIf->mark();
		exprElse->mark();
	}

	InlineIf * InlineIf::toInlineIf() {
		return this;
	}

	State * InlineIf::createState(State *_prev) {
		return new StateImpl(_prev, this);
	}

	State * InlineIf::StateImpl::next() {
		switch (step) {
			case CHECK_CONDITION:
				return expr->condition->createState(this);
			case EXECUTE:
				if (condition->toObjectBoolean()->value) {
					return expr->exprIf->createState(this);
				}
				else {
					return expr->exprElse->createState(this);
				}
			case DONE: {
				State * p = prev;
				delete this;
				return p;
			}
			default:
				throw NotImplemented();
		}
	}

	void InlineIf::StateImpl::ret(Object *obj) {
		switch (step)
		{
			case CHECK_CONDITION:
				condition = obj;
				step = EXECUTE;
				return;
			case EXECUTE:
				prev->ret(obj);
				step = DONE;
				return;
			default:
				throw NotImplemented();;
		}
	}

	void InlineIf::StateImpl::trace() {
		if (condition) {
			condition->mark();
		}
	}
}
