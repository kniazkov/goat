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

#include "PrefixIncrement.h"
#include "ObjectBuiltIn.h"
#include "ObjectFunction.h"
#include "ObjectException.h"
#include "Resource.h"
#include "Identifier.h"

namespace goat {

	PrefixIncrement::PrefixIncrement(Operator *_oper, LeftExpression *_right) {
		loc = _oper->loc;
		oper = _oper;
		right = _right;
	}

	PrefixIncrement * PrefixIncrement::toPrefixIncrement() {
		return this;
	}

	void PrefixIncrement::trace() {
		oper->mark();
		right->mark();
	}

	State *PrefixIncrement::createState(State *_prev) {
		return new StateImpl(_prev, this);
	}

	State * PrefixIncrement::StateImpl::next() {
		switch (step) {
			case GET_RIGHT:
				return expr->right->createState(this);
			case EXECUTE: {
				if (!oldValue) {
					return throw_(new CanNotReadOperatorOfUndefined(expr->oper->value));
				}
				Object *obj = oldValue->find(expr->oper->value);
				if (!obj) {
					return throw_(new OperatorIsNotDefined(expr->oper->value));
				}
				ObjectFunction *of = obj->toObjectFunction();
				if (of) {
					changeScope(of->context->clone());
					scope->this_ = oldValue;
					scope->arguments = new ObjectArray();
					scope->objects.insert(Resource::arguments, scope->arguments);
					if (of->function->args) {
						unsigned int i = 0, count = scope->arguments->vector.len();
						Token *name = of->function->args->first;
						while (name && i < count) {
							scope->objects.insert(name->toIdentifier()->name, scope->arguments->vector[i]);
							i++;
							name = name->next;
						}
					}
					return of->function->createState(this);
				}
				ObjectBuiltIn *obi = obj->toObjectBuiltIn();
				if (obi) {
					cloneScope();
					scope->this_ = oldValue;
					return obi->createState(this);
				}
				return throw_(new IsNotAFunction(expr->oper->value));
			}
			case SET_RIGHT:
				step = DONE;
				return expr->right->createStateAssign(this, newValue);
			case DONE: {
					State * p = prev;
					delete this;
					return p;
				}
			default:
				throw NotImplemented();
		}
	}

	void PrefixIncrement::StateImpl::ret(Object *obj) {
		switch (step) {
		case GET_RIGHT:
			oldValue = obj;
			step = EXECUTE;
			return;
		case EXECUTE:
			newValue = obj;
			prev->ret(obj);
			step = SET_RIGHT;
			return;
		case DONE:
			return;
		default:
			throw NotImplemented();
		}
	}

	void PrefixIncrement::StateImpl::trace() {
		if (oldValue) {
			oldValue->mark();
		}
		if (newValue) {
			newValue->mark();
		}
	}
}