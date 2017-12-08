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

#include "Object.h"
#include "ObjectString.h"
#include "ObjectArray.h"
#include "ObjectBoolean.h"
#include "ObjectBuiltIn.h"
#include "ObjectException.h"
#include "ObjectFunction.h"
#include "ObjectInteger.h"
#include "WideStringBuilder.h"
#include "Assert.h"

namespace goat {

	ObjectList ObjectList::global;
	ObjectList ObjectList::forMarking;

	Object::Object() {
		status = UNMARKED;
		list = nullptr;
		ObjectList::global.pushBack(this);
		proto.pushBack(SuperObject::getInstance());
	}

	Object::Object(bool builtIn) {
		status = PERMANENT;
	}

	Object::Object(Object *_proto) : Object() {
		status = UNMARKED;
		list = nullptr;
		ObjectList::global.pushBack(this);
		proto.pushBack(_proto);
	}

	Object::~Object() {
		if (status != PERMANENT) {
			ObjectList::global.remove(this);
		}
	}

	void Object::mark() {
		if (status == UNMARKED) {
			status = MARKED;
			ObjectList::forMarking.pushBack(this);
		}
	}

	void Object::mark_2() {
		objects.forEach([](String key, Object *obj) {
			if (obj)
				obj->mark();
		});
		chain.forEach([](Pair &pair) {
			if (pair.key)
				pair.key->mark();
			if (pair.value)
				pair.value->mark();
		});
		proto.forEach([](Object *obj) {
			if (obj)
				obj->mark();
		});
		trace();
		ObjectList::global.pushBack(this);
	}

	void Object::trace() {

	}

	Object * Object::find(String key) {
		Object *found = nullptr;
		if (!objects.find(key, &found)) {
			List<Pair>::Item *pair = chain.first;
			while (pair) {
				ObjectString *objStr = pair->data.key->toObjectString();
				if (objStr) {
					if (key == objStr->value) {
						found = pair->data.value;
						break;
					}
				}
				pair = pair->next;
			}
			for (unsigned int i = 0; !found && i < proto.len(); i++) {
				found = proto[i]->find(key);
			}
		}
		return found;
	}

	Object * Object::find(WideString key) {
		Object *found = nullptr;
		if (!objects.find(key.toString(), &found)) {
			List<Pair>::Item *pair = chain.first;
			while (pair) {
				ObjectString *objStr = pair->data.key->toObjectString();
				if (objStr) {
					if (key == objStr->value) {
						found = pair->data.value;
						break;
					}
				}
				pair = pair->next;
			}
			for (unsigned int i = 0; !found && i < proto.len(); i++) {
				found = proto[i]->find(key);
			}
		}
		return found;
	}

	Object * Object::find(Object *key) {
		ObjectString *objStr = key->toObjectString();
		if (objStr) {
			return find(objStr->value);
		}
		Object *found = nullptr;
		List<Pair>::Item *pair = chain.first;
		while (pair) {
			if (key->equals(pair->data.key)) {
				found = pair->data.value;
				break;
			}
			pair = pair->next;
		}
		for (unsigned int i = 0; !found && i < proto.len(); i++) {
			found = proto[i]->find(key);
		}
		return found;
	}

	void Object::insert(String key, Object *value) {
		objects.insert(key, value);
		List<Pair>::Item *pair = chain.first;
		while (pair) {
			auto next = pair->next;
			ObjectString *objStr = pair->data.key->toObjectString();
			if (objStr && objStr->value == key) {
				chain.remove(pair);
			}
			pair = next;
		}
	}

	void Object::insert(Object *key, Object *value) {
		ObjectString *objStr = key->toObjectString();
		if (objStr) {
			objects.remove(objStr->value.toString());
		}
		List<Pair>::Item *pair = chain.first;
		while (pair) {
			if (key->equals(pair->data.key)) {
				pair->data.value = value;
				return;
			}
			pair = pair->next;
		}
		chain.pushBack(Pair(key, value));
	}

	bool Object::replace(String key, Object *repl) {
		if (objects.replace(key, repl)) {
			return true;
		}
		for (unsigned int i = 0; i < proto.len(); i++) {
			if (proto[i]->replace(key, repl)) {
				return true;
			}
		}
		return false;
	}

