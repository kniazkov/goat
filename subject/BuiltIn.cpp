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

namespace goat {

	Scope * BuiltIn::create(Environment *env) {
		Scope *s = new Scope();
		s->objects.insert("print", new Print(env->out));
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

}
