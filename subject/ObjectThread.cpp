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

#include "ObjectThread.h"
#include "ObjectBuiltIn.h"

namespace goat {

	ObjectThread::ObjectThread(Function *_function, Scope *_context) :
		ObjectFunction(_function, _context) {
		proto[0] = Proto::getInstance();
		thread = nullptr;
	}

	ObjectThread * ObjectThread::toObjectThread() {
		return this;
	}

	void ObjectThread::trace() {
		if (context) {
			context->mark();
		}
	}

	class ObjThreadClone : public ObjectBuiltIn {
	public:
		Object * run(Scope *scope) override;
		static Object *getInstance();
	};

	class ObjThreadRun : public ObjectBuiltIn {
	public:
		Object * run(Scope *scope) override;
		static Object *getInstance();
	};

	class ObjThreadStop : public ObjectBuiltIn {
	public:
		Object * run(Scope *scope) override;
		static Object *getInstance();
	};

	ObjectThread::Proto::Proto() {
		status = PERMANENT | LOCKED;
		proto[0] = ObjectFunction::Proto::getInstance();

		objects.insert("clone", ObjThreadClone::getInstance());
		objects.insert("run", ObjThreadRun::getInstance());
		objects.insert("stop", ObjThreadStop::getInstance());
	}


	Object * ObjectThread::Proto::getInstance() {
		static Proto __this;
		return &__this;
	}


	Object * ObjThreadClone::run(Scope *scope) {
		ObjectThread *this_ = scope->this_->toObjectThread();
		ObjectThread *obj = new ObjectThread(this_->function, this_->context);
		this_->clone(obj);
		return obj;
	}

	Object *ObjThreadClone::getInstance() {
		static ObjThreadClone __this;
		return &__this;
	}


	Object * ObjThreadRun::run(Scope *scope) {
		ObjectThread *this_ = scope->this_->toObjectThread();
		if (!this_->thread)
			this_->thread = new Thread(this_->function, this_->context->clone(), this_);
		return nullptr;
	}

	Object *ObjThreadRun::getInstance() {
		static ObjThreadRun __this;
		return &__this;
	}


	Object * ObjThreadStop::run(Scope *scope) {
		ObjectThread *this_ = scope->this_->toObjectThread();
		if (this_->thread)
			delete this_->thread;
		return nullptr;
	}

	Object * ObjThreadStop::getInstance() {
		static ObjThreadStop __this;
		return &__this;
	}
}