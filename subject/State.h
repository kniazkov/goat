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

#pragma once

#include "Object.h"
#include "Type.h"
#include "Exception.h"
#include "Scope.h"


namespace goat {

	class State {
	public:
		enum Mode {
			RUN,
			EXCEPTION,
			RETURN
		};

		Mode mode;
		Object *thru;
		State *prev;
		Scope *scope;

		State(State *_prev);
		virtual ~State() {}
		virtual State *execute();
	protected:
		virtual State *next() = 0;
	public:
		virtual void ret(Object *obj);
		void mark(bool deep);
		virtual void trace();
		State * throw_(Object *obj);
		State * return_(Object *obj);

		void * operator new(MemorySize size);
		void operator delete(void *ptr);

		class NotImplemented : public Exception {
		public:
			RawString toRawString() override;
		};

		class UncaughtException : public Exception {
		public:
			Object *obj;

			UncaughtException(Object *_obj) : obj(_obj) {
			}
			RawString toRawString() override;
		};
	};

}