	void Object::findAll(String key, PlainVector<Object *> *vector) {
		Object *found = nullptr;
		if (objects.find(key, &found)) {
			vector->pushBack(found);
		}
		for (unsigned int i = 0; i < proto.len(); i++) {
			proto[i]->findAll(key, vector);
		}
	}

	void Object::findUnique(String key, PlainVector<Object *> *result) {
		PlainVector<Object *> tmp;
		findAll(key, &tmp);
		int l = (int)tmp.len();
		if (l > 0) {
			result->pushBack(tmp[l - 1]);
			for (int i = l - 2; i >= 0; i--) {
				Object *obj = tmp[i];
				if (!result->indexOf(obj)) {
					result->pushBack(obj);
				}
			}
		}
	}

	void Object::flat(Object *fobj) {
		proto.forEachReverse([&](Object *pobj) {
			pobj->flat(fobj);
		});

		objects.forEach([&](String key, Object *obj) {
			fobj->insert(key, obj);
		});

		chain.forEach([&](Pair pair) {
			fobj->insert(pair.key, pair.value);
		});
	}

	void Object::enumerate(Vector<Pair> *vector) {
		Object *fobj = new Object();
		flat(fobj);

		fobj->objects.forEach([&](String key, Object *obj) {
			vector->pushBack(Pair(new ObjectString(key.toWideString()), obj));
		});

		fobj->chain.forEach([&](Pair pair) {
			vector->pushBack(pair);
		});
	}

	void Object::clone(Object *_clone) {
		objects.clone(_clone->objects);
		_clone->proto.clean();
		proto.clone(_clone->proto);
	}

	bool Object::equals(Object *_obj) {
		return this == _obj;
	}

	bool Object::instanceOf(Object *_obj) {
		if (_obj == this) {
			return true;
		}

		for (unsigned int i = 0, l = proto.len(); i < l; i++) {
			if (proto[i]->instanceOf(_obj)) {
				return true;
			}
		}

		return false;
	}

	WideString Object::toWideString() {
		WideStringBuilder b;
		b << (wchar)'{';
		int i = 0;
		objects.forEach([&](String key, Object *obj) {
			if (i) {
				b << L',';
			}
			i++;
			b << key << L':' << obj->toWideStringNotation();
		});
		chain.forEach([&](Pair &pair) {
			if (i) {
				b << L',';
			}
			i++;
			b << pair.key->toWideStringNotation() << L':' << pair.value->toWideStringNotation();
		});
		b << (wchar)'}';
		return b.toWideString();
	}

	WideString Object::toWideStringNotation() {
		return toWideString();
	}

	RawString Object::toRawString() {
		return toWideString().toRawString();
	}

	ObjectString * Object::toObjectString() {
		return nullptr;
	}

	ObjectFunction * Object::toObjectFunction() {
		return nullptr;
	}

	ObjectThread * Object::toObjectThread() {
		return nullptr;
	}

	ObjectArray * Object::toObjectArray() {
		return nullptr;
	}

	ObjectBuiltIn * Object::toObjectBuiltIn() {
		return nullptr;
	}

	ObjectInteger * Object::toObjectInteger() {
		return nullptr;
	}

	ObjectException * Object::toObjectException() {
		return nullptr;
	}

	ObjectBoolean * Object::toObjectBoolean() {
		return nullptr;
	}

	ObjectChar * Object::toObjectChar() {
		return nullptr;
	}

	void ObjectList::mark_2() {
		while(count > 0) {
			forEach([](Object *obj) {
				obj->mark_2();
			});
		}
	}

	void ObjectList::sweep() {
		forEach([](Object *obj) {
			if (obj->status == Object::UNMARKED) {
				delete obj;
			}
			else if (obj->status == Object::MARKED) {
				obj->status = Object::UNMARKED;
			}
		});
	}

	////////

	union ObjectPlaceholder {
		char s[sizeof(ObjectString)];
		char b[sizeof(ObjectBoolean)];
		char e[sizeof(ObjectException)];
		char f[sizeof(ObjectFunction)];
		char i[sizeof(ObjectInteger)];
	};

	static Pool<sizeof(ObjectPlaceholder) + 2 * sizeof(void*)> objPool;
	long long int totalObjMem = 0;

