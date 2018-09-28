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
#include "ObjectVoid.h"
#include "ObjectNull.h"
#include "ObjectUndefined.h"
#include "ObjectFile.h"
#include "ObjectStringBuilder.h"
#include "ObjectByteArray.h"

namespace goat {

	class Print : public ObjectBuiltIn {
	protected:
		OutputStream<wchar> *out;
		bool newLine;

	public:
		Print(OutputStream<wchar> *_out, bool _newLine);
		Object *run(Scope *scope) override;
	};

	class Getc : public ObjectBuiltIn {
	protected:
		InputStream<wchar> *in;

	public:
		Getc(InputStream<wchar> *_in);
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
		s->objects.insert(Object::createIndex("print"), new Print(env->out, false));
		s->objects.insert(Object::createIndex("println"), new Print(env->out, true));
		s->objects.insert(Object::createIndex("getc"), new Getc(env->in));
		s->objects.insert(Object::createIndex("open"), Open::getInstance());
		s->objects.insert(Object::createIndex("defined"), Defined::getInstance());
		s->objects.insert(Object::createIndex("isNumber"), IsNumber::getInstance());
		s->objects.insert(Object::createIndex("clock"), Clock::getInstance());

		s->objects.insert(Object::createIndex("String"), ObjectString::Proto::getInstance());
		s->objects.insert(Object::createIndex("Integer"), ObjectInteger::Proto::getInstance());
		s->objects.insert(Object::createIndex("Exception"), ObjectException::Proto::getInstance());
		s->objects.insert(Object::createIndex("Boolean"), ObjectBoolean::Proto::getInstance());
		s->objects.insert(Object::createIndex("Char"), ObjectChar::Proto::getInstance());
		s->objects.insert(Object::createIndex("Array"), ObjectArray::Proto::getInstance());
		s->objects.insert(Object::createIndex("Function"), ObjectFunction::Proto::getInstance());
		s->objects.insert(Object::createIndex("Thread"), ObjectThread::Proto::getInstance());
		s->objects.insert(Object::createIndex("Real"), ObjectReal::Proto::getInstance());
		s->objects.insert(Object::createIndex("Void"), ObjectVoid::getInstance());
		s->objects.insert(Object::createIndex("Null"), ObjectNull::getInstance());
		s->objects.insert(Object::createIndex("Undefined"), ObjectUndefined::getInstance());
		s->objects.insert(Object::createIndex("File"), ObjectFile::Proto::getInstance());
		s->objects.insert(Object::createIndex("StringBuilder"), ObjectStringBuilder::Proto::getInstance());
		s->objects.insert(Object::createIndex("ByteArray"), ObjectByteArray::Proto::getInstance());
		return s;
	}

	Print::Print(OutputStream<wchar> *_out, bool _newLine) : out(_out), newLine(_newLine) {
		status = LOCKED;
	}

	Object * Print::run(Scope *scope) {
		if (!out) {
			return nullptr;
		}

		ObjectArray * args = scope->arguments;
		if (args->vector.len() > 0) {
			Object *arg0 = args->vector[0];
			Set<Object*> set;
			WideString value = arg0 ? arg0->toWideString(set) : L"undefined";

			value.forEach([&](wchar ch) {
				out->write(ch);
			});
			if (newLine) {
				out->write(L"\n");
			}
		}

		// not enough arguments, exception ??
		return nullptr;
	}


	Getc::Getc(InputStream<wchar> *_in) : in(_in) {
		status = LOCKED;
	}

	Object * Getc::run(Scope *scope) {
		if (!in) {
			return nullptr;
		}
		if (in->hasData()) {
			return new ObjectChar(in->read());
		}
		return new ObjectChar(0);
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
