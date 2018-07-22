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

#include "ObjectBoolean.h"
#include "ObjectException.h"

namespace goat {

	ObjectBoolean::ObjectBoolean(bool _value) {
		value = _value;
		proto[0] = Proto::getInstance();
	}

	ObjectBoolean * ObjectBoolean::toObjectBoolean() {
		return this;
	}

	WideString ObjectBoolean::toWideString() {
		return value ? L"true" : L"false";
	}

	bool ObjectBoolean::equals(Object *_obj) {
		ObjectBoolean *obj = _obj->toObjectBoolean();
		return obj && obj->value == value;
	}

	ObjectBoolean::Proto::Proto() {
		status = PERMANENT | LOCKED;

		objects.insert(createIndex("clone"), Clone::getInstance());
		objects.insert(createIndex("!"), OperatorNot::getInstance());
		objects.insert(createIndex("&&"), OperatorAnd::getInstance());
		objects.insert(createIndex("||"), OperatorOr::getInstance());
	}

	Object * ObjectBoolean::Proto::getInstance() {
		static Proto __this;
		return &__this;
	}


	Object * ObjectBoolean::Proto::Clone::run(Scope *scope) {
		ObjectBoolean *this_ = scope->this_->toObjectBoolean();
		ObjectBoolean *obj = new ObjectBoolean(this_->value);
		this_->clone(obj);
		return obj;
	}

	Object * ObjectBoolean::Proto::Clone::getInstance() {
		static Clone __this;
		return &__this;
	}


	Object * ObjectBoolean::Proto::OperatorNot::run(Scope *scope) {
		ObjectBoolean *this_ = scope->this_->toObjectBoolean();
		return new ObjectBoolean(!this_->value);
	}

	Object * ObjectBoolean::Proto::OperatorNot::getInstance() {
		static OperatorNot __this;
		return &__this;
	}


	Object * ObjectBoolean::Proto::OperatorAnd::run(Scope *scope) {
		ObjectBoolean *this_ = scope->this_->toObjectBoolean();
		ObjectBoolean *operand = scope->arguments->vector[0]->toObjectBoolean();
		if (!operand) {
			return new IllegalArgument();
		}
		return new ObjectBoolean(this_->value && operand->value);
	}

	Object * ObjectBoolean::Proto::OperatorAnd::getInstance() {
		static OperatorAnd __this;
		return &__this;
	}


	Object * ObjectBoolean::Proto::OperatorOr::run(Scope *scope) {
		ObjectBoolean *this_ = scope->this_->toObjectBoolean();
		ObjectBoolean *operand = scope->arguments->vector[0]->toObjectBoolean();
		if (!operand) {
			return new IllegalArgument();
		}
		return new ObjectBoolean(this_->value || operand->value);
	}

	Object * ObjectBoolean::Proto::OperatorOr::getInstance() {
		static OperatorOr __this;
		return &__this;
	}

}
