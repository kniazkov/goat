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

#include "Utils.h"
#include "Platform.h"

namespace goat {

	unsigned int Utils::strLen(const char *cstr) {
		if (cstr) {
			const char *begin = cstr;
			while (*cstr++) {
			}
			return (unsigned int)(cstr - begin - 1);
		}
		return 0;
	}

	int Utils::strCmp(const char *a, const char *b) {
		if (a == nullptr || !*a) {
			if (b == nullptr || !*b) {
				return 0;
			}
			return -1;
		}

		if (b == nullptr || !*b) {
			return 1;
		}

		while ((*a || *b) && *a == *b) {
			a++;
			b++;
		}

		if (*a == *b)
			return 0;

		return *a < *b ? -1 : 1;
	}

	unsigned int Utils::wstrLen(const wchar *cwstr) {
		if (cwstr) {
			const wchar *begin = cwstr;
			while (*cwstr++) {
			}
			return (unsigned int)(cwstr - begin - 1);
		}
		return 0;
	}

	int Utils::wstrCmp(const wchar *a, const wchar *b) {
		if (a == nullptr || !*a) {
			if (b == nullptr || !*b) {
				return 0;
			}
			return -1;
		}

		if (b == nullptr || !*b) {
			return 1;
		}

		while ((*a || *b) && *a == *b) {
			a++;
			b++;
		}

		if (*a == *b)
			return 0;

		return *a < *b ? -1 : 1;
	}

	int Utils::strWstrCmp(const char *a, const wchar *b) {
		if (a == nullptr) {
			if (b == nullptr) {
				return 0;
			}
			return -1;
		}

		if (b == nullptr) {
			return 1;
		}

		while ((*a || *b) && (wchar)(*a) == *b) {
			a++;
			b++;
		}

		if ((wchar)(*a) == *b)
			return 0;

		return (wchar)(*a) < *b ? -1 : 1;
	}

	const char* Utils::fileNameFromFullPath(const char *path) {
		if (!path) {
			return nullptr;
		}

		const char *begin = path;
		while (*path) {
			if (*path == '\\' || *path == '/') {
				begin = path + 1;
			}
			path++;
		}
		return begin;
	}

	void Utils::print(const wchar *cwstr) {
		while (*cwstr) {
			Platform::putChar(*cwstr);
			cwstr++;
		}
	}

	void Utils::printErr(const wchar *cwstr) {
		while (*cwstr) {
			Platform::putCharErr(*cwstr);
			cwstr++;
		}
	}
}
