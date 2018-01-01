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

#include "Unicode.h"
#include "WideStringBuilder.h"

namespace goat {

	RawString Unicode::BadUTF8::toRawString() {
		return L"bad UTF8";
	}

	WideString Unicode::UTF8Decode(String s) {
		unsigned int i = 0,
			len = s.len();
		WideStringBuilder result;
		while (i < len) {
			unsigned char c0 = (unsigned char)s[i];
			wchar w = 0;
			if ((c0 & 0x80) == 0) {
				w = c0;
				i++;
			}
			else if ((c0 & 0xE0) == 0xC0) {
				if (i + 1 >= len) {
					throw BadUTF8();
				}
				unsigned char c1 = (unsigned char)s[i + 1];
				if ((c1 & 0xC0) != 0x80) {
					throw BadUTF8();
				}
				w = ((c0 & 0x1F) << 6) + (c1 & 0x3F);
				i += 2;
			}
			else if ((c0 & 0xF0) == 0xE0) {
				if (i + 2 >= len) {
					throw BadUTF8();
				}
				unsigned char c1 = (unsigned char)s[i + 1];
				unsigned char c2 = (unsigned char)s[i + 2];
				if ((c1 & 0xC0) != 0x80 || (c2 & 0xC0) != 0x80) {
					throw BadUTF8();
				}
				w = ((c0 & 0xF) << 12) + ((c1 & 0x3F) << 6) + (c2 & 0x3F);
				i += 3;
			}
#ifdef WCHAR_32
			else if ((c0 & 0xF8) == 0xF0) {
				if (i + 3 >= len) {
					throw BadUTF8();
				}
				unsigned char c1 = (unsigned char)s[i + 1];
				unsigned char c2 = (unsigned char)s[i + 2];
				unsigned char c3 = (unsigned char)s[i + 3];
				if ((c1 & 0xC0) != 0x80 || (c2 & 0xC0) != 0x80 || (c3 & 0xC0) != 0x80) {
					throw BadUTF8();
				}
				w = ((c0 & 0x7) << 18) + ((c1 & 0x3F) << 12) + ((c2 & 0x3F) << 6) + (c3 & 0x3F);
				i += 4;
			}
#endif
			else {
				throw BadUTF8();
			}
			result << w;
		}
		return result.toWideString();
	}

	unsigned int Unicode::UTF8EncodeChar(wchar w, char *c) {
		if (w < 0x80) {
			c[0] = (char)w;
			return 1;
		}
		if (w < 0x800) {
			c[0] = (char)((w & 0x7C0) >> 6) + 0xC0;
			c[1] = (char)(w & 0x3F) + 0x80;
			return 2;
		}
		if (w < 0x10000) {
			c[0] = (char)((w & 0xF000) >> 12) + 0xE0;
			c[1] = (char)((w & 0xFC0) >> 6) + 0x80;
			c[2] = (char)(w & 0x3F) + 0x80;
			return 3;
		}
#ifdef WCHAR_32
		if (w < 0x200000) {
			c[0] = (char)((w & 0x1C0000) >> 18) + 0xF0;
			c[1] = (char)((w & 0x3F000) >> 12) + 0x80;
			c[2] = (char)((w & 0xFC0) >> 6) + 0x80;
			c[3] = (char)(w & 0x3F) + 0x80;
			return 3;
		}
#endif
		return 0;
	}

}