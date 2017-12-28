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

#include "ObjectChar.h"
#include "ObjectBoolean.h"
#include "ObjectInteger.h"
#include "ObjectException.h"
#include "WideStringBuilder.h"

namespace goat {

	ObjectChar::ObjectChar(wchar _value) {
		value = _value;
		proto[0] = Proto::getInstance();
	}

	ObjectChar * ObjectChar::toObjectChar() {
		return this;
	}

	WideString ObjectChar::toWideString() {
		return WideString(&value, 1);
	}

	WideString ObjectChar::toWideStringNotation() {
		return (WideStringBuilder() << L'\'' << value << L'\'').toWideString();
	}

	bool ObjectChar::equals(Object *_obj) {
		ObjectChar *obj = _obj->toObjectChar();
		return obj && obj->value == value;
	}

	ObjectChar::Proto::Proto() {
		status = PERMANENT;

		objects.insert("clone", Clone::getInstance());
		objects.insert("valueOf", ValueOf::getInstance());
		objects.insert("++", OperatorIncrement::getInstance());
		objects.insert("--", OperatorDecrement::getInstance());
		objects.insert("<", OperatorLess::getInstance());
		objects.insert("<=", OperatorLessEqual::getInstance());
		objects.insert(">", OperatorGreater::getInstance());
		objects.insert(">=", OperatorGreaterEqual::getInstance());
	}

	Object * ObjectChar::Proto::getInstance() {
		static Proto __this;
		return &__this;
	}


	Object * ObjectChar::Proto::OperatorLess::run(Scope *scope) {
		ObjectChar *this_ = scope->this_->toObjectChar();
		ObjectChar *operand = scope->arguments->vector[0]->toObjectChar();
		if (!operand) {
			return new IllegalArgument();
		}
		return new ObjectBoolean(this_->value < operand->value);
	}

	Object * ObjectChar::Proto::OperatorLess::getInstance() {
		static OperatorLess __this;
		return &__this;
	}


	Object * ObjectChar::Proto::OperatorLessEqual::run(Scope *scope) {
		ObjectChar *this_ = scope->this_->toObjectChar();
		ObjectChar *operand = scope->arguments->vector[0]->toObjectChar();
		if (!operand) {
			return new IllegalArgument();
		}
		return new ObjectBoolean(this_->value <= operand->value);
	}

	Object * ObjectChar::Proto::OperatorLessEqual::getInstance() {
		static OperatorLessEqual __this;
		return &__this;
	}


	Object * ObjectChar::Proto::OperatorGreater::run(Scope *scope) {
		ObjectChar *this_ = scope->this_->toObjectChar();
		ObjectChar *operand = scope->arguments->vector[0]->toObjectChar();
		if (!operand) {
			return new IllegalArgument();
		}
		return new ObjectBoolean(this_->value > operand->value);
	}

	Object * ObjectChar::Proto::OperatorGreater::getInstance() {
		static OperatorGreater __this;
		return &__this;
	}


	Object * ObjectChar::Proto::OperatorGreaterEqual::run(Scope *scope) {
		ObjectChar *this_ = scope->this_->toObjectChar();
		ObjectChar *operand = scope->arguments->vector[0]->toObjectChar();
		if (!operand) {
			return new IllegalArgument();
		}
		return new ObjectBoolean(this_->value >= operand->value);
	}

	Object * ObjectChar::Proto::OperatorGreaterEqual::getInstance() {
		static OperatorGreaterEqual __this;
		return &__this;
	}


	Object * ObjectChar::Proto::OperatorIncrement::run(Scope *scope) {
		ObjectChar *this_ = scope->this_->toObjectChar();
		return new ObjectChar(this_->value + 1);
	}

	Object * ObjectChar::Proto::OperatorIncrement::getInstance() {
		static OperatorIncrement __this;
		return &__this;
	}


	Object * ObjectChar::Proto::OperatorDecrement::run(Scope *scope) {
		ObjectChar *this_ = scope->this_->toObjectChar();
		return new ObjectChar(this_->value - 1);
	}

	Object * ObjectChar::Proto::OperatorDecrement::getInstance() {
		static OperatorDecrement __this;
		return &__this;
	}


	Object * ObjectChar::Proto::Clone::run(Scope *scope) {
		ObjectChar *this_ = scope->this_->toObjectChar();
		ObjectChar *obj = new ObjectChar(this_->value);
		this_->clone(obj);
		return obj;
	}

	Object * ObjectChar::Proto::Clone::getInstance() {
		static Clone __this;
		return &__this;
	}


	Object * ObjectChar::Proto::ValueOf::run(Scope *scope) {
		Object *arg = scope->arguments->vector[0];
		if (arg) {
			ObjectChar *argChar = arg->toObjectChar();
			if (argChar) {
				return new ObjectChar(argChar->value);
			}
			ObjectInteger *argInt = arg->toObjectInteger();
			if (argInt && argInt->value > 0 && argInt->value < 0x10FFFF) {
				return new ObjectChar((wchar)argInt->value);
			}
		}
		return new ObjectChar(L'\0');
	}

	Object * ObjectChar::Proto::ValueOf::getInstance() {
		static ValueOf __this;
		return &__this;
	}
}
