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

#include "ObjectInteger.h"
#include "ObjectBoolean.h"
#include "ObjectString.h"
#include "ObjectReal.h"
#include "Assert.h"
#include "ObjectException.h"

namespace goat {

	ObjectInteger::ObjectInteger(long long int _value) {
		value = _value;
		proto[0] = Proto::getInstance();
	}

	ObjectInteger * ObjectInteger::toObjectInteger() {
		return this;
	}

	WideString ObjectInteger::toWideString() {
		return WideString::valueOf(value);
	}

	bool ObjectInteger::equals(Object *_obj) {
		ObjectInteger *objInt = _obj->toObjectInteger();
		if (objInt) {
			return objInt->value == value;
		}
		ObjectReal *objReal = _obj->toObjectReal();
		if (objReal) {
			return objReal->value == value;
		}
		return false;
	}

	ObjectInteger::Proto::Proto() {
		status = PERMANENT;

		objects.insert("clone", Clone::getInstance());
		objects.insert("toString", ToString::getInstance());
		objects.insert("+", OperatorPlus::getInstance());
		objects.insert("+=", OperatorPlus::getInstance());
		objects.insert("-", OperatorMinus::getInstance());
		objects.insert("-=", OperatorMinus::getInstance());
		objects.insert("*", OperatorMul::getInstance());
		objects.insert("*=", OperatorMul::getInstance());
		objects.insert("/", OperatorDiv::getInstance());
		objects.insert("/=", OperatorDiv::getInstance());
		objects.insert("%", OperatorMod::getInstance());
		objects.insert("%=", OperatorMod::getInstance());
		objects.insert("<", OperatorLess::getInstance());
		objects.insert("<=", OperatorLessEqual::getInstance());
		objects.insert(">", OperatorGreater::getInstance());
		objects.insert(">=", OperatorGreaterEqual::getInstance());
		objects.insert("++", OperatorIncrement::getInstance());
		objects.insert("--", OperatorDecrement::getInstance());
		objects.insert("~", OperatorNot::getInstance());
		objects.insert("<<", OperatorLeftShift::getInstance());
		objects.insert("<<=", OperatorLeftShift::getInstance());
		objects.insert(">>", OperatorSignedRightShift::getInstance());
		objects.insert(">>=", OperatorSignedRightShift::getInstance());
		objects.insert(">>>", OperatorZeroFillRightShift::getInstance());
		objects.insert(">>>=", OperatorZeroFillRightShift::getInstance());
		objects.insert("&", OperatorAnd::getInstance());
		objects.insert("&=", OperatorAnd::getInstance());
		objects.insert("^", OperatorXor::getInstance());
		objects.insert("^=", OperatorXor::getInstance());
		objects.insert("|", OperatorOr::getInstance());
		objects.insert("|=", OperatorOr::getInstance());
	}

	Object * ObjectInteger::Proto::getInstance() {
		static Proto __this;
		return &__this;
	}

	Object * ObjectInteger::Proto::OperatorPlus::run(Scope *scope) {
		ObjectInteger *this_ = scope->this_->toObjectInteger();
		if (scope->arguments && scope->arguments->vector.len() > 0) {
			Object *arg = scope->arguments->vector[0];
			ObjectReal *operReal = arg->toObjectReal();
			if (operReal) {
				return new ObjectReal(((long double)this_->value) + operReal->value);
			}
			ObjectInteger *operand = arg->toObjectInteger();
			if (!operand) {
				return new IllegalArgument();
			}
			return new ObjectInteger(this_->value + operand->value);
		}
		else {
			return new ObjectInteger(this_->value);
		}
	}

	Object * ObjectInteger::Proto::OperatorPlus::getInstance() {
		static OperatorPlus __this;
		return &__this;
	}


	Object * ObjectInteger::Proto::OperatorMinus::run(Scope *scope) {
		ObjectInteger *this_ = scope->this_->toObjectInteger();
		if (scope->arguments && scope->arguments->vector.len() > 0) {
			Object *arg = scope->arguments->vector[0];
			ObjectReal *operReal = arg->toObjectReal();
			if (operReal) {
				return new ObjectReal(((long double)this_->value) - operReal->value);
			}
			ObjectInteger *operand = arg->toObjectInteger();
			if (!operand) {
				return new IllegalArgument();
			}
			return new ObjectInteger(this_->value - operand->value);
		}
		else {
			return new ObjectInteger(-this_->value);
		}
	}

	Object * ObjectInteger::Proto::OperatorMinus::getInstance() {
		static OperatorMinus __this;
		return &__this;
	}


