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

#include "WideString.h"
#include "String.h"
#include "OutputStream.h"

namespace goat {

	class WideStringBuilder : public OutputStream<wchar> {
	protected:
		wchar *data;
		uint32 size, capacity;

		WideStringBuilder(const WideStringBuilder &) {
		};

		void operator=(const WideStringBuilder &) {
		}

		void expand(uint32 len);
		void append(const char *cstr, uint32 len);
		void append(const wchar *cwstr, uint32 len);

	public:
		WideStringBuilder();
		~WideStringBuilder();
		WideString toWideString();
	
		void clear();
		void write(wchar ch) override;
		void print();
		void reserve(uint32 len);
		RawString toRawString();
		WideStringBuilder & operator<<(wchar ch);
		WideStringBuilder & operator<<(const wchar *cwstr);
		WideStringBuilder & operator<<(const WideString &wstr);
		WideStringBuilder & operator<<(int val);
		WideStringBuilder & operator<<(const char *cstr);
		WideStringBuilder & operator<<(const String &str);
	};

}
