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
#include "Type.h"

namespace goat {

	class String;

	class WideString {
		friend class String;
		friend class WideStringBuilder;
	protected:
		class Buffer {
		public:
			uint32 refs,
				len;
			wchar *data;
			bool ascii;

			Buffer(uint32 len);
			~Buffer();
			inline void release();
		};

		Buffer *buff;

		WideString(uint32 len);

	public:
		WideString();
		WideString(const wchar *cwstr);
		WideString(const wchar *cwstr, uint32 len);
		WideString(const WideString & wstr);
		WideString(WideString && wstr);
		~WideString();
		WideString & operator=(const wchar *cwstr);
		WideString & operator=(const WideString & wstr);
		wchar operator[] (uint32 idx);
		uint32 len() const;
		const wchar *cwstr() const;
		RawString toRawString();
		bool isAscii();
		String toString();
		WideString subString(uint32 start, uint32 count);
		bool operator==(const wchar *cstr);
		bool operator!=(const wchar *cstr);
		bool operator==(const WideString &wstr);
		bool operator!=(const WideString &wstr);
		bool operator<(const WideString &wstr);
		bool operator<=(const WideString &wstr);
		bool operator>(const WideString &wstr);
		bool operator>=(const WideString &wstr);
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

		static WideString valueOf(lint val);
		static WideString valueOf(ldouble val, uint8 precision, bool trim);
	};


}
