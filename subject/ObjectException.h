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

#pragma once

#include "Object.h"

namespace goat {

	class ObjectException : public Object {
	protected:
		ObjectException();

	public:
		ObjectException * toObjectException() override;

		class Proto : public Object {
		protected:
			bool initialized;
		public:
			Proto();
			void init();
			static Object *getInstance();
		};
	};

	class NameIsNotDefined : public ObjectException {
	public:
		String name;

		NameIsNotDefined(String _name);
		WideString toWideString(Set<Object*> &set) override;

		class Proto : public ObjectException {
		public:
			Proto();
			static Object *getInstance();
		};
	};

	class IsNotAFunction : public ObjectException {
	public:
		String name;

		IsNotAFunction(String _name);
		WideString toWideString(Set<Object*> &set) override;

		class Proto : public ObjectException {
		public:
			Proto();
			static Object *getInstance();
		};
	};

	class OperatorIsNotDefined : public ObjectException {
	public:
		String name;

		OperatorIsNotDefined(String _name);
		WideString toWideString(Set<Object*> &set) override;

		class Proto : public ObjectException {
		public:
			Proto();
			static Object *getInstance();
		};
	};

	class CanNotCallUndefined : public ObjectException {
	public:
		CanNotCallUndefined();
		WideString toWideString(Set<Object*> &set) override;

		class Proto : public ObjectException {
		public:
			Proto();
			static Object *getInstance();
		};
	};

	class CanNotCallExpression : public ObjectException {
	public:
		CanNotCallExpression();
		WideString toWideString(Set<Object*> &set) override;

		class Proto : public ObjectException {
		public:
			Proto();
			static Object *getInstance();
		};
	};

	class CanNotReadPropertyOfUndefined : public ObjectException {
	public:
		WideString prop;

		CanNotReadPropertyOfUndefined(Object *_obj);
		CanNotReadPropertyOfUndefined(String _name);
		WideString toWideString(Set<Object*> &set) override;

		class Proto : public ObjectException {
		public:
			Proto();
			static Object *getInstance();
		};
	};

	class CanNotWritePropertyOfUndefined : public ObjectException {
	public:
		WideString prop;

		CanNotWritePropertyOfUndefined(Object *_obj);
		CanNotWritePropertyOfUndefined(String _name);
		WideString toWideString(Set<Object*> &set) override;

		class Proto : public ObjectException {
		public:
			Proto();
			static Object *getInstance();
		};
	};

	class CanNotWritePropertyOfLockedObject : public ObjectException {
	public:
		WideString prop;

		CanNotWritePropertyOfLockedObject(Object *_obj);
		CanNotWritePropertyOfLockedObject(String _name);
		WideString toWideString(Set<Object*> &set) override;

		class Proto : public ObjectException {
		public:
			Proto();
			static Object *getInstance();
		};
	};

	class CanNotReadOperatorOfUndefined : public ObjectException {
	public:
		String name;

		CanNotReadOperatorOfUndefined(String _name);
		WideString toWideString(Set<Object*> &set) override;

		class Proto : public ObjectException {
		public:
			Proto();
			static Object *getInstance();
		};
	};

	class DivisionByZero : public ObjectException {
	public:
		DivisionByZero();
		WideString toWideString(Set<Object*> &set) override;

		class Proto : public ObjectException {
		public:
			Proto();
			static Object *getInstance();
		};
	};

	class IncorrectIndex : public ObjectException {
	public:
		IncorrectIndex();
		WideString toWideString(Set<Object*> &set) override;

		class Proto : public ObjectException {
		public:
			Proto();
			static Object *getInstance();
		};
	};

	class OutOfBounds : public ObjectException {
	public:
		OutOfBounds();
		WideString toWideString(Set<Object*> &set) override;

		class Proto : public ObjectException {
		public:
			Proto();
			static Object *getInstance();
		};
	};

	class IllegalArgument : public ObjectException {
	public:
		IllegalArgument();
		WideString toWideString(Set<Object*> &set) override;

		class Proto : public ObjectException {
		public:
			Proto();
			static Object *getInstance();
		};
	};

	class NotImplemented : public ObjectException {
	public:
		NotImplemented();
		WideString toWideString(Set<Object*> &set) override;

		class Proto : public ObjectException {
		public:
			Proto();
			static Object *getInstance();
		};
	};

	class InvalidOperation : public ObjectException {
	public:
		InvalidOperation();
		WideString toWideString(Set<Object*> &set) override;

		class Proto : public ObjectException {
		public:
			Proto();
			static Object *getInstance();
		};
	};

	class PrototypeIsNotDefined : public ObjectException {
	public:
		PrototypeIsNotDefined();
		WideString toWideString(Set<Object*> &set) override;

		class Proto : public ObjectException {
		public:
			Proto();
			static Object *getInstance();
		};
	};
}
