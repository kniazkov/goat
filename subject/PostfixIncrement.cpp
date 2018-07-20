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

#include "PostfixIncrement.h"
#include "ObjectBuiltIn.h"
#include "ObjectFunction.h"
#include "ObjectException.h"
#include "Resource.h"
#include "Identifier.h"
#include "StringBuilder.h"

namespace goat {

	PostfixIncrement::PostfixIncrement(Operator *_oper, LeftExpression *_left) {
		loc = _oper->loc;
		oper = _oper;
		left = _left;
	}

	PostfixIncrement * PostfixIncrement::toPostfixIncrement() {
		return this;
	}

	void PostfixIncrement::trace() {
		oper->mark();
		left->mark();
	}

	State *PostfixIncrement::createState(State *_prev) {
		return new StateImpl(_prev, this);
	}

	State * PostfixIncrement::StateImpl::next() {
		switch (step) {
		case GET_LEFT:
			return expr->left->createState(this);
		case EXECUTE: {
			if (!oldValue) {
				return throw_(new CanNotReadOperatorOfUndefined(expr->oper->value));
			}
			Object *obj = oldValue->find(expr->oper->value);
			if (obj->toObjectUndefined()) {
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
		case SET_LEFT:
			step = DONE;
			return expr->left->createStateAssign(this, newValue);
		case DONE: {
			State * p = prev;
			delete this;
			return p;
		}
		default:
			throw NotImplemented();
		}
	}

	void PostfixIncrement::StateImpl::ret(Object *obj) {
		switch (step) {
		case GET_LEFT:
			oldValue = obj;
			prev->ret(obj);
			step = EXECUTE;
			return;
		case EXECUTE:
			newValue = obj;
			step = SET_LEFT;
			return;
		case DONE:
			return;
		default:
			throw NotImplemented();
		}
	}

	void PostfixIncrement::StateImpl::trace() {
		if (oldValue) {
			oldValue->mark();
		}
		if (newValue) {
			newValue->mark();
		}
	}

	String PostfixIncrement::toString() {
		return (StringBuilder() << left->toString() << oper->value).toString();
	}

	Token * PostfixIncrement::StateImpl::token() {
		return expr;
	}
}
