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

#include "New.h"
#include "Assert.h"
#include "ObjectException.h"
#include "ObjectInteger.h"
#include "ObjectFunction.h"
#include "ObjectStringBuilder.h"
#include "ObjectByteArray.h"
#include "Resource.h"
#include "StringBuilder.h"

namespace goat {

	New::New(Keyword *tokKeyword, FunctionCall *tokFcall) {
		loc = tokKeyword->loc;
		fcall = tokFcall;
	}

	New::~New() {
	}

	void New::trace() {
		fcall->mark();
	}

	New * New::toNew() {
		return this;
	}

	State * New::createState(State *prev) {
		return new StateImpl(prev, this);
	}

	New::StateImpl::StateImpl(State *_prev, New *_expr) : State(_prev), expr(_expr) {
		retObj = nullptr;
		arguments = new ObjectArray();
		if (expr->fcall->args->count > 0) {
			step = GET_ARGUMENTS;
			arg = expr->fcall->args->first;
		}
		else {
			step = CREATE_OBJECT;
			arg = nullptr;
		}
		index = 0;
	}

	State * New::StateImpl::next() {
		switch (step) {
		case GET_ARGUMENTS:
			if (arg) {
				State *st = arg->toExpression()->createState(this);
				arg = arg->next;
				return st;
			}
			step = CREATE_OBJECT;
		case CREATE_OBJECT:
			return expr->fcall->func->createState(this);
		case INIT_OBJECT:
			if (!retObj)
				return throw_(new PrototypeIsNotDefined());
			if (index < chain.len()) {
				Object *init = chain[index];
				ObjectFunction *objf = init->toObjectFunction();
				if (objf) {
					changeScope(objf->context->clone());
					if (index == chain.len() - 1) {
						scope->arguments = arguments;
						unsigned int i = 0, count = scope->arguments->vector.len();
						Token *name = objf->function->args->first;
						while (name && i < count) {
							scope->objects.insert(Object::createIndex(name->toIdentifier()->name), scope->arguments->vector[i]);
							i++;
							name = name->next;
						}
					}
					else {
						scope->arguments = new ObjectArray();
					}
					scope->objects.insert(Object::createIndex(Resource::arguments), scope->arguments);
					scope->this_ = retObj;
					scope->proto.pushBack(scope->proto[0]);
					scope->proto[0] = retObj;
					index++;
					return objf->function->createState(this);
				}
				return throw_(new IsNotAFunction(Resource::init));
			}
			else {
				step = DONE;
			}
		case DONE:
			State *p = prev;
			p->ret(retObj);
			delete this;
			return p;
		}
		throw NotImplemented();
	}

	void New::StateImpl::ret(Object *obj) {
		switch (step) {
			case CREATE_OBJECT: {
				step = INIT_OBJECT;
				Object *proto = obj;
				if (proto->toObjectUndefined()) {
					return;
				}
				if (proto == ObjectInteger::Proto::getInstance()) {
					retObj = new ObjectInteger(0);
				}
				else if (proto == ObjectStringBuilder::Proto::getInstance()) {
					retObj = new ObjectStringBuilder();
				}
				else if (proto == ObjectByteArray::Proto::getInstance()) {
					retObj = new ObjectByteArray();
				}
				else {
					retObj = new Object();
					retObj->proto[0] = proto;
				}
				proto->findUnique(Resource::init, &chain);
				return;
			}
			case GET_ARGUMENTS:
				arguments->vector.pushBack(obj);
				return;
			case DONE:
				// ignore return object from constructor
				return;
			default:
				throw NotImplemented();
		}
	}

	void New::StateImpl::trace() {
		arguments->mark();
		if (retObj) {
			retObj->mark();
		}
	}

	String New::toString() {
		return (StringBuilder() << "new " << fcall->toString()).toString();
	}

	Token * New::StateImpl::token() {
		return expr;
	}
}