	Object * ObjectInteger::Proto::OperatorMul::run(Scope *scope) {
		ObjectInteger *this_ = scope->this_->toObjectInteger();
		Object *arg = scope->arguments->vector[0];
		ObjectReal *operReal = arg->toObjectReal();
		if (operReal) {
			return new ObjectReal(((long double)this_->value) * operReal->value);
		}
		ObjectInteger *operand = arg->toObjectInteger();
		if (!operand) {
			return new IllegalArgument();
		}
		return new ObjectInteger(this_->value * operand->value);
	}

	Object * ObjectInteger::Proto::OperatorMul::getInstance() {
		static OperatorMul __this;
		return &__this;
	}


	Object * ObjectInteger::Proto::OperatorDiv::run(Scope *scope) {
		ObjectInteger *this_ = scope->this_->toObjectInteger();
		Object *arg = scope->arguments->vector[0];
		ObjectReal *operReal = arg->toObjectReal();
		if (operReal) {
			if (operReal->value == 0) {
				return new DivisionByZero();
			}
			return new ObjectReal(((long double)this_->value) / operReal->value);
		}
		ObjectInteger *operand = arg->toObjectInteger();
		if (!operand) {
			return new IllegalArgument();
		}
		if (operand->value == 0) {
			return new DivisionByZero();
		}
		return new ObjectInteger(this_->value / operand->value);
	}

	Object * ObjectInteger::Proto::OperatorDiv::getInstance() {
		static OperatorDiv __this;
		return &__this;
	}


	Object * ObjectInteger::Proto::OperatorMod::run(Scope *scope) {
		ObjectInteger *this_ = scope->this_->toObjectInteger();
		ObjectInteger *operand = scope->arguments->vector[0]->toObjectInteger();
		if (!operand) {
			return new IllegalArgument();
		}
		return new ObjectInteger(this_->value % operand->value);
	}

	Object * ObjectInteger::Proto::OperatorMod::getInstance() {
		static OperatorMod __this;
		return &__this;
	}


	Object * ObjectInteger::Proto::OperatorLess::run(Scope *scope) {
		ObjectInteger *this_ = scope->this_->toObjectInteger();
		Object *arg = scope->arguments->vector[0];
		ObjectReal *operReal = arg->toObjectReal();
		if (operReal) {
			return new ObjectBoolean(((long double)this_->value) < operReal->value);
		}
		ObjectInteger *operand = arg->toObjectInteger();
		if (!operand) {
			return new IllegalArgument();
		}
		return new ObjectBoolean(this_->value < operand->value);
	}

	Object * ObjectInteger::Proto::OperatorLess::getInstance() {
		static OperatorLess __this;
		return &__this;
	}


	Object * ObjectInteger::Proto::OperatorLessEqual::run(Scope *scope) {
		ObjectInteger *this_ = scope->this_->toObjectInteger();
		Object *arg = scope->arguments->vector[0];
		ObjectReal *operReal = arg->toObjectReal();
		if (operReal) {
			return new ObjectBoolean(((long double)this_->value) <= operReal->value);
		}
		ObjectInteger *operand = arg->toObjectInteger();
		if (!operand) {
			return new IllegalArgument();
		}
		return new ObjectBoolean(this_->value <= operand->value);
	}

	Object * ObjectInteger::Proto::OperatorLessEqual::getInstance() {
		static OperatorLessEqual __this;
		return &__this;
	}


	Object * ObjectInteger::Proto::OperatorGreater::run(Scope *scope) {
		ObjectInteger *this_ = scope->this_->toObjectInteger();
		Object *arg = scope->arguments->vector[0];
		ObjectReal *operReal = arg->toObjectReal();
		if (operReal) {
			return new ObjectBoolean(((long double)this_->value) > operReal->value);
		}
		ObjectInteger *operand = arg->toObjectInteger();
		if (!operand) {
			return new IllegalArgument();
		}
		return new ObjectBoolean(this_->value > operand->value);
	}

	Object * ObjectInteger::Proto::OperatorGreater::getInstance() {
		static OperatorGreater __this;
		return &__this;
	}


	Object * ObjectInteger::Proto::OperatorGreaterEqual::run(Scope *scope) {
		ObjectInteger *this_ = scope->this_->toObjectInteger();
		Object *arg = scope->arguments->vector[0];
		ObjectReal *operReal = arg->toObjectReal();
		if (operReal) {
			return new ObjectBoolean(((long double)this_->value) >= operReal->value);
		}
		ObjectInteger *operand = arg->toObjectInteger();
		if (!operand) {
			return new IllegalArgument();
		}
		return new ObjectBoolean(this_->value >= operand->value);
	}

	Object * ObjectInteger::Proto::OperatorGreaterEqual::getInstance() {
		static OperatorGreaterEqual __this;
		return &__this;
	}


	Object * ObjectInteger::Proto::OperatorIncrement::run(Scope *scope) {
		ObjectInteger *this_ = scope->this_->toObjectInteger();
		return new ObjectInteger(this_->value + 1);
	}

