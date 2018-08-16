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
#include "Vector.h"

namespace goat {

	class WideString;

	class String {
		friend class StringBuilder;
		friend class WideString;
		friend class WideStringBuilder;
	protected:
		class Buffer {
		public:
			unsigned int refs,
				len;
			char *data;

			Buffer(unsigned int len);
			~Buffer();
			inline void release();
		};

		Buffer *buff;

		String(unsigned int len);

	public:
		String();
		String(const char *cstr);
		String(const char *cstr, unsigned int len);
		String(const String & str);
		String(String && str);
		~String();
		String & operator=(const char *cstr);
		String & operator=(const String & str);
		char operator[] (unsigned int idx);
		unsigned int len() const;
		const char *cstr() const;
		bool operator==(const char *cstr);
		bool operator!=(const char *cstr);
		bool operator==(const String &str);
		bool operator!=(const String &str);
		bool operator<(const String &str);
		bool operator==(const WideString &wstr);
		bool operator!=(const WideString &wstr);
		bool operator<(const WideString &wstr);
		void *getHash() const;
		String replace(char from, char to);
		String substr(unsigned int start);
		String substr(unsigned int start, unsigned int count);
		void split(char ch, Vector<String> &vec);

		class OutOfBounds : public Exception {
		public:
			RawString toRawString() override;
		};

		static String valueOf(lint val);
		static String valueOf(ldouble val, unsigned int precision, bool trim);
		WideString toWideString();
	};

}