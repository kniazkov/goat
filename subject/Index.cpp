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

#include "Index.h"
#include "Assert.h"
#include "ObjectException.h"
#include "ObjectInteger.h"
#include "ObjectString.h"
#include "ObjectChar.h"
#include "ObjectByteArray.h"

namespace goat {

	Index::Index(Expression *tokLeft, Brackets *tokIndex) {
		loc = tokIndex->loc;
		left = tokLeft;
		tokens = tokIndex->tokens;
		tokIndex->tokens = nullptr;
	}

	Index * Index::toIndex() {
		return this;
	}

	State * Index::createState(State *prev) {
		return new StateImpl(prev, this);
	}

	State * Index::createStateAssign(State *prev, Object *obj) {
		return new StateAssignImpl(prev, obj, this);
	}

	void Index::trace() {
		left->mark();
		tokens->mark();
	}

	State * Index::StateImpl::next() {
		switch (step) {
		case GET_LEFT_OBJECT:
			return expr->left->createState(this);
		case GET_INDEX_OBJECT:
			return expr->tokens->first->toExpression()->createState(this);
		case DONE:
			if (left) {
				ObjectArray *objArr = left->toObjectArray();
				if (objArr) {
					ObjectInteger *intIdx = index ? index->toObjectInteger() : nullptr;
					if (!intIdx) {
						return throw_(new IncorrectIndex());
					}
					if (intIdx->value < 0 || intIdx->value >= objArr->vector.len()) {
						return throw_(new OutOfBounds());
					}
					Object *value = objArr->vector[(unsigned int)intIdx->value];
					State *p = prev;
					p->ret(value);
					delete this;
					return p;
				}
				ObjectByteArray *objByteArr = left->toObjectByteArray();
				if (objByteArr) {
					ObjectInteger *intIdx = index ? index->toObjectInteger() : nullptr;
					if (!intIdx) {
						return throw_(new IncorrectIndex());
					}
					if (intIdx->value < 0 || intIdx->value >= objByteArr->vector.len()) {
						return throw_(new OutOfBounds());
					}
					Object *value = new ObjectInteger(objByteArr->vector[(unsigned int)intIdx->value]);
					State *p = prev;
					p->ret(value);
					delete this;
					return p;
				}
				ObjectString *objStr = left->toObjectString();
				if (objStr) {
					ObjectInteger *intIdx = index ? index->toObjectInteger() : nullptr;
					if (!intIdx) {
						return throw_(new IncorrectIndex());
					}
					if (intIdx->value < 0 || intIdx->value >= objStr->value.len()) {
						return throw_(new OutOfBounds());
					}
					ObjectChar *value = new ObjectChar(objStr->value[(unsigned int)intIdx->value]);
					State *p = prev;
					p->ret(value);
					delete this;
					return p;
				}

				// search key in objects
				{
					State *p = prev;
					p->ret(left->find(index));
					delete this;
					return p;
				}
			}
			return throw_(new CanNotReadPropertyOfUndefined(index));
		}
		throw NotImplemented();
	}

	void Index::StateImpl::ret(Object *obj) {
		switch (step) {
		case GET_LEFT_OBJECT:
			left = obj;
			step = GET_INDEX_OBJECT;
			return;
		case GET_INDEX_OBJECT:
			index = obj;
			step = DONE;
			return;
		default:
			throw NotImplemented();
		}
	}

	void Index::StateImpl::trace() {
		if (left) {
			left->mark();
		}
		if (index) {
			index->mark();
		}
	}

	State * Index::StateAssignImpl::next() {
		switch (step) {
		case GET_LEFT_OBJECT:
			return expr->left->createState(this);
		case GET_INDEX_OBJECT:
			return expr->tokens->first->toExpression()->createState(this);
		case DONE:
			if (left) {
				ObjectArray *objArr = left->toObjectArray();
				if (objArr) {
					ObjectInteger *intIdx = index ? index->toObjectInteger() : nullptr;
					if (!intIdx) {
						return throw_(new IncorrectIndex());
					}
					if (intIdx->value < 0 || intIdx->value >= objArr->vector.len()) {
						return throw_(new OutOfBounds());
					}
					objArr->vector[(unsigned int)intIdx->value] = obj;
					State *p = prev;
					p->ret(obj);
					delete this;
					return p;
				}
				ObjectByteArray *objByteArr = left->toObjectByteArray();
				if (objByteArr) {
					ObjectInteger *intIdx = index ? index->toObjectInteger() : nullptr;
					if (!intIdx) {
						return throw_(new IncorrectIndex());
					}
					if (intIdx->value < 0 || intIdx->value >= objByteArr->vector.len()) {
						return throw_(new OutOfBounds());
					}
					ObjectInteger *intObj = obj ? obj->toObjectInteger() : nullptr;
					if (!intObj || intObj->value < 0 || intObj->value > 255) {
						return throw_(new IllegalArgument());
					}
					objByteArr->vector[(unsigned int)intIdx->value] = (unsigned char)intObj->value;
					State *p = prev;
					p->ret(obj);
					delete this;
					return p;
				}
				ObjectString *objStr = left->toObjectString();
				if (objStr) {
					// can not change strings
					return throw_(new IncorrectIndex());
				}
				{
					left->insert(index, obj);
					State *p = prev;
					p->ret(obj);
					delete this;
					return p;
				}
			}
			return throw_(new CanNotWritePropertyOfUndefined(index));
		}
		throw NotImplemented();
	}

	void Index::StateAssignImpl::ret(Object *obj) {
		switch (step) {
		case GET_LEFT_OBJECT:
			left = obj;
			step = GET_INDEX_OBJECT;
			return;
		case GET_INDEX_OBJECT:
			index = obj;
			step = DONE;
			return;
		default:
			throw NotImplemented();
		}
	}

	void Index::StateAssignImpl::trace() {
		if (left) {
			left->mark();
		}
		if (index) {
			index->mark();
		}
	}

}
