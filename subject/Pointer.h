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

namespace goat {

	template <typename Type> class Pointer {
	protected:
		Type *obj;

	public:
		Pointer() : obj(nullptr) {
		}

		Pointer(Type *_obj) {
			obj = _obj;
			if (obj && obj->refs >= 0) {
				obj->refs++;
			}
		}

		Pointer(const Pointer &ptr) {
			obj = ptr.obj;
			if (obj && obj->refs >= 0) {
				obj->refs++;
			}
		}

		Pointer(Pointer &&ptr) {
			obj = ptr.obj;
			ptr.obj = nullptr;
		}

		~Pointer() {
			if (obj && obj->refs > 0 && !(--obj->refs)) {
				delete obj;
			}
		}

		Pointer & operator=(Type *_obj) {
			if (obj && obj->refs > 0 && !(--obj->refs)) {
				delete obj;
			}
			obj = _obj;
			if (obj && obj->refs >= 0) {
				obj->refs++;
			}
			return *this;
		}

		Pointer & operator=(const Pointer &ptr) {
			if (obj != ptr.obj) {
				if (obj && obj->refs > 0 && !(--obj->refs)) {
					delete obj;
				}
				obj = ptr.obj;
				if (obj && obj->refs >= 0) {
					obj->refs++;
				}
			}
			return *this;
		}

		operator bool() {
			return obj != nullptr;
		}

		Type * ptr() {
			return obj;
		}

		Type * operator->() {
			return obj;
		}
	};

}