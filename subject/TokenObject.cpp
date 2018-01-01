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

#include "TokenObject.h"
#include "Pair.h"
#include "Assert.h"

namespace goat {

	TokenObject::TokenObject(Brackets *tokItems) {
		loc = tokItems->loc;
		items = tokItems->tokens;
		tokItems->tokens = nullptr;
	}

	TokenObject::~TokenObject() {
		delete items;
	}

	void TokenObject::trace() {
		items->mark();
	}

	TokenObject * TokenObject::toTokenObject() {
		return this;
	}

	State * TokenObject::createState(State *prev) {
		return new StateImpl(prev, this);
	}

	TokenObject::StateImpl::StateImpl(State *_prev, TokenObject *_expr) : State(_prev), expr(_expr) {
		item = expr->items->first;
		objKey = nullptr;
		result = new Object();
		step = GET_LEFT;
	}

	State * TokenObject::StateImpl::next() {
		switch (step) {
		case GET_LEFT: {
			if (!item) {
				State *p = prev;
				p->ret(result);
				delete this;
				return p;
			}
			Pair *pair = item->toPair();
			assert(pair != nullptr);
			Identifier *ident = pair->left->toIdentifier();
			objKey = nullptr;
			if (ident) {
				strKey = ident->name;
				step = GET_RIGHT;
				State *st = pair->right->createState(this);
				item = item->next;
				return st;
			}
			else {
				return pair->left->createState(this);
			}
		}
		case GET_RIGHT: {
			State *st = item->toPair()->right->createState(this);
			item = item->next;
			return st;
		}
		default:
			throw NotImplemented();
		}
	}

	void TokenObject::StateImpl::ret(Object *obj) {
		switch (step)
		{
		case GET_LEFT:
			objKey = obj;
			step = GET_RIGHT;
			return;
		case GET_RIGHT:
			if (objKey) {
				result->insert(objKey, obj);
			}
			else {
				result->insert(strKey, obj);
			}
			step = GET_LEFT;
			return;
		default:
			throw NotImplemented();
		}
	}

	void TokenObject::StateImpl::trace() {
		if (objKey)
			objKey->mark();
		result->mark();
	}
}
