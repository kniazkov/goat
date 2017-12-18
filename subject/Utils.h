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

#include "Type.h"

namespace goat {

	class Utils {
	public:
		template <typename Type>
		static Type alignDown(Type x, Type align) {
			return (x & ~(align - 1));
		}

		template <typename Type>
		static Type alignUp(Type x, Type align) {
			return (x & (align - 1)) ? alignDown(x, align) + align : x;
		}

		template <typename Type>
		static void memSet(Type *ptr, Type value, unsigned int count) {
			while (count--) {
				*ptr++ = value;
			}
		}

		template <typename Dst, typename Src>
		static void memCopy(Dst *dst, Src *src, unsigned int count) {
			if (dst > src) {
				dst += count;
				src += count;
				while (count--) {
					*--dst = *--src;
				}
			}
			else {
				while (count--) {
					*dst++ = *src++;
				}
			}
		}

		template <typename Type>
		static Type max(Type a, Type b) {
			return a > b ? a : b;
		}

		static unsigned int strLen(const char *cstr);
		static int strCmp(const char *a, const char *b);
		static int strCmp(const char *a, const char *b, unsigned int n);
		static unsigned int wstrLen(const wchar *cwstr);
		static int wstrCmp(const wchar *a, const wchar *b);
		static int strWstrCmp(const char *a, const wchar *b);
		static const char *fileNameFromFullPath(const char *path);
		
		static long double pow10(unsigned int n);
		
		static void print(const wchar *cwstr);
		static void printErr(const wchar *cwstr);

	};

}