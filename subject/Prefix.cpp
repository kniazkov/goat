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

#include "Prefix.h"
#include "ObjectBuiltIn.h"
#include "ObjectFunction.h"
#include "ObjectException.h"
#include "Resource.h"
#include "Identifier.h"
#include "StringBuilder.h"

namespace goat {

	Prefix::Prefix(Operator *_oper, Expression *_right) {
		loc = _oper->loc;
		oper = _oper;
		right = _right;
	}

	Prefix * Prefix::toPrefix() {
		return this;
	}

	void Prefix::trace() {
		oper->mark();
		right->mark();
	}

	State *Prefix::createState(State *_prev) {
		return new StateImpl(_prev, this);
	}

	State * Prefix::StateImpl::next() {
		switch (step) {
		case GET_RIGHT:
			return expr->right->createState(this);
		case EXECUTE: {
			step = DONE;
			if (!right) {
				return throw_(new CanNotReadOperatorOfUndefined(expr->oper->value));
			}
			Object *obj = right->find(expr->oper->value);
			if (!obj) {
				return throw_(new OperatorIsNotDefined(expr->oper->value));
			}
			ObjectFunction *of = obj->toObjectFunction();
			if (of) {
				changeScope(of->context->clone());
				scope->this_ = right;
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
				scope->this_ = right;
				return obi->createState(this);
			}
			return throw_(new IsNotAFunction(expr->oper->value));
		}
		case DONE:
			State * p = prev;
			delete this;
			return p;
		}
		throw NotImplemented();
	}

	void Prefix::StateImpl::ret(Object *obj) {
		switch (step) {
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

	void Prefix::StateImpl::trace() {
		if (right) {
			right->mark();
		}
	}

	String Prefix::toString() {
		return (StringBuilder() << oper->value << right->toString()).toString();
	}
}