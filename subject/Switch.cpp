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

#include "Switch.h"
#include "ObjectBoolean.h"

namespace goat {

	Switch::Switch(Keyword *_kw, Expression *_expr, Brackets *_body) {
		loc = _kw->loc;
		expr = _expr;
		blocks = _body->tokens;
		_body->tokens = nullptr;
		if (blocks->count > 0) {
			def = blocks->last->toDefault();
			if (def) {
				blocks->remove(def);
			}
		}
		else
			def = nullptr;
	}

	void Switch::trace() {
		expr->mark();
		blocks->mark();
		if (def) {
			def->mark();
		}
	}

	Switch * Switch::toSwitch() {
		return this;
	}

	State * Switch::createState(State *_prev) {
		return new StateImpl(_prev, this);
	}

	Switch::StateImpl::StateImpl(State *_prev, Switch *_stmt) : State(_prev), stmt(_stmt) {
		objSwitch = nullptr;
		block = stmt->blocks->first->toCase();
		step = GET_OBJECT;
		tok = nullptr;
	}

	State * Switch::StateImpl::next() {
		switch (step) {
			case GET_OBJECT:
				return stmt->expr->createState(this);
			case COMPARE_OBJECT:
				if (!block) {
					if (stmt->def) {
						tok = stmt->def->tokens->first;
					}
					step = EXECUTE;
				}
				else {
					return block->condition->createState(this);
				}
			case EXECUTE:
				if (tok) {
					State *st = tok->toStatement()->createState(this);
					tok = tok->next;
					return st;
				}
				else {
					State *p = prev;
					delete this;
					return p;
				}
			default:
				throw NotImplemented();
		}
	}

	void Switch::StateImpl::ret(Object *obj) {
		switch (step) {
			case GET_OBJECT:
				objSwitch = obj;
				step = COMPARE_OBJECT;
				return;
			case COMPARE_OBJECT:
				if (Object::equals(objSwitch, obj)) {
					tok = block->tokens->first;
					step = EXECUTE;
				}
				else {
					block = block->next->toCase();
				}
				return;
			case EXECUTE:
				return;
			default:
				throw NotImplemented();
		}
	}

	void Switch::StateImpl::trace() {
		if (objSwitch) {
			objSwitch->mark();
		}
	}
}
