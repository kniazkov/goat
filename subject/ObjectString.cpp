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

#include "ObjectString.h"
#include "ObjectInteger.h"
#include "ObjectBoolean.h"
#include "ObjectChar.h"
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

//	ObjectBoolean * ObjectString::toObjectBoolean() {
//		return new ObjectBoolean(value.len() > 0);
//	}

	WideString ObjectString::toWideString(Set<Object*> &set) {
		return value;
	}

	WideString ObjectString::toWideStringNotation(Set<Object*> &set) {
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
		status = PERMANENT | LOCKED;

		objects.insert(createIndex("+"), OperatorPlus::getInstance());
		objects.insert(createIndex("+="), OperatorPlus::getInstance());
		objects.insert(createIndex("<"), OperatorLess::getInstance());
		objects.insert(createIndex("<="), OperatorLessEqual::getInstance());
		objects.insert(createIndex(">"), OperatorGreater::getInstance());
		objects.insert(createIndex(">="), OperatorGreaterEqual::getInstance());
		objects.insert(createIndex("!"), OperatorNot::getInstance());
		objects.insert(createIndex("length"), Length::getInstance());
		objects.insert(createIndex("clone"), Clone::getInstance());
		objects.insert(createIndex("valueOf"), ValueOf::getInstance());
		objects.insert(createIndex("subString"), SubString::getInstance());
		objects.insert(createIndex("split"), Split::getInstance());
	}

	Object * ObjectString::Proto::getInstance() {
		static Proto __this;
		return &__this;
	}


	Object * ObjectString::Proto::OperatorPlus::run(Scope *scope) {
		ObjectString *this_ = scope->this_->toObjectString();
		Object *arg = scope->arguments->vector[0];
		if (arg) {
			Set<Object*> set;
			return new ObjectString(this_->value + arg->toWideString(set));
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


	Object * ObjectString::Proto::OperatorGreater::run(Scope *scope) {
		ObjectString *this_ = scope->this_->toObjectString();
		ObjectString *operand = scope->arguments->vector[0]->toObjectString();
		if (!operand) {
			return new IllegalArgument();
		}
		return new ObjectBoolean(this_->value > operand->value);
	}

	Object * ObjectString::Proto::OperatorGreater::getInstance() {
		static OperatorGreater __this;
		return &__this;
	}


	Object * ObjectString::Proto::OperatorGreaterEqual::run(Scope *scope) {
		ObjectString *this_ = scope->this_->toObjectString();
		ObjectString *operand = scope->arguments->vector[0]->toObjectString();
		if (!operand) {
			return new IllegalArgument();
		}
		return new ObjectBoolean(this_->value >= operand->value);
	}

	Object * ObjectString::Proto::OperatorGreaterEqual::getInstance() {
		static OperatorGreaterEqual __this;
		return &__this;
	}


	Object * ObjectString::Proto::OperatorNot::run(Scope *scope) {
		ObjectString *this_ = scope->this_->toObjectString();
		return new ObjectBoolean(this_->value.len() == 0);
	}

	Object * ObjectString::Proto::OperatorNot::getInstance() {
		static OperatorNot __this;
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
		Set<Object*> set;
		return new ObjectString(arg->toWideString(set));
	}

	Object * ObjectString::Proto::ValueOf::getInstance() {
		static ValueOf __this;
		return &__this;
	}


	Object * ObjectString::Proto::SubString::run(Scope *scope) {
		ObjectString *this_ = scope->this_->toObjectString();
		if (scope->arguments->vector.len() < 1) {
			return new IllegalArgument();
		}
		ObjectInteger *start = scope->arguments->vector[0]->toObjectInteger();
		if (!start || start->value < 0 || start->value > uint32max) {
			return new IllegalArgument();
		}
		ObjectInteger *count = nullptr;
		if (scope->arguments->vector.len() > 1) {
			count = scope->arguments->vector[1]->toObjectInteger();
		}
		uint32 start_ = (uint32)start->value,
			count_;
		if (count) {
			if (count->value < 0 || count->value > uint32max) {
				return new IllegalArgument();
			}
			count_ = (uint32)count->value;
		}
		else {
			count_ = this_->value.len() - start_;
		}
		return new ObjectString(this_->value.subString(start_, count_));
	}

	Object * ObjectString::Proto::SubString::getInstance() {
		static SubString __this;
		return &__this;
	}


	Object * ObjectString::Proto::Split::run(Scope *scope) {
		ObjectString *this_ = scope->this_->toObjectString();
		if (scope->arguments->vector.len() < 1) {
			return new IllegalArgument();
		}
		ObjectChar *chSeparator = scope->arguments->vector[0]->toObjectChar();
		if (chSeparator) {
			ObjectArray *result = new ObjectArray();
			unsigned int len = this_->value.len(),
				end = 0,
				begin = 0;
			while (end < len) {
				if (this_->value[end] == chSeparator->value) {
					result->vector.pushBack(new ObjectString((this_->value.subString(begin, end - begin))));
					begin = end + 1;
				}
				end++;
			}
			result->vector.pushBack(new ObjectString((this_->value.subString(begin, end - begin))));
			return result;
		}

		return new IllegalArgument();
	}

	Object * ObjectString::Proto::Split::getInstance() {
		static Split __this;
		return &__this;
	}
}
