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

#include "ObjectFile.h"
#include "ObjectBuiltIn.h"
#include "Resource.h"
#include "ObjectInteger.h"
#include "ObjectChar.h"
#include "ObjectBoolean.h"
#include "ObjectConstInteger.h"

namespace goat {

	ObjectFile::ObjectFile(Platform::File *_file) {
		file = _file;
		proto[0] = Proto::getInstance();
	}

	ObjectFile::~ObjectFile() {
		delete file;
	}

	ObjectFile * ObjectFile::toObjectFile() {
		return this;
	}

	WideString ObjectFile::toWideString() {
		return Resource::w_File;
	}

	void ObjectFile::trace() {
	}

	class ObjFileClone : public ObjectBuiltIn {
	public:
		Object * run(Scope *scope) override;
		static Object *getInstance();
	};

	class ObjFileRead : public ObjectBuiltIn {
	public:
		Object * run(Scope *scope) override;
		static Object *getInstance();
	};

	class ObjFileEof : public ObjectBuiltIn {
	public:
		Object * run(Scope *scope) override;
		static Object *getInstance();
	};


	class ObjFileMode : public Object {
	public:
		ObjFileMode();
		static Object *getInstance();
	};


	ObjectFile::Proto::Proto() {
		status = PERMANENT;

		objects.insert("clone", ObjFileClone::getInstance());
		objects.insert("read", ObjFileRead::getInstance());
		objects.insert("eof", ObjFileEof::getInstance());
		objects.insert("Mode", ObjFileMode::getInstance());
	}

	Object * ObjectFile::Proto::getInstance() {
		static Proto __this;
		return &__this;
	}


	Object * ObjFileClone::run(Scope *scope) {
		return scope->this_;
	}

	Object *ObjFileClone::getInstance() {
		static ObjFileClone __this;
		return &__this;
	}


	Object * ObjFileRead::run(Scope *scope) {
		ObjectFile *this_ = scope->this_->toObjectFile();
		return new ObjectInteger(this_->file->read());
	}

	Object *ObjFileRead::getInstance() {
		static ObjFileRead __this;
		return &__this;
	}


	Object * ObjFileEof::run(Scope *scope) {
		ObjectFile *this_ = scope->this_->toObjectFile();
		return new ObjectBoolean(this_->file->eof());
	}

	Object *ObjFileEof::getInstance() {
		static ObjFileEof __this;
		return &__this;
	}


	ObjFileMode::ObjFileMode() : Object(true) {
		objects.insert("READ", ObjectConstInteger<0>::getInstance());
		objects.insert("WRITE", ObjectConstInteger<1>::getInstance());
		objects.insert("APPEND", ObjectConstInteger<2>::getInstance());
	}

	Object *ObjFileMode::getInstance() {
		static ObjFileMode __this;
		return &__this;
	}
}