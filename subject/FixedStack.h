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

#include "Exception.h"

namespace goat {

	template <typename Type, unsigned int Capacity> class FixedStack {
	protected:
		Type data[Capacity];
		unsigned int count;

		FixedStack(const FixedStack &) {
		}

		void operator=(const FixedStack &) {
		}

	public:

		FixedStack() {
			count = 0;
		}

		void push(Type item) {
			if (count == Capacity) {
				throw StackOverflow();
			}

			data[count++] = item;
		}

		Type pop() {
			if (count == 0) {
				throw StackIsEmpty();
			}

			count--;
			return data[count];
		}

		Type top() {
			if (count == 0) {
				throw StackIsEmpty();
			}

			return data[count - 1];
		}

		unsigned int size() {
			return count;
		}

		class StackOverflow : public Exception {
		public:
			RawString toRawString() override {
				return L"stack overflow";
			}
		};

		class StackIsEmpty : public Exception {
		public:
			RawString toRawString() override {
				return L"stack is empty";
			}
		};

	};

}