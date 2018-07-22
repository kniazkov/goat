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

#include "String.h"
#include "WideString.h"
#include "Utils.h"

namespace goat {
	
	String::String() {
		buff = nullptr;
	}

	String::String(const char *cstr) {
		unsigned int len = Utils::strLen(cstr);
		if (len > 0) {
			buff = new Buffer(len);
			Utils::memCopy(buff->data, cstr, len);
		}
		else {
			buff = nullptr;
		}
	}

	String::String(unsigned int len) {
		if (len > 0) {
			buff = new Buffer(len);
		}
		else {
			buff = nullptr;
		}
	}

	String::String(const char *cstr, unsigned int len) {
		if (len > 0) {
			buff = new Buffer(len);
			Utils::memCopy(buff->data, cstr, len);
		}
		else {
			buff = nullptr;
		}
	}

	String::String(const String &str) {
		buff = str.buff;
		if (buff) {
			buff->refs++;
		}
	}

	String::String(String &&str) {
		buff = str.buff;
		str.buff = nullptr;
	}

	String::~String() {
		if (buff) {
			buff->release();
		}
	}

	String & String::operator=(const char *cstr) {
		if (buff) {
			buff->release();
		}
		unsigned int len = Utils::strLen(cstr);
		if (len > 0) {
			buff = new Buffer(len);
			Utils::memCopy(buff->data, cstr, len);
		}
		else {
			buff = nullptr;
		}
		return *this;
	}

	String & String::operator=(const String &str) {
		if (buff != str.buff) {
			if (buff) {
				buff->release();
			}
			buff = str.buff;
			if (buff) {
				buff->refs++;
			}
		}
		return *this;
	}

	char String::operator[](unsigned int idx) {
		if (!buff || idx >= buff->len) {
			throw OutOfBounds();
		}

		return buff->data[idx];
	}

	unsigned int String::len() const {
		return buff ? buff->len : 0;
	}

	const char * String::cstr() const {
		return buff ? buff->data : nullptr;
	}

	bool String::operator==(const char *cstr) {
		return Utils::strCmp(buff ? buff->data : nullptr, cstr) == 0;
	}

	bool String::operator!=(const char *cstr) {
		return Utils::strCmp(buff ? buff->data : nullptr, cstr) != 0;
	}

	bool String::operator==(const String &str) {
		return Utils::strCmp(buff ? buff->data : nullptr, str.buff ? str.buff->data : nullptr) == 0;
	}

	bool String::operator!=(const String &str) {
		return Utils::strCmp(buff ? buff->data : nullptr, str.buff ? str.buff->data : nullptr) != 0;
	}

	bool String::operator<(const String &str) {
		return Utils::strCmp(buff ? buff->data : nullptr, str.buff ? str.buff->data : nullptr) < 0;
	}

	bool String::operator==(const WideString &wstr) {
		return Utils::strWstrCmp(buff ? buff->data : nullptr, wstr.buff ? wstr.buff->data : nullptr) == 0;
	}

	bool String::operator!=(const WideString &wstr) {
		return Utils::strWstrCmp(buff ? buff->data : nullptr, wstr.buff ? wstr.buff->data : nullptr) != 0;
	}

	bool String::operator<(const WideString &wstr) {
		return Utils::strWstrCmp(buff ? buff->data : nullptr, wstr.buff ? wstr.buff->data : nullptr) < 0;
	}

	void * String::getHash() const {
		return (void*)buff;
	}

	String String::replace(char from, char to) {
		if (buff) {
			String ret(buff->len);
			for (unsigned int i = 0; i < buff->len; i++) {
				char ch = buff->data[i];
				ret.buff->data[i] = ch == from ? to : ch;
			}
			return ret;
		}
		else {
			return nullptr;
		}
	}

	String String::substr(unsigned int start) {
		if (buff && buff->len > start) {
			if (start == 0) {
				return *this;
			}
			String ret(buff->len - start);
			Utils::memCopy(ret.buff->data, buff->data + start, buff->len - start);
			return ret;
		}
		else {
			return nullptr;
		}
	}

	String String::substr(unsigned int start, unsigned int count) {
		if (buff && buff->len > start) {
			if (start == 0 && count == buff->len) {
				return *this;
			}
			count = Utils::min(count, buff->len - start);
			String ret(count);
			Utils::memCopy(ret.buff->data, buff->data + start, count);
			return ret;
		}
		else {
			return nullptr;
		}
	}

	void String::split(char ch, Vector<String> &vec) {
		if (buff->len > 0) {
			unsigned int end = 0, begin = 0;
			while (end < buff->len) {
				if (buff->data[end] == ch) {
					vec.pushBack(substr(begin, end - begin));
					begin = end + 1;
				}
				end++;
			}
			vec.pushBack(substr(begin, end - begin));
		}
	}

	String::Buffer::Buffer(unsigned int len) {
		refs = 1;
		data = new char[len + 1];
		data[len] = '\0';
		this->len = len;
	}

	String::Buffer::~Buffer() {
		delete[] data;
	}

	void String::Buffer::release() {
		if (!(--refs)) {
			delete this;
		}
	}

	String String::valueOf(long long int val) {
		bool neg = false;
		if (val < 0) {
			neg = true;
			val = -val;
		}
		char buff[16];
		char *ptr = &buff[15];
		*ptr = '\0';
		do {
			*--ptr = (char)(val % 10) + '0';
			val /= 10;
		} while (val);
		if (neg) {
			*--ptr = '-';
		}
		return String(ptr);
	}

	String String::valueOf(long double val, unsigned int precision, bool trim) {
		if (precision < 1) {
			precision = 1;
		}
		bool neg = false;
		if (val < 0) {
			neg = true;
			val = -val;
		}
		char buff[32];
		char *fptr = &buff[31];
		*fptr = '\0';
		char *iptr = fptr - precision - 1;
		*iptr = '.';
		long long int ival = (long long int)val;
		auto tail = val - (long double)ival;
		long long int fval = (long long int)(tail * Utils::pow10(precision + 1));
		auto lastDigit = fval % 10;
		fval /= 10;
		if (lastDigit >= 5) {
			fval++;
			if (fval >= Utils::pow10(precision)) {
				fval = 0;
				ival++;
			}
		}
		if (fval > 0) {
			bool zero = true;
			for (unsigned int k = 0; k < precision; k++) {
				auto digit = fval % 10;
				if (digit == 0) {
					if (zero && trim) {
						*--fptr = '\0';
					}
					else {
						*--fptr = '0';
					}
				}
				else {
					zero = false;
					*--fptr = (char)(digit)+'0';
				}
				fval /= 10;
			}
		}
		else {
			*(iptr + 1) = '0';
			*(iptr + 2) = '\0';
		}
		do {
			*--iptr = (char)(ival % 10) + '0';
			ival /= 10;
		} while (ival);
		if (neg) {
			*--iptr = '-';
		}
		return String(iptr);
	}

	WideString String::toWideString() {
		if (buff) {
			WideString w(buff->len);
			for (unsigned int i = 0; i < buff->len; i++) {
				w.buff->data[i] = (wchar)(buff->data[i]);
			}
			return w;
		}
		return nullptr;
	}

	RawString String::OutOfBounds::toRawString() {
		return L"string: index out of bounds";
	}
}
