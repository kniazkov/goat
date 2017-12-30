#pragma once

#include "Exception.h"
#include "Utils.h"

namespace goat {

	template <typename Type> class Vector {
	protected:
		Type *data;
		unsigned int size, capacity;

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
				capacity = Utils::max(capacity * 2, (unsigned int)2);
				Type *tmp = new Type[capacity];
				if (data) {
					Utils::memCopy(tmp, data, size);
					delete[] data;
				}
				data = tmp;
			}
			data[size++] = item;
		}

		Type &operator[](unsigned int idx) {
			if (idx >= size) {
				throw OutOfBounds();
			}

			return data[idx];
		}

		unsigned int len() {
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
