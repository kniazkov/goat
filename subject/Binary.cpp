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

#include "Binary.h"
#include "ObjectBuiltIn.h"
#include "ObjectFunction.h"
#include "ObjectException.h"
#include "Resource.h"
#include "Identifier.h"
#include "StringBuilder.h"

namespace goat {

	Binary::Binary(Operator *_oper, Expression *_left, Expression *_right) {
		loc = _oper->loc;
		oper = _oper;
		left = _left;
		right = _right;
	}

	Binary * Binary::toBinary() {
		return this;
	}

	void Binary::trace() {
		oper->mark();
		left->mark();
		right->mark();
	}

	State *Binary::createState(State *_prev) {
		return new StateImpl(_prev, this);
	}

	State * Binary::StateImpl::next() {
		switch (step) {
		case GET_LEFT:
			return expr->left->createState(this);
		case GET_RIGHT:
			if (!left) {
				return throw_(new CanNotReadOperatorOfUndefined(expr->oper->value));
			}
			return expr->right->createState(this);
		case EXECUTE: {
			step = DONE;
			Object *obj = left->find(expr->oper->value);
			if (obj->toObjectUndefined()) {
				return throw_(new OperatorIsNotDefined(expr->oper->value));
			}
			ObjectFunction *of = obj->toObjectFunction();
			if (of) {
				changeScope(of->context->clone());
				scope->this_ = left;
				scope->arguments = new ObjectArray();
				scope->arguments->vector.pushBack(right);
				scope->objects.insert(Object::createIndex(Resource::arguments), scope->arguments);
				scope->proto.pushBack(scope->proto[0]);
				scope->proto[0] = left;
				if (of->function->args) {
					unsigned int i = 0, count = scope->arguments->vector.len();
					Token *name = of->function->args->first;
					while (name && i < count) {
						scope->objects.insert(Object::createIndex(name->toIdentifier()->name), scope->arguments->vector[i]);
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
		case DONE:
			State *p = prev;
			delete this;
			return p;
		}
		throw NotImplemented();
	}

	void Binary::StateImpl::ret(Object *obj) {
		switch (step) {
		case GET_LEFT:
			left = obj;
			step = GET_RIGHT;
			return;
		case GET_RIGHT:
			right = obj;
			step = EXECUTE;
			return;
		case DONE:
			prev->ret(obj);
			return;
		default:
			throw NotImplemented();
		}
	}

	void Binary::StateImpl::trace() {
		if (left) {
			left->mark();
		}
		if (right) {
			right->mark();
		}
	}

	String Binary::toString() {
		return (StringBuilder() << left->toString() << ' ' << oper->value << ' ' << right->toString()).toString();
	}

	Token * Binary::StateImpl::token() {
		return expr;
	}
}