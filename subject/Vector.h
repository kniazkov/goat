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

#include "Assert.h"
#include "Exception.h"
#include "Type.h"
#include "Utils.h"

namespace goat {

	template <typename Type> class Vector {
	protected:
		Type *data;
		uint32 size, capacity;

		Vector(const Vector &) {
		}

		void operator=(const Vector &) {
		}

	public:
		Vector() : data(nullptr), size(0), capacity(0) {
		}

		~Vector() {
			delete[] data;
		}

		void pushBack(Type item) {
			if (size == capacity) {
				capacity = Utils::max(capacity * 2, (uint32)2);
				Type *tmp = new Type[capacity];
				if (data) {
					Utils::memCopy(tmp, data, size);
					delete[] data;
				}
				data = tmp;
			}
			data[size++] = item;
		}

		void pushBack(Type *items, uint32 count) {
			uint32 newSize = size + count;
			if (newSize > capacity) {
				Type *tmp = new Type[newSize];
				if (data) {
					Utils::memCopy(tmp, data, size);
					delete[] data;
				}
				data = tmp;
				capacity = newSize;
			}
			Utils::memCopy(data + size, items, count);
			size = newSize;
		}

		Type &operator[](uint32 idx) {
			if (idx >= size) {
				throw OutOfBounds();
			}

			return data[idx];
		}

		Type & get(uint32 idx) {
			if (idx >= size) {
				throw OutOfBounds();
			}

			return data[idx];
		}

		uint32 len() {
			return size;
		}

		template <typename Function> void forEach(Function func) {
			if (data) {
				Type *item = data,
					*end = data + size;
				while (item != end) {
					func(*item);
					item++;
				}
			}
		}

		void clone(Vector &v) {
			assert(v.size == 0);
			if (size > 0) {
				v.data = new Type[size];
				Utils::memCopy(v.data, data, size);
				v.capacity = size;
				v.size = size;
			}
		}

		class OutOfBounds : public Exception {
		public:
			RawString toRawString() override {
				return L"vector: index out of bounds";
			}
		};
	};
}
