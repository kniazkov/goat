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

#include "ObjectFunction.h"
#include "ObjectBuiltIn.h"

namespace goat {

	ObjectFunction::ObjectFunction(Function *_function, Scope *_context) :
		function(_function), context(_context) {
		proto[0] = Proto::getInstance();
	}

	ObjectFunction * ObjectFunction::toObjectFunction() {
		return this;
	}

	WideString ObjectFunction::toWideString() {
		return L"Function";
	}

	void ObjectFunction::trace() {
		if (context) {
			context->mark();
		}
	}

	class ObjFunctionClone : public ObjectBuiltIn {
	public:
		Object * run(Scope *scope) override;
		static Object *getInstance();
	};

	ObjectFunction::Proto::Proto() {
		status = PERMANENT;

		objects.insert("clone", ObjFunctionClone::getInstance());
	}

	Object * ObjectFunction::Proto::getInstance() {
		static Proto __this;
		return &__this;
	}

	Object * ObjFunctionClone::run(Scope *scope) {
		ObjectFunction *this_ = scope->this_->toObjectFunction();
		ObjectFunction *obj = new ObjectFunction(this_->function, this_->context);
		this_->clone(obj);
		return obj;
	}

	Object *ObjFunctionClone::getInstance() {
		static ObjFunctionClone __this;
		return &__this;
	}
}
