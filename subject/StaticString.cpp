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

#include "StaticString.h"
#include "ObjectString.h"
#include "StringBuilder.h"
#include "Unicode.h"

namespace goat {

	StaticString * StaticString::toStaticString() {
		return this;
	}

	State * StaticString::createState(State *prev) {
		return new StateImpl(prev, this);
	}

	State * StaticString::StateImpl::next() {
		State *p = prev;
		p->ret(new ObjectString(expr->text));
		delete this;
		return p;
	}

	String StaticString::toString() {
		StringBuilder b;
		b << '\"';
		for (unsigned int i = 0, l = text.len(); i < l; i++) {
			wchar c = text[i];
			switch (c) {
				case '\r':
					b << "\r"; break;
				case '\n':
					b << "\n"; break;
				case '\t':
					b << "\t"; break;
				case '\"':
					b << "\\\""; break;
				case '\\':
					b << "\\\\"; break;
				default: {
					char buff[5] = { 0, 0, 0, 0, 0 };
					Unicode::UTF8EncodeChar(c, buff);
					b << buff;
				}
			}
		}
		b << '\"';
		return b.toString();
	}
}