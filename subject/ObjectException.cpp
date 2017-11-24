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

#include "ObjectException.h"
#include "WideStringBuilder.h"

namespace goat {

	ObjectException::ObjectException() {
		proto.pushBack(Proto::getInstance());
	}

	ObjectException * ObjectException::toObjectException() {
		return this;
	}

	ObjectException::Proto::Proto() {
		status = PERMANENT;
		initialized = false;
	}

	void ObjectException::Proto::init() {
		if (initialized) {
			return;
		}

		initialized = true;
		objects.insert("NameIsNotDefined", NameIsNotDefined::Proto::getInstance());
		objects.insert("OperatorIsNotDefined", OperatorIsNotDefined::Proto::getInstance());
		objects.insert("IsNotAFunction", IsNotAFunction::Proto::getInstance());
		objects.insert("CanNotCallUndefined", CanNotCallUndefined::Proto::getInstance());
		objects.insert("CanNotCallExpression", CanNotCallExpression::Proto::getInstance());
		objects.insert("CanNotReadPropertyOfUndefined", CanNotReadPropertyOfUndefined::Proto::getInstance());
		objects.insert("CanNotWritePropertyOfUndefined", CanNotWritePropertyOfUndefined::Proto::getInstance());
		objects.insert("CanNotReadOperatorOfUndefined", CanNotReadOperatorOfUndefined::Proto::getInstance());
		objects.insert("DivisionByZero", DivisionByZero::Proto::getInstance());
		objects.insert("IncorrectIndex", IncorrectIndex::Proto::getInstance());
		objects.insert("OutOfBounds", OutOfBounds::Proto::getInstance());
	}

	Object * ObjectException::Proto::getInstance() {
		static Proto __this;
		__this.init();
		return &__this;
	}

	NameIsNotDefined::NameIsNotDefined(String _name) {
		name = _name;
		proto.pushBack(Proto::getInstance());
	}

	WideString NameIsNotDefined::toWideString() {
		return (WideStringBuilder() << "name \'" << name << "\' is not defined").toWideString();
	}

	NameIsNotDefined::Proto::Proto() {
		status = PERMANENT;
		proto.pushBack(ObjectException::Proto::getInstance());
	}

	Object * NameIsNotDefined::Proto::getInstance() {
		static Proto __this;
		return &__this;
	}

	IsNotAFunction::IsNotAFunction(String _name) {
		name = _name;
		proto.pushBack(Proto::getInstance());
	}

	WideString IsNotAFunction::toWideString() {
		return (WideStringBuilder() << "\'" << name << "\' is not a function").toWideString();
	}

	IsNotAFunction::Proto::Proto() {
		status = PERMANENT;
		proto.pushBack(ObjectException::Proto::getInstance());
	}

	Object * IsNotAFunction::Proto::getInstance() {
		static Proto __this;
		return &__this;
	}

	OperatorIsNotDefined::OperatorIsNotDefined(String _name) {
		name = _name;
		proto.pushBack(Proto::getInstance());
	}

	WideString OperatorIsNotDefined::toWideString() {
		return (WideStringBuilder() << "operator \'" << name << "\' is not defined").toWideString();
	}

	OperatorIsNotDefined::Proto::Proto() {
		status = PERMANENT;
		proto.pushBack(ObjectException::Proto::getInstance());
	}

	Object * OperatorIsNotDefined::Proto::getInstance() {
		static Proto __this;
		return &__this;
	}

	CanNotCallUndefined::CanNotCallUndefined() {
		proto.pushBack(Proto::getInstance());
	}

	WideString CanNotCallUndefined::toWideString() {
		return L"can not call undefined function";
	}

	CanNotCallUndefined::Proto::Proto() {
		status = PERMANENT;
		proto.pushBack(ObjectException::Proto::getInstance());
	}

	Object * CanNotCallUndefined::Proto::getInstance() {
		static Proto __this;
		return &__this;
	}

	CanNotCallExpression::CanNotCallExpression() {
		proto.pushBack(Proto::getInstance());
	}

	WideString CanNotCallExpression::toWideString() {
		return L"can not call, expression is not a function";
	}

