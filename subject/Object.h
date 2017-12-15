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

#include "Map.h"
#include "Vector.h"
#include "PlainVector.h"
#include "String.h"
#include "WideString.h"
#include "RawBuffer.h"
#include "List.h"
#include "MultiList.h"
#include "Type.h"
#include "Pool.h"

namespace goat {

	class ObjectString;
	class ObjectFunction;
	class ObjectThread;
	class ObjectArray;
	class ObjectBuiltIn;
	class ObjectInteger;
	class ObjectReal;
	class ObjectException;
	class ObjectBoolean;
	class ObjectChar;

	class Object {
	public:
		class Pair {
		public:
			Object *key;
			Object *value;

			Pair() : key(nullptr), value(nullptr) {
			}

			Pair(Object *_key, Object *_value) : key(_key), value(_value) {
			}
		};

		enum Status {
			UNMARKED,
			MARKED,
			PERMANENT
		};
		void *list;
		Object *prev, *next;
		Status status;
		Map<String, Object*> objects;
		List<Pair> chain;
		PlainVector<Object *> proto;

		Object();
		Object(bool builtIn);
		Object(Object *_proto);
		virtual ~Object();
		void mark();
		void mark_2();
		virtual void trace();
		Object *find(String key);
		Object *find(WideString key);
		Object *find(Object *key);
		void insert(String key, Object *value);
		void insert(Object *key, Object *value);
		bool replace(String key, Object *repl);
		void clone(Object *_clone);
		virtual bool equals(Object *_obj);
		static bool equals(Object *first, Object *second);
		bool instanceOf(Object *_obj);
		void findAll(String key, PlainVector<Object *> *vector);
		void findUnique(String key, PlainVector<Object *> *vector);
		virtual void flat(Object *fobj);
		void enumerate(Vector<Pair> *vector);

		virtual WideString toWideString();
		virtual WideString toWideStringNotation();
		virtual RawString toRawString();

		virtual ObjectString * toObjectString();
		virtual ObjectFunction * toObjectFunction();
		virtual ObjectThread * toObjectThread();
		virtual ObjectArray * toObjectArray();
		virtual ObjectBuiltIn * toObjectBuiltIn();
		virtual ObjectInteger * toObjectInteger();
		virtual ObjectReal * toObjectReal();
		virtual ObjectException *toObjectException();
		virtual ObjectBoolean *toObjectBoolean();
		virtual ObjectChar *toObjectChar();

		void *operator new (MemorySize size);
		void operator delete (void *ptr);
	};

	extern long long totalObjMem;

	class ObjectList : public MultiList<Object, &Object::prev, &Object::next, &Object::list> {
	public:
		static ObjectList global;
		static ObjectList forMarking;
		void unmark();
		bool mark_2();
		void sweep();
	};

	class SuperObject : public Object {
	public:
		SuperObject();
		static Object *getInstance();
		void flat(Object *fobj) override;
	};

}
