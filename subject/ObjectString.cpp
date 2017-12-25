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

#include "ObjectString.h"
#include "ObjectInteger.h"
#include "ObjectBoolean.h"
#include "ObjectException.h"
#include "WideStringBuilder.h"
#include "Resource.h"

namespace goat {

	ObjectString::ObjectString(WideString _value) {
		value = _value;
		proto[0] = Proto::getInstance();
	}

	ObjectString * ObjectString::toObjectString() {
		return this;
	}

	WideString ObjectString::toWideString() {
		return value;
	}

	WideString ObjectString::toWideStringNotation() {
		WideStringBuilder b;
		b << (wchar)'\"';
		for (unsigned int i = 0, l = value.len(); i < l; i++) {
			wchar c = value[i];
			switch (c) {
				case '\r':
					b << "\r"; break;
				case '\n':
					b << "\n"; break;
				case '\t':
					b << "\t"; break;
				case '\"':
					b << "\\\""; break;
				case '\\':
					b << "\\\\"; break;
				default:
					b << c;
			}
		}
		b << (wchar)'\"';
		return b.toWideString();
	}

	bool ObjectString::equals(Object *_obj) {
		ObjectString *obj = _obj->toObjectString();
		return obj && obj->value == value;
	}

	ObjectString::Proto::Proto() {
		status = PERMANENT;

		objects.insert("+", OperatorPlus::getInstance());
		objects.insert("<", OperatorLess::getInstance());
		objects.insert("<=", OperatorLessEqual::getInstance());
		objects.insert("length", Length::getInstance());
		objects.insert("clone", Clone::getInstance());
		objects.insert("valueOf", ValueOf::getInstance());
	}

	Object * ObjectString::Proto::getInstance() {
		static Proto __this;
		return &__this;
	}


	Object * ObjectString::Proto::OperatorPlus::run(Scope *scope) {
		ObjectString *this_ = scope->this_->toObjectString();
		Object *arg = scope->arguments->vector[0];
		if (arg) {
			return new ObjectString(this_->value + arg->toWideString());
		}
		// exception
		return nullptr;
	}

	Object * ObjectString::Proto::OperatorPlus::getInstance() {
		static OperatorPlus __this;
		return &__this;
	}


	Object * ObjectString::Proto::OperatorLess::run(Scope *scope) {
		ObjectString *this_ = scope->this_->toObjectString();
		ObjectString *operand = scope->arguments->vector[0]->toObjectString();
		if (!operand) {
			return new IllegalArgument();
		}
		return new ObjectBoolean(this_->value < operand->value);
	}

	Object * ObjectString::Proto::OperatorLess::getInstance() {
		static OperatorLess __this;
		return &__this;
	}


	Object * ObjectString::Proto::OperatorLessEqual::run(Scope *scope) {
		ObjectString *this_ = scope->this_->toObjectString();
		ObjectString *operand = scope->arguments->vector[0]->toObjectString();
		if (!operand) {
			return new IllegalArgument();
		}
		return new ObjectBoolean(this_->value <= operand->value);
	}

	Object * ObjectString::Proto::OperatorLessEqual::getInstance() {
		static OperatorLessEqual __this;
		return &__this;
	}


	Object * ObjectString::Proto::Length::run(Scope *scope) {
		ObjectString *this_ = scope->this_->toObjectString();
		return new ObjectInteger(this_->value.len());
	}

	Object * ObjectString::Proto::Length::getInstance() {
		static Length __this;
		return &__this;
	}


	Object * ObjectString::Proto::Clone::run(Scope *scope) {
		ObjectString *this_ = scope->this_->toObjectString();
		ObjectString *obj = new ObjectString(this_->value);
		this_->clone(obj);
		return obj;
	}

	Object * ObjectString::Proto::Clone::getInstance() {
		static Clone __this;
		return &__this;
	}


	Object * ObjectString::Proto::ValueOf::run(Scope *scope) {
		Object *arg = scope->arguments->vector[0];
		if (!arg) {
			return new ObjectString(Resource::w_undefined);
		}
		return new ObjectString(arg->toWideString());
	}

	Object * ObjectString::Proto::ValueOf::getInstance() {
		static ValueOf __this;
		return &__this;
	}
}