	void * Object::operator new(MemorySize size) {
		totalObjMem += objPool.size;
		return objPool.alloc(size);
	}

	void Object::operator delete(void *ptr) {
		return objPool.free(ptr);
	}

	//////

	class Clone : public ObjectBuiltIn {
	public:
		Object * run(Scope *scope) override;
		static Object *getInstance();
	};

	Object * Clone::run(Scope *scope) {
		Object *obj = new Object();
		scope->this_->clone(obj);
		return obj;
	}

	Object * Clone::getInstance() {
		static Clone __this;
		return &__this;
	}

	
	class InstanceOf : public ObjectBuiltIn {
	public:
		Object * run(Scope *scope) override;
		static Object *getInstance();
	};

	Object * InstanceOf::run(Scope *scope) {
		bool result = false;
		if (scope->arguments->vector.len() > 0) {
			Object *operand = scope->arguments->vector[0];
			result = scope->this_->instanceOf(operand);
		}
		return new ObjectBoolean(result);
	}

	Object * InstanceOf::getInstance() {
		static InstanceOf __this;
		return &__this;
	}


	class Inherit : public ObjectBuiltIn {
	protected:
		class StateImpl : public State {
		public:
			enum Step {
				CLONE,
				DONE
			};

			Inherit *expr;
			Step step;
			Object *cloned;
			Object *proto;

			StateImpl(State *_prev, Inherit *_expr) 
				: State(_prev), expr(_expr), step(CLONE), cloned(nullptr), proto(nullptr) {
			}
			State *next() override;
			void trace() override;
			void ret(Object *obj) override;
		};

	public:
		State * createState(State *_prev) override;
		Object * run(Scope *scope) override;
		static Object *getInstance();
	};

	State * Inherit::createState(State *_prev) {
		return new StateImpl(_prev, this);
	}

	Object * Inherit::run(Scope *scope) {
		return nullptr;
	}

	Object * Inherit::getInstance() {
		static Inherit __this;
		return &__this;
	}

	State * Inherit::StateImpl::next() {
		switch (step) {
		case CLONE: {
			step = DONE;
			proto = scope->this_;
			Object *blank = scope->arguments->vector[0];
			Object *funcClone = blank->find("clone");
			ObjectFunction *of = funcClone->toObjectFunction();
			if (of) {
				changeScope(of->context->clone());
				scope->arguments = nullptr;
				scope->this_ = blank;
				scope->proto.pushBack(scope->proto[0]);
				scope->proto[0] = blank;
				return of->function->createState(this);
			}
			ObjectBuiltIn * obi = funcClone->toObjectBuiltIn();
			if (obi) {
				cloneScope();
				scope->arguments = nullptr;
				scope->this_ = blank;
				return obi->createState(this);
			}
			return throw_(new IsNotAFunction("clone"));
		}
		case DONE: {
			ObjectArray *multi = proto->toObjectArray();
			if (multi) {
				cloned->proto.clean();
				multi->vector.clone(cloned->proto);
			}
			else {
				cloned->proto[0] = proto;
			}
			State *p = prev;
			p->ret(cloned);
			delete this;
			return p;
		}
		default:
			throw NotImplemented();
		}
	}

	void Inherit::StateImpl::trace() {
		if (cloned) {
			cloned->mark();
		}
	}

	void Inherit::StateImpl::ret(Object *obj) {
		cloned = obj;
	}


	class Flat : public ObjectBuiltIn {
	public:
		Object * run(Scope *scope) override;
		static Object *getInstance();
	};

	Object * Flat::run(Scope *scope) {
		Object *fobj = new Object();
		scope->this_->flat(fobj);
		return fobj;
	}

	Object * Flat::getInstance() {
		static Flat __this;
		return &__this;
	}


	SuperObject::SuperObject() : Object(true) {
		objects.insert("clone", Clone::getInstance());
		objects.insert("flat", Flat::getInstance());
		objects.insert("instanceOf", InstanceOf::getInstance());
		objects.insert("->", Inherit::getInstance());
	}

	Object * SuperObject::getInstance() {
		static SuperObject __this;
		return &__this;
	}

	void SuperObject::flat(Object *fobj) {
		// do nothing
	}
}