	CanNotCallExpression::Proto::Proto() {
		status = PERMANENT;
		proto.pushBack(ObjectException::Proto::getInstance());
	}

	Object * CanNotCallExpression::Proto::getInstance() {
		static Proto __this;
		return &__this;
	}

	CanNotReadPropertyOfUndefined::CanNotReadPropertyOfUndefined(Object *_obj) {
		prop = _obj->toWideString();
		proto.pushBack(Proto::getInstance());
	}

	CanNotReadPropertyOfUndefined::CanNotReadPropertyOfUndefined(String _name) {
		prop = _name.toWideString();
		proto.pushBack(Proto::getInstance());
	}

	CanNotReadPropertyOfUndefined::Proto::Proto() {
		status = PERMANENT;
		proto.pushBack(ObjectException::Proto::getInstance());
	}

	Object * CanNotReadPropertyOfUndefined::Proto::getInstance() {
		static Proto __this;
		return &__this;
	}

	WideString CanNotReadPropertyOfUndefined::toWideString() {
		return (WideStringBuilder() << "cannot read propery \'" << prop << "\' of undefined").toWideString();
	}

	CanNotWritePropertyOfUndefined::CanNotWritePropertyOfUndefined(Object *_obj) {
		obj = _obj;
		proto.pushBack(Proto::getInstance());
	}

	CanNotWritePropertyOfUndefined::CanNotWritePropertyOfUndefined(String _name) {
		name = _name;
		obj = nullptr;
		proto.pushBack(Proto::getInstance());
	}

	WideString CanNotWritePropertyOfUndefined::toWideString() {
		if (obj)
			return (WideStringBuilder() << "cannot write propery \'" << obj->toWideString() << "\' of undefined").toWideString();
		else
			return (WideStringBuilder() << "cannot write propery \'" << name << "\' of undefined").toWideString();
	}

	CanNotWritePropertyOfUndefined::Proto::Proto() {
		status = PERMANENT;
		proto.pushBack(ObjectException::Proto::getInstance());
	}

	Object * CanNotWritePropertyOfUndefined::Proto::getInstance() {
		static Proto __this;
		return &__this;
	}

	CanNotReadOperatorOfUndefined::CanNotReadOperatorOfUndefined(String _name) {
		name = _name;
		proto.pushBack(Proto::getInstance());
	}

	WideString CanNotReadOperatorOfUndefined::toWideString() {
		return (WideStringBuilder() << "cannot read operator \'" << name << "\' of undefined").toWideString();
	}

	CanNotReadOperatorOfUndefined::Proto::Proto() {
		status = PERMANENT;
		proto.pushBack(CanNotReadPropertyOfUndefined::Proto::getInstance());
	}

	Object * CanNotReadOperatorOfUndefined::Proto::getInstance() {
		static Proto __this;
		return &__this;
	}

	DivisionByZero::DivisionByZero() {
		proto.pushBack(Proto::getInstance());
	}

	WideString DivisionByZero::toWideString() {
		return L"divizion by zero";
	}

	DivisionByZero::Proto::Proto() {
		status = PERMANENT;
		proto.pushBack(ObjectException::Proto::getInstance());
	}

	Object * DivisionByZero::Proto::getInstance() {
		static Proto __this;
		return &__this;
	}

	IncorrectIndex::IncorrectIndex() {
		proto.pushBack(Proto::getInstance());
	}

	WideString IncorrectIndex::toWideString() {
		return L"incorrect index, should be integer";
	}

	IncorrectIndex::Proto::Proto() {
		status = PERMANENT;
		proto.pushBack(ObjectException::Proto::getInstance());
	}

	Object * IncorrectIndex::Proto::getInstance() {
		static Proto __this;
		return &__this;
	}

	OutOfBounds::OutOfBounds() {
		proto.pushBack(Proto::getInstance());
	}

	WideString OutOfBounds::toWideString() {
		return L"index out of bounds";
	}

	OutOfBounds::Proto::Proto() {
		status = PERMANENT;
		proto.pushBack(ObjectException::Proto::getInstance());
	}

	Object * OutOfBounds::Proto::getInstance() {
		static Proto __this;
		return &__this;
	}
}
