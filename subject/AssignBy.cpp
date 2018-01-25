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

#include "AssignBy.h"
#include "ObjectBuiltIn.h"
#include "ObjectFunction.h"
#include "ObjectException.h"
#include "Resource.h"
#include "Identifier.h"
#include "StringBuilder.h"

namespace goat {

	AssignBy::AssignBy(Operator *_oper, LeftExpression *_left, Expression *_right) {
		loc = _oper->loc;
		oper = _oper;
		left = _left;
		right = _right;
	}

	AssignBy * AssignBy::toAssignBy() {
		return this;
	}

	void AssignBy::trace() {
		oper->mark();
		left->mark();
		right->mark();
	}

	State *AssignBy::createState(State *_prev) {
		return new StateImpl(_prev, this);
	}

	State * AssignBy::StateImpl::next() {
		switch (step) {
		case GET_LEFT:
			return expr->left->createState(this);
		case GET_RIGHT:
			if (!left) {
				return throw_(new CanNotReadOperatorOfUndefined(expr->oper->value));
			}
			return expr->right->createState(this);
		case EXECUTE: {
			Object *obj = left->find(expr->oper->value);
			if (!obj) {
				return throw_(new OperatorIsNotDefined(expr->oper->value));
			}
			ObjectFunction *of = obj->toObjectFunction();
			if (of) {
				changeScope(of->context->clone());
				scope->this_ = left;
				scope->arguments = new ObjectArray();
				scope->arguments->vector.pushBack(right);
				scope->objects.insert(Resource::arguments, scope->arguments);
				scope->proto.pushBack(scope->proto[0]);
				scope->proto[0] = left;
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
				scope->this_ = left;
				scope->arguments = new ObjectArray();
				scope->arguments->vector.pushBack(right);
				return obi->createState(this);
			}
			return throw_(new IsNotAFunction(expr->oper->value));
		}
		case SET_LEFT:
			step = DONE;
			return expr->left->createStateAssign(this, result);
		case DONE:
			State * p = prev;
			delete this;
			return p;
		}
		throw NotImplemented();
	}

	void AssignBy::StateImpl::ret(Object *obj) {
		switch (step) {
		case GET_LEFT:
			left = obj;
			step = GET_RIGHT;
			return;
		case GET_RIGHT:
			right = obj;
			step = EXECUTE;
			return;
		case EXECUTE:
			prev->ret(obj);
			result = obj;
			step = SET_LEFT;
			return;
		case SET_LEFT:
		case DONE:
			return;
		default:
			throw NotImplemented();
		}
	}

	void AssignBy::StateImpl::trace() {
		if (left) {
			left->mark();
		}
		if (right) {
			right->mark();
		}
		if (result) {
			result->mark();
		}
	}

	String AssignBy::toString() {
		return (StringBuilder() << left->toString() << ' ' << oper->value << ' ' << right->toString()).toString();
	}

	Token * AssignBy::StateImpl::token() {
		return expr;
	}
}