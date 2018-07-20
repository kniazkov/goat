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

#include "BuiltIn.h"
#include "ObjectArray.h"
#include "ObjectString.h"
#include "ObjectInteger.h"
#include "ObjectException.h"
#include "ObjectBoolean.h"
#include "ObjectArray.h"
#include "ObjectFunction.h"
#include "ObjectThread.h"
#include "ObjectChar.h"
#include "ObjectReal.h"
#include "ObjectNull.h"
#include "ObjectUndefined.h"
#include "ObjectFile.h"
#include "ObjectStringBuilder.h"
#include "ObjectByteArray.h"

namespace goat {

	class Print : public ObjectBuiltIn {
	protected:
		OutputStream<wchar> *out;

	public:
		Print(OutputStream<wchar> *_out);
		Object *run(Scope *scope) override;
	};

	class Open : public ObjectBuiltIn {
	public:
		Object * run(Scope *scope) override;
		static Object *getInstance();
	};

	class Defined : public ObjectBuiltIn {
	public:
		Object * run(Scope *scope) override;
		static Object *getInstance();
	};

	class IsNumber : public ObjectBuiltIn {
	public:
		Object * run(Scope *scope) override;
		static Object *getInstance();
	};

	class Clock : public ObjectBuiltIn {
	public:
		Object * run(Scope *scope) override;
		static Object *getInstance();
	};

	Scope * BuiltIn::create(Environment *env) {
		Scope *s = new Scope();
		s->objects.insert("print", new Print(env->out));
		s->objects.insert("open", Open::getInstance());
		s->objects.insert("defined", Defined::getInstance());
		s->objects.insert("isNumber", IsNumber::getInstance());
		s->objects.insert("clock", Clock::getInstance());

		s->objects.insert("String", ObjectString::Proto::getInstance());
		s->objects.insert("Integer", ObjectInteger::Proto::getInstance());
		s->objects.insert("Exception", ObjectException::Proto::getInstance());
		s->objects.insert("Boolean", ObjectBoolean::Proto::getInstance());
		s->objects.insert("Char", ObjectChar::Proto::getInstance());
		s->objects.insert("Array", ObjectArray::Proto::getInstance());
		s->objects.insert("Function", ObjectFunction::Proto::getInstance());
		s->objects.insert("Thread", ObjectThread::Proto::getInstance());
		s->objects.insert("Real", ObjectReal::Proto::getInstance());
		s->objects.insert("Null", ObjectNull::getInstance());
		s->objects.insert("Undefined", ObjectUndefined::getInstance());
		s->objects.insert("File", ObjectFile::Proto::getInstance());
		s->objects.insert("StringBuilder", ObjectStringBuilder::Proto::getInstance());
		s->objects.insert("ByteArray", ObjectByteArray::Proto::getInstance());
		return s;
	}

	Print::Print(OutputStream<wchar> *_out) : out(_out) {
		status = UNMARKED;
	}

	Object * Print::run(Scope *scope) {
		if (!out) {
			return nullptr;
		}

		ObjectArray * args = scope->arguments;
		if (args->vector.len() > 0) {
			Object *arg0 = args->vector[0];
			WideString value = arg0 ? arg0->toWideString() : L"undefined";

			value.forEach([&](wchar ch) {
				out->write(ch);
			});
		}

		// not enough arguments, exception ??
		return nullptr;
	}


	Object * Open::run(Scope *scope) {
		ObjectArray * args = scope->arguments;
		if (args->vector.len() == 2) {
			ObjectString *fname = args->vector[0]->toObjectString();
			ObjectInteger *mode = args->vector[1]->toObjectInteger();
			if (fname && mode) {
				Platform::File *file = Platform::File::open(fname->value.toString(), (Platform::File::Mode)mode->value);
				if (!file) {
					return nullptr;
				}
				else {
					return new ObjectFile(file);
				}
			}
		}

		return new IllegalArgument();
	}

	Object * Open::getInstance() {
		static Open __this;
		return &__this;
	}


	Object * Defined::run(Scope *scope) {
		ObjectArray * args = scope->arguments;
		if (args->vector.len() > 0) {
			Object *obj = args->vector[0];
			return new ObjectBoolean(obj->toObjectUndefined() == nullptr);
		}
		return new IllegalArgument();
	}

	Object * Defined::getInstance() {
		static Defined __this;
		return &__this;
	}


	Object * IsNumber::run(Scope *scope) {
		ObjectArray * args = scope->arguments;
		if (args->vector.len() > 0) {
			Object *obj = args->vector[0];
			return new ObjectBoolean(obj != nullptr &&
				(obj->toObjectInteger() != nullptr || obj->toObjectReal() != nullptr));
		}
		return new IllegalArgument();
	}

	Object * IsNumber::getInstance() {
		static IsNumber __this;
		return &__this;
	}


	Object * Clock::run(Scope *scope) {
		return new ObjectInteger(Platform::getTimeNs());
	}

	Object * Clock::getInstance() {
		static Clock __this;
		return &__this;
	}
}
