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

#include "FileName.h"

namespace goat {

#ifdef _WIN32
	String FileName::s_currentPath = ".\\";
#else
	String FileName::s_currentPath = "./";
#endif

	String FileName::extractName(String fullName) {
		int i, len = (int)fullName.len();
		for (i = len - 1; i >= 0; i--) {
			char c = fullName[i];
			if (c == '\\' || c == '/') {
				break;
			}
		}
		return fullName.substr(i + 1);
	}

	String FileName::extractPath(String fullName) {
		int i, len = (int)fullName.len();
		for (i = len - 1; i >= 0; i--) {
			char c = fullName[i];
			if (c == '\\' || c == '/') {
				break;
			}
		}
		return i > 0 ? fullName.substr(0, i) : nullptr;
	}
}
