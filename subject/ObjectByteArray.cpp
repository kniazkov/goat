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

#include "ObjectByteArray.h"
#include "ObjectString.h"
#include "ObjectInteger.h"
#include "WideStringBuilder.h"
#include "ObjectException.h"

namespace goat {

	ObjectByteArray::ObjectByteArray() {
		proto[0] = Proto::getInstance();
	}

	ObjectByteArray * ObjectByteArray::toObjectByteArray() {
		return this;
	}

	WideString ObjectByteArray::toWideString() {
		WideStringBuilder b;
		b << (wchar)'[';
		int i = 0;
		vector.forEach([&](long long int v) {
			if (i) {
				b << (wchar)',';
			}
			i++;
			b << WideString::valueOf(v);
		});
		b << (wchar)']';
		return b.toWideString();
	}

	class ByteArrayClone : public ObjectBuiltIn {
	public:
		Object * run(Scope *scope) override;
		static Object *getInstance();
	};

	class ByteArrayLength : public ObjectBuiltIn {
	public:
		Object * run(Scope *scope) override;
		static Object *getInstance();
	};

	class ByteArrayPush : public ObjectBuiltIn {
	public:
		Object * run(Scope *scope) override;
		static Object *getInstance();
	};

	ObjectByteArray::Proto::Proto() {
		status = PERMANENT;

		objects.insert("clone", ByteArrayClone::getInstance());
		objects.insert("length", ByteArrayLength::getInstance());
		objects.insert("push", ByteArrayPush::getInstance());
	}

	Object * ObjectByteArray::Proto::getInstance() {
		static Proto __this;
		return &__this;
	}


	Object * ByteArrayClone::run(Scope *scope) {
		ObjectByteArray *this_ = scope->this_->toObjectByteArray();
		ObjectByteArray *obj = new ObjectByteArray();
		this_->vector.clone(obj->vector);
		this_->clone(obj);
		return obj;
	}

	Object *ByteArrayClone::getInstance() {
		static ByteArrayClone __this;
		return &__this;
	}


	Object * ByteArrayLength::run(Scope *scope) {
		ObjectByteArray *this_ = scope->this_->toObjectByteArray();
		return new ObjectInteger(this_->vector.len());
	}

	Object *ByteArrayLength::getInstance() {
		static ByteArrayLength __this;
		return &__this;
	}


	Object * ByteArrayPush::run(Scope *scope) {
		ObjectByteArray *this_ = scope->this_->toObjectByteArray();
		unsigned int i, l = scope->arguments->vector.len();
		for (i = 0; i < l; i++) {
			ObjectInteger *obj = scope->arguments->vector[i]->toObjectInteger();
			if (!obj || obj->value < 0 || obj->value > 255) {
				return new IllegalArgument();
			}
			this_->vector.pushBack((unsigned char)obj->value);
		}
		return nullptr;
	}

	Object *ByteArrayPush::getInstance() {
		static ByteArrayPush __this;
		return &__this;
	}
}