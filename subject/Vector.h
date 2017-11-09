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

#include "Assert.h"
#include "Exception.h"
#include "Utils.h"

namespace goat {

	template <typename Type> class Vector {
	protected:
		union {
			Type s[2] ;
			struct {
				Type *data;
				unsigned int capacity;
			} d;
		} b;
		unsigned int size;

		Vector(const Vector &) {
		}

		void operator=(const Vector &) {
		}

	public:
		Vector() {
			size = 0;
		}

		~Vector() {
			if (size > 2) {
				delete[] b.d.data;
			}
		}

		void clean() {
			if (size > 2) {
				delete[] b.d.data;
			}
			size = 0;
		}

		void clone(Vector &v) {
			assert(v.size == 0);
			if (size < 3) {
				for (unsigned int i = 0; i < size; i++)
					v.b.s[i] = b.s[i];
			}
			else {
				v.b.d.data = new Type[size];
				Utils::memCopy(v.b.d.data, b.d.data, size);
				v.b.d.capacity = size;
			}
			v.size = size;
		}

		void pushBack(Type item) {
			if (size < 2) {
				b.s[size++] = item;
			}
			else if (size == 2) {
				Type *tmp = new Type[4];
				tmp[0] = b.s[0];
				tmp[1] = b.s[1];
				tmp[2] = item;
				b.d.data = tmp;
				b.d.capacity = 4;
				size = 3;
			}
			else {
				if (size == b.d.capacity) {
					b.d.capacity = b.d.capacity * 2;
					Type *tmp = new Type[b.d.capacity];
					Utils::memCopy(tmp, b.d.data, size);
					delete[] b.d.data;
					b.d.data = tmp;
				}
				b.d.data[size++] = item;
			}
		}

		Type &operator[](unsigned int idx) {
			if (idx >= size) {
				throw OutOfBounds();
			}

			return size < 3 ? b.s[idx] : b.d.data[idx];
		}

		unsigned int len() {
			return size;
		}

		template <typename Function> void forEach(Function func) {
			Type *data = size < 3 ? b.s : b.d.data;
			Type *item = data,
				*end = data + size;
			while (item != end) {
				func(*item);
				item++;
			}
		}

		bool indexOf(Type &item, unsigned int *index = nullptr) {
			unsigned int k = 0;
			if (size < 3) {
				for (; k < size; k++) {
					if (b.s[k] == item) {
						if (index) {
							*index = k;
						}
						return true;
					}
				}
			}
			else {
				for (; k < size; k++) {
					if (b.d.data[k] == item) {
						if (index) {
							*index = k;
						}
						return true;
					}
				}
			}
			return false;
		}

		class OutOfBounds : public Exception {
		public:
			RawString toRawString() override {
				return L"vector: index out of bounds";
			}
		};
	};

}