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

#include "TokenArray.h"
#include "Assert.h"
#include "StringBuilder.h"

namespace goat {

	TokenArray::TokenArray(Brackets *tokItems) {
		loc = tokItems->loc;
		items = tokItems->tokens;
		tokItems->tokens = nullptr;
	}

	TokenArray::~TokenArray() {
		delete items;
	}

	void TokenArray::trace() {
		items->mark();
	}

	TokenArray * TokenArray::toTokenArray() {
		return this;
	}

	State * TokenArray::createState(State *prev) {
		return new StateImpl(prev, this);
	}

	TokenArray::StateImpl::StateImpl(State *_prev, TokenArray *_expr) : State(_prev), expr(_expr) {
		item = expr->items->first;
		result = new ObjectArray();
	}

	State * TokenArray::StateImpl::next() {
		if (item) {
			Expression *expr = item->toExpression();
			assert(expr != nullptr);
			State *st = expr->createState(this);
			item = item->next;
			return st;
		}
		else {
			State *p = prev;
			p->ret(result);
			delete this;
			return p;
		}
	}

	void TokenArray::StateImpl::ret(Object *obj) {
		result->vector.pushBack(obj);
	}

	void TokenArray::StateImpl::trace() {
		result->mark();
	}

	String TokenArray::toString() {
		StringBuilder b;
		b << '[';
		int k;
		items->forEach([&](Token *tok) {
			if (k) {
				b << ", ";
			}
			k++;
			b << tok->toString();
		});
		b << " ]";
		return b.toString();
	}
}
