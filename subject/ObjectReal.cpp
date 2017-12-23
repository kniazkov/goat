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

#include "ObjectReal.h"
#include "ObjectInteger.h"
#include "ObjectBoolean.h"
#include "Assert.h"
#include "ObjectException.h"

namespace goat {

	ObjectReal::ObjectReal(long double _value) {
		value = _value;
		proto[0] = Proto::getInstance();
	}

	ObjectReal * ObjectReal::toObjectReal() {
		return this;
	}

	WideString ObjectReal::toWideString() {
		return WideString::valueOf(value, 8, true);
	}

	bool ObjectReal::equals(Object *_obj) {
		ObjectReal *obj = _obj->toObjectReal();
		return obj && obj->value == value;
	}

	ObjectReal::Proto::Proto() {
		status = PERMANENT;

		objects.insert("clone", Clone::getInstance());
		objects.insert("+", OperatorPlus::getInstance());
		objects.insert("-", OperatorMinus::getInstance());
		objects.insert("*", OperatorMul::getInstance());
		objects.insert("/", OperatorDiv::getInstance());
		objects.insert("==", OperatorEqual::getInstance());
		objects.insert("!=", OperatorNotEqual::getInstance());
		objects.insert("<", OperatorLess::getInstance());
		objects.insert(">", OperatorGreater::getInstance());
		objects.insert("++", OperatorIncrement::getInstance());
		objects.insert("--", OperatorDecrement::getInstance());
	}

	Object * ObjectReal::Proto::getInstance() {
		static Proto __this;
		return &__this;
	}

	Object * ObjectReal::Proto::OperatorPlus::run(Scope *scope) {
		ObjectReal *this_ = scope->this_->toObjectReal();
		ObjectReal *operand = nullptr;
		if (scope->arguments && scope->arguments->vector.len() > 0) {
			operand = scope->arguments->vector[0]->toObjectReal();
			if (!operand) {
				return new IllegalArgument();
			}
			return new ObjectReal(this_->value + operand->value);
		}
		else {
			return new ObjectReal(this_->value);
		}
	}

	Object * ObjectReal::Proto::OperatorPlus::getInstance() {
		static OperatorPlus __this;
		return &__this;
	}


	Object * ObjectReal::Proto::OperatorMinus::run(Scope *scope) {
		ObjectReal *this_ = scope->this_->toObjectReal();
		ObjectReal *operand = nullptr;
		if (scope->arguments && scope->arguments->vector.len() > 0) {
			operand = scope->arguments->vector[0]->toObjectReal();
			if (!operand) {
				return new IllegalArgument();
			}
			return new ObjectReal(this_->value - operand->value);
		}
		else {
			return new ObjectReal(-this_->value);
		}
	}

	Object * ObjectReal::Proto::OperatorMinus::getInstance() {
		static OperatorMinus __this;
		return &__this;
	}


	Object * ObjectReal::Proto::OperatorMul::run(Scope *scope) {
		ObjectReal *this_ = scope->this_->toObjectReal();
		Object *arg = scope->arguments->vector[0];
		ObjectInteger *operInt = arg->toObjectInteger();
		if (operInt) {
			return new ObjectReal(this_->value * ((long double)operInt->value));
		}
		ObjectReal *operand = arg->toObjectReal();
		if (!operand) {
			return new IllegalArgument();
		}
		return new ObjectReal(this_->value * operand->value);
	}

	Object * ObjectReal::Proto::OperatorMul::getInstance() {
		static OperatorMul __this;
		return &__this;
	}


	Object * ObjectReal::Proto::OperatorDiv::run(Scope *scope) {
		ObjectReal *this_ = scope->this_->toObjectReal();
		Object *arg = scope->arguments->vector[0];
		ObjectInteger *operInt = arg->toObjectInteger();
		if (operInt) {
			if (operInt->value == 0) {
				return new DivisionByZero();
			}
			return new ObjectReal(this_->value / ((long double)operInt->value));
		}
		ObjectReal *operand = arg->toObjectReal();
		if (!operand) {
			return new IllegalArgument();
		}
		if (operand->value == 0) {
			return new DivisionByZero();
		}
		return new ObjectReal(this_->value / operand->value);
	}

	Object * ObjectReal::Proto::OperatorDiv::getInstance() {
		static OperatorDiv __this;
		return &__this;
	}


	Object * ObjectReal::Proto::OperatorEqual::run(Scope *scope) {
		ObjectReal *this_ = scope->this_->toObjectReal();
		ObjectReal *operand = scope->arguments->vector[0]->toObjectReal();
		if (!operand) {
			// should be exception
			return nullptr;
		}
		return new ObjectBoolean(this_->value == operand->value);
	}

	Object * ObjectReal::Proto::OperatorEqual::getInstance() {
		static OperatorEqual __this;
		return &__this;
	}


	Object * ObjectReal::Proto::OperatorNotEqual::run(Scope *scope) {
		ObjectReal *this_ = scope->this_->toObjectReal();
		ObjectReal *operand = scope->arguments->vector[0]->toObjectReal();
		if (!operand) {
			// should be exception
			return nullptr;
		}
		return new ObjectBoolean(this_->value != operand->value);
	}

	Object * ObjectReal::Proto::OperatorNotEqual::getInstance() {
		static OperatorNotEqual __this;
		return &__this;
	}


	Object * ObjectReal::Proto::OperatorLess::run(Scope *scope) {
		ObjectReal *this_ = scope->this_->toObjectReal();
		ObjectReal *operand = scope->arguments->vector[0]->toObjectReal();
		if (!operand) {
			// should be exception
			return nullptr;
		}
		return new ObjectBoolean(this_->value < operand->value);
	}

	Object * ObjectReal::Proto::OperatorLess::getInstance() {
		static OperatorLess __this;
		return &__this;
	}


	Object * ObjectReal::Proto::OperatorGreater::run(Scope *scope) {
		ObjectReal *this_ = scope->this_->toObjectReal();
		ObjectReal *operand = scope->arguments->vector[0]->toObjectReal();
		if (!operand) {
			// should be exception
			return nullptr;
		}
		return new ObjectBoolean(this_->value > operand->value);
	}

	Object * ObjectReal::Proto::OperatorGreater::getInstance() {
		static OperatorGreater __this;
		return &__this;
	}


	Object * ObjectReal::Proto::OperatorIncrement::run(Scope *scope) {
		ObjectReal *this_ = scope->this_->toObjectReal();
		return new ObjectReal(this_->value + 1);
	}

	Object * ObjectReal::Proto::OperatorIncrement::getInstance() {
		static OperatorIncrement __this;
		return &__this;
	}


	Object * ObjectReal::Proto::OperatorDecrement::run(Scope *scope) {
		ObjectReal *this_ = scope->this_->toObjectReal();
		return new ObjectReal(this_->value - 1);
	}

	Object * ObjectReal::Proto::OperatorDecrement::getInstance() {
		static OperatorDecrement __this;
		return &__this;
	}


	Object * ObjectReal::Proto::Clone::run(Scope *scope) {
		ObjectReal *this_ = scope->this_->toObjectReal();
		ObjectReal *obj = new ObjectReal(this_->value);
		this_->clone(obj);
		return obj;
	}

	Object * ObjectReal::Proto::Clone::getInstance() {
		static Clone __this;
		return &__this;
	}

}
