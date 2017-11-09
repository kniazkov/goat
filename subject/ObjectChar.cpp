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
	}

	Object * ObjectChar::Proto::getInstance() {
		static Proto __this;
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
}
