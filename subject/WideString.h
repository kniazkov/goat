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

#include "Exception.h"
#include "Type.h"

namespace goat {

	class String;

	class WideString {
		friend class String;
		friend class WideStringBuilder;
	protected:
		class Buffer {
		public:
			unsigned int refs,
				len;
			wchar *data;

			Buffer(unsigned int len);
			~Buffer();
			inline void release();
		};

		Buffer *buff;

		WideString(unsigned int len);

	public:
		WideString();
		WideString(const wchar *cwstr);
		WideString(const wchar *cwstr, unsigned int len);
		WideString(const WideString & wstr);
		WideString(WideString && wstr);
		~WideString();
		WideString & operator=(const wchar *cwstr);
		WideString & operator=(const WideString & wstr);
		wchar operator[] (unsigned int idx);
		unsigned int len() const;
		const wchar *cwstr() const;
		RawString toRawString();
		String toString();
		WideString subString(unsigned int start, unsigned int count);
		bool operator==(const wchar *cstr);
		bool operator!=(const wchar *cstr);
		bool operator==(const WideString &wstr);
		bool operator!=(const WideString &wstr);
		bool operator<(const WideString &wstr);
		bool operator<=(const WideString &wstr);
		bool operator==(const String &str);
		bool operator!=(const String &str);
		bool operator<(const String &str);
		WideString operator+(const WideString &wstr);

		template <typename Function> void forEach(Function func) {
			if (buff) {
				wchar *data = buff->data,
					*end = buff->data + buff->len;
				while (data != end) {
					func(*data);
					data++;
				}
			}
		}

		class OutOfBounds : public Exception {
		public:
			RawString toRawString() override;
		};

		static WideString valueOf(long long int val);
		static WideString valueOf(long double val, unsigned int precision, bool trim);
	};


}