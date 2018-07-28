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

#include "FunctionCall.h"
#include "Assert.h"
#include "ObjectFunction.h"
#include "ObjectBuiltIn.h"
#include "ObjectException.h"
#include "ObjectUndefined.h"
#include "Variable.h"
#include "Resource.h"
#include "Field.h"
#include "StringBuilder.h"

namespace goat {

	FunctionCall::FunctionCall(Expression *tokFunc, Brackets *tokArgs) {
		assert(tokArgs->symbol == '(');

		loc = tokFunc->loc;
		func = tokFunc;
		args = tokArgs->tokens;
		tokArgs->tokens = nullptr;
		method = false;
		guard = false;
	}

	FunctionCall * FunctionCall::toFunctionCall() {
		return this;
	}

	FunctionCall::~FunctionCall() {
		delete args;
	}

	void FunctionCall::trace() {
		func->mark();
		args->mark();
	}

	String FunctionCall::toString() {
		StringBuilder b;
		b << func->toString() << '(';
		int k = 0;
		args->forEach([&](Token *tok) {
			if (k) {
				b << ", ";
			}
			k++;
			b << tok->toString();
		});
		b << ')';
		return b.toString();
	}

	State * FunctionCall::createState(State *_prev) {
		return new StateImpl(_prev, this);
	}

	FunctionCall::StateImpl::StateImpl(State *_prev, FunctionCall *_fcall) : State(_prev), fcall(_fcall) {
		arg = fcall->args->first;
		funcObj = nullptr;
		step = GET_FUNC_OBJECT;
		retObj = nullptr;
		thisObj = nullptr;
		arguments = new ObjectArray();
		level++;
	}

	State * FunctionCall::StateImpl::next() {
		switch (step) {
		case GET_FUNC_OBJECT:
			if (fcall->method) {
				return fcall->func->toField()->createState(this, &thisObj);
			}
			else {
				return fcall->func->createState(this);
			}
		case GET_ARGUMENTS: {
			if (arg) {
				State *st = arg->toExpression()->createState(this);
				arg = arg->next;
				return st;
			}
			step = DONE;
			if (funcObj->toObjectUndefined()) {
				if (fcall->guard) {
					State *p = prev;
					p->ret(ObjectUndefined::getInstance());
					delete this;
					return p;
				}
				Identifier *i = fcall->func->toIdentifier();
				if (i) {
					return throw_(new NameIsNotDefined(i->name));
				}
				else {
					return throw_(new CanNotCallUndefined());
				}
			}
			ObjectFunction *of = funcObj->toObjectFunction();
			if (of) {
				changeScope(of->context->clone());
				scope->arguments = arguments;
				scope->this_ = thisObj;
				scope->objects.insert(Resource::i_arguments(), scope->arguments);
				if (fcall->method) {
					scope->proto.pushBack(scope->proto[0]);
					scope->proto[0] = thisObj;
				}
				if (of->function->args) {
					unsigned int i = 0, count = scope->arguments->vector.len();
					Token *name = of->function->args->first;
					while (name) {
						scope->objects.insert(Object::createIndex(name->toIdentifier()->name), i < count ? scope->arguments->vector[i] : nullptr);
						i++;
						name = name->next;
					}
				}
				return of->function->createState(this);
			}
			ObjectBuiltIn * obi = funcObj->toObjectBuiltIn();
			if (obi) {
				cloneScope();
				scope->arguments = arguments;
				scope->this_ = thisObj;
				return obi->createState(this);
			}
			Identifier *i = fcall->func->toIdentifier();
			if (i) {
				return throw_(new IsNotAFunction(i->name));
			}
			else {
				return throw_(new CanNotCallExpression());
			}
		}
		case DONE: {
			State *p = prev;
			p->ret(retObj);
			delete this;
			return p;
		}
		default:
			throw NotImplemented();
		}
	}

	void FunctionCall::StateImpl::ret(Object *obj) {
		switch (step) {
		case GET_FUNC_OBJECT:
			funcObj = obj;
			step = GET_ARGUMENTS;
			return;
		case GET_ARGUMENTS:
			arguments->vector.pushBack(obj);
			return;
		case DONE:
			retObj = obj;
			return;
		default:
			throw NotImplemented();
		}
	}

	void FunctionCall::StateImpl::trace() {
		if (funcObj) {
			funcObj->mark();
		}
		if (retObj) {
			retObj->mark();
		}
		if (thisObj) {
			thisObj->mark();
		}
		arguments->mark();
		prev->scope->mark();
	}

	Token * FunctionCall::StateImpl::token() {
		return fcall;
	}

	State::DebugMode FunctionCall::StateImpl::stop() {
		return step == GET_FUNC_OBJECT ? INTO : SKIP;
	}
}
