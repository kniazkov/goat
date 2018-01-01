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

#include "StringBuilder.h"
#include "Utils.h"

namespace goat {

	void StringBuilder::append(const char *cstr, unsigned int len) {
		if (!cstr || !len) {
			return;
		}

		if (size + len > capacity) {
			capacity = Utils::max(capacity * 2, size + len);
			char *tmp = new char[capacity + 1];
			if (data) {
				Utils::memCopy(tmp, data, size);
				delete[] data;
			}
			data = tmp;
		}

		Utils::memCopy(data + size, cstr, len);
		size += len;
		data[size] = '\0';
	}

	StringBuilder::StringBuilder() : data(nullptr), size(0), capacity(0) {
	}

	StringBuilder::~StringBuilder() {
		delete[] data;
	}

	String StringBuilder::toString() {
		return String(data, size);
	}

	StringBuilder & StringBuilder::operator<<(char ch) {
		append(&ch, 1);
		return *this;
	}

	StringBuilder & StringBuilder::operator<<(const char *cstr) {
		append(cstr, Utils::strLen(cstr));
		return *this;
	}

	StringBuilder & StringBuilder::operator<<(const String &str) {
		if (str.buff) {
			append(str.buff->data, str.buff->len);
		}
		return *this;
	}

	StringBuilder & StringBuilder::operator<<(int val) {
		String str = String::valueOf(val);
		append(str.buff->data, str.buff->len);
		return *this;
	}
}