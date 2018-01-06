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

#include "WideStringBuilder.h"
#include "Utils.h"

namespace goat {

	void WideStringBuilder::expand(unsigned int len) {
		if (size + len > capacity) {
			capacity = Utils::max(capacity * 2, size + len);
			wchar *tmp = new wchar[capacity + 1];
			if (data) {
				Utils::memCopy(tmp, data, size);
				delete[] data;
			}
			data = tmp;
		}
	}

	void WideStringBuilder::append(const char *cstr, unsigned int len) {
		if (!cstr || !len) {
			return;
		}
		expand(len);
		for (unsigned int i = 0; i < len; i++) {
			data[size + i] = cstr[i];
		}
		size += len;
		data[size] = 0;
	}

	void WideStringBuilder::append(const wchar *cwstr, unsigned int len) {
		if (!cwstr || !len) {
			return;
		}
		expand(len);
		Utils::memCopy(data + size, cwstr, len);
		size += len;
		data[size] = 0;
	}

	WideStringBuilder::WideStringBuilder() : data(nullptr), size(0), capacity(0) {
	}

	WideStringBuilder::~WideStringBuilder() {
		delete[] data;
	}

	WideString WideStringBuilder::toWideString() {
		return WideString(data, size);
	}

	void WideStringBuilder::clear() {
		delete[] data;
		data = nullptr;
		size = 0;
		capacity = 0;
	}

	void WideStringBuilder::write(wchar ch) {
		append(&ch, 1);
	}

	void WideStringBuilder::print() {
		if (data) {
			Utils::print(data);
		}
	}

	void WideStringBuilder::reserve(unsigned int len) {
		if (size + len < capacity) {
			expand(len);
		}
	}

	WideStringBuilder & WideStringBuilder::operator<<(wchar ch) {
		append(&ch, 1);
		return *this;
	}

	WideStringBuilder & WideStringBuilder::operator<<(const wchar *cwstr) {
		append(cwstr, Utils::wstrLen(cwstr));
		return *this;
	}

	WideStringBuilder & WideStringBuilder::operator<<(const WideString &wstr) {
		if (wstr.buff) {
			append(wstr.buff->data, wstr.buff->len);
		}
		return *this;
	}

	WideStringBuilder & WideStringBuilder::operator<<(int val) {
		String str = String::valueOf(val);
		append(str.buff->data, str.buff->len);
		return *this;
	}

	WideStringBuilder & WideStringBuilder::operator<<(const char *cstr) {
		append(cstr, Utils::strLen(cstr));
		return *this;
	}

	WideStringBuilder & WideStringBuilder::operator<<(const String &str) {
		if (str.buff) {
			append(str.buff->data, str.buff->len);
		}
		return *this;
	}

	RawString WideStringBuilder::toRawString() {
		RawString rs;
		if (size) {
			rs.data = new wchar[size + 1];
			rs.len = size;
			rs.dynamic = true;
			Utils::memCopy(rs.data, data, size);
			rs.data[size] = '\0';
		}
		return rs;
	}

}