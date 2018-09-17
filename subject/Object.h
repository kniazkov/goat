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

#include "Map.h"
#include "Set.h"
#include "Vector.h"
#include "PlainVector.h"
#include "String.h"
#include "WideString.h"
#include "RawBuffer.h"
#include "List.h"
#include "MultiList.h"
#include "Type.h"
#include "Pool.h"
#include "StringIndex.h"

namespace goat {

	class Object;
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
	class ObjectVoid;
	class ObjectNull;
	class ObjectUndefined;
	class ObjectFile;
	class ObjectStringBuilder;
	class ObjectByteArray;
	class PrimitiveHandler;
	class Container;

	class PrimitiveHandler {
	public:
		virtual Object * toObject(Container *ctr);
	};

	class Container {
	public:
		PrimitiveHandler *handler;
		union {
			Object *_object;
			bool _boolean;
			lint _integer;
			ldouble _real;
		} data;

		Container(Object *_object) {
			handler = nullptr;
			data._object = _object;
		}

		Object *toObject() {
			if (handler)  {
				return handler->toObject(this);
			}
			return data._object;
		}
	};

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
			MARKED = 1,
			PERMANENT = 2,
			LOCKED = 4
		};
		void *list;
		Object *prev, *next;
		uint32 status;
		Map<int32, Object*> objects;
		List<Pair> chain;
		PlainVector<Object *> proto;
		static StringIndex indexes;

		Object();
		Object(bool builtIn);
		Object(Object *_proto);
		virtual ~Object();
		void mark();
		void mark_2();
		virtual void trace();
		static inline int32 createIndex(String key);
		static inline int32 searchIndex(String key);
		static inline String getKey(int32 index);
	private:
		Object *find_(int32 index);
		Object *find_(WideString key);
		Object *find_(Object *key);
	public:
		Object *find(int32 index);
		Object *find(WideString key);
		Object *find(Object *key);
		void insert(int32 index, Object *value);
		void insert(Object *key, Object *value);
		bool replace(int32 index, Object *repl);
		void clone(Object *_clone);
		virtual bool equals(Object *_obj);
		static bool equals(Object *first, Object *second);
		bool instanceOf(Object *_obj);
		void findAll(String key, PlainVector<Object *> *vector);
		void findUnique(String key, PlainVector<Object *> *vector);
		virtual void flat(Object *fobj);
		void enumerate(Vector<Pair> *vector);

		virtual WideString toWideString(Set<Object*> &set);
		virtual WideString toWideStringNotation(Set<Object*> &set);
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
		virtual ObjectVoid *toObjectVoid();
		virtual ObjectNull *toObjectNull();
		virtual ObjectUndefined *toObjectUndefined();
		virtual ObjectFile *toObjectFile();
		virtual ObjectStringBuilder *toObjectStringBuilder();
		virtual ObjectByteArray * toObjectByteArray();

		void *operator new (__size size);
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

	int32 Object::createIndex(String key) {
		return indexes.createIndex(key);
	}

	int32 Object::searchIndex(String key) {
		return indexes.searchIndex(key);
	}

	String Object::getKey(int32 index) {
		return indexes.getString(index);
	}
}
