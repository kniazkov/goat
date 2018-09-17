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
#include "ObjectException.h"
#include "ObjectByteArray.h"

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

	WideString ObjectFile::toWideString(Set<Object*> &set) {
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

	class ObjFilePosition : public ObjectBuiltIn {
	public:
		Object * run(Scope *scope) override;
		static Object *getInstance();
	};

	class ObjFileSeek : public ObjectBuiltIn {
	public:
		Object * run(Scope *scope) override;
		static Object *getInstance();
	};

	class ObjFileMode : public Object {
	public:
		ObjFileMode();
		static Object *getInstance();
	};

	class ObjFileOrigin : public Object {
	public:
		ObjFileOrigin();
		static Object *getInstance();
	};


	ObjectFile::Proto::Proto() {
		status = PERMANENT | LOCKED;

		objects.insert(createIndex("clone"), ObjFileClone::getInstance());
		objects.insert(createIndex("read"), ObjFileRead::getInstance());
		objects.insert(createIndex("eof"), ObjFileEof::getInstance());
		objects.insert(createIndex("position"), ObjFilePosition::getInstance());
		objects.insert(createIndex("seek"), ObjFileSeek::getInstance());
		objects.insert(createIndex("Mode"), ObjFileMode::getInstance());
		objects.insert(createIndex("Origin"), ObjFileOrigin::getInstance());
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
		ObjectArray * args = scope->arguments;
		unsigned int argsCount = args->vector.len();
		if (argsCount == 1) {
			ObjectInteger *count = args->vector[0]->toObjectInteger();
			if (count && count->value >= 0) {
				ObjectByteArray *result = new ObjectByteArray();
				if (count->value > 0) {
					unsigned char *buff = new unsigned char[count->value];
					long int fact = this_->file->read(buff, (long int)count->value);
					result->vector.pushBack(buff, fact);
					delete[] buff;
				}
				return result;
			}
		}
		else if (argsCount == 0) {
			return new ObjectInteger(this_->file->read());
		}
		return new IllegalArgument();
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


	Object * ObjFilePosition::run(Scope *scope) {
		ObjectFile *this_ = scope->this_->toObjectFile();
		return new ObjectInteger(this_->file->position());
	}

	Object *ObjFilePosition::getInstance() {
		static ObjFilePosition __this;
		return &__this;
	}


	Object * ObjFileSeek::run(Scope *scope) {
		ObjectFile *this_ = scope->this_->toObjectFile();
		ObjectArray * args = scope->arguments;
		if (args->vector.len() == 2) {
			ObjectInteger *offset = args->vector[0]->toObjectInteger();
			ObjectInteger *origin = args->vector[1]->toObjectInteger();
			if (offset && origin) {
				switch (origin->value) {
					case Platform::File::Origin::BEGIN:
					case Platform::File::Origin::END:
					case Platform::File::Origin::CURRENT:
						return new ObjectInteger(
							this_->file->seek(
								(long int)offset->value,
								(Platform::File::Origin)origin->value
							)
						);
					default:
						break;
				}
			}
		}
		return new IllegalArgument();
	}

	Object *ObjFileSeek::getInstance() {
		static ObjFileSeek __this;
		return &__this;
	}


	ObjFileMode::ObjFileMode() : Object(true) {
		objects.insert(createIndex("READ"), ObjectConstInteger<Platform::File::Mode::READ>::getInstance());
		objects.insert(createIndex("WRITE"), ObjectConstInteger<Platform::File::Mode::WRITE>::getInstance());
		objects.insert(createIndex("APPEND"), ObjectConstInteger<Platform::File::Mode::APPEND>::getInstance());
	}

	Object *ObjFileMode::getInstance() {
		static ObjFileMode __this;
		return &__this;
	}


	ObjFileOrigin::ObjFileOrigin() : Object(true) {
		objects.insert(createIndex("BEGIN"), ObjectConstInteger<Platform::File::Origin::BEGIN>::getInstance());
		objects.insert(createIndex("END"), ObjectConstInteger<Platform::File::Origin::END>::getInstance());
		objects.insert(createIndex("CURRENT"), ObjectConstInteger<Platform::File::Origin::CURRENT>::getInstance());
	}

	Object *ObjFileOrigin::getInstance() {
		static ObjFileOrigin __this;
		return &__this;
	}
}