	Object * ObjectInteger::Proto::OperatorIncrement::getInstance() {
		static OperatorIncrement __this;
		return &__this;
	}


	Object * ObjectInteger::Proto::OperatorDecrement::run(Scope *scope) {
		ObjectInteger *this_ = scope->this_->toObjectInteger();
		return new ObjectInteger(this_->value - 1);
	}

	Object * ObjectInteger::Proto::OperatorDecrement::getInstance() {
		static OperatorDecrement __this;
		return &__this;
	}


	Object * ObjectInteger::Proto::OperatorNot::run(Scope *scope) {
		ObjectInteger *this_ = scope->this_->toObjectInteger();
		return new ObjectInteger(~this_->value);
	}

	Object * ObjectInteger::Proto::OperatorNot::getInstance() {
		static OperatorNot __this;
		return &__this;
	}


	Object * ObjectInteger::Proto::OperatorLeftShift::run(Scope *scope) {
		ObjectInteger *this_ = scope->this_->toObjectInteger();
		ObjectInteger *operand = scope->arguments->vector[0]->toObjectInteger();
		if (!operand) {
			return new IllegalArgument();
		}
		return new ObjectInteger(this_->value << operand->value);
	}

	Object * ObjectInteger::Proto::OperatorLeftShift::getInstance() {
		static OperatorLeftShift __this;
		return &__this;
	}


	Object * ObjectInteger::Proto::OperatorSignedRightShift::run(Scope *scope) {
		ObjectInteger *this_ = scope->this_->toObjectInteger();
		ObjectInteger *operand = scope->arguments->vector[0]->toObjectInteger();
		if (!operand) {
			return new IllegalArgument();
		}
		return new ObjectInteger(this_->value >> operand->value);
	}

	Object * ObjectInteger::Proto::OperatorSignedRightShift::getInstance() {
		static OperatorSignedRightShift __this;
		return &__this;
	}


	Object * ObjectInteger::Proto::OperatorZeroFillRightShift::run(Scope *scope) {
		ObjectInteger *this_ = scope->this_->toObjectInteger();
		ObjectInteger *operand = scope->arguments->vector[0]->toObjectInteger();
		if (!operand) {
			return new IllegalArgument();
		}
		unsigned long long int value = (unsigned long long)this_->value >> operand->value;
		return new ObjectInteger((long long int)value);
	}

	Object * ObjectInteger::Proto::OperatorZeroFillRightShift::getInstance() {
		static OperatorZeroFillRightShift __this;
		return &__this;
	}


	Object * ObjectInteger::Proto::OperatorAnd::run(Scope *scope) {
		ObjectInteger *this_ = scope->this_->toObjectInteger();
		ObjectInteger *operand = scope->arguments->vector[0]->toObjectInteger();
		if (!operand) {
			return new IllegalArgument();
		}
		return new ObjectInteger(this_->value & operand->value);
	}

	Object * ObjectInteger::Proto::OperatorAnd::getInstance() {
		static OperatorAnd __this;
		return &__this;
	}


	Object * ObjectInteger::Proto::OperatorXor::run(Scope *scope) {
		ObjectInteger *this_ = scope->this_->toObjectInteger();
		ObjectInteger *operand = scope->arguments->vector[0]->toObjectInteger();
		if (!operand) {
			return new IllegalArgument();
		}
		return new ObjectInteger(this_->value ^ operand->value);
	}

	Object * ObjectInteger::Proto::OperatorXor::getInstance() {
		static OperatorXor __this;
		return &__this;
	}


	Object * ObjectInteger::Proto::OperatorOr::run(Scope *scope) {
		ObjectInteger *this_ = scope->this_->toObjectInteger();
		ObjectInteger *operand = scope->arguments->vector[0]->toObjectInteger();
		if (!operand) {
			return new IllegalArgument();
		}
		return new ObjectInteger(this_->value | operand->value);
	}

	Object * ObjectInteger::Proto::OperatorOr::getInstance() {
		static OperatorOr __this;
		return &__this;
	}


	Object * ObjectInteger::Proto::Clone::run(Scope *scope) {
		ObjectInteger *this_ = scope->this_->toObjectInteger();
		ObjectInteger *obj = new ObjectInteger(this_->value);
		this_->clone(obj);
		return obj;
	}

	Object * ObjectInteger::Proto::Clone::getInstance() {
		static Clone __this;
		return &__this;
	}


	Object * ObjectInteger::Proto::ToString::run(Scope *scope) {
		ObjectInteger *this_ = scope->this_->toObjectInteger();
		return new ObjectString(WideString::valueOf(this_->value));
	}

	Object * ObjectInteger::Proto::ToString::getInstance() {
		static ToString __this;
		return &__this;
	}
}
