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

#include "SourceString.h"
#include "StringBuilder.h"

namespace goat {

	SourceString::LocationImpl::LocationImpl(int _row, int _column) : row(_row), column(_column) {
	}

	String SourceString::LocationImpl::toString() {
		return ( StringBuilder() << row << '.' << column ).toString();
	}

	SourceString::SourceString(String _data) {
		data = _data;
		idx = 0;
		len = data.len();
		row = 1;
		column = 1;
	}

	char SourceString::get() {
		return idx < len ? data[idx] : 0;
	}

	char SourceString::next() {
		if (idx >= len)
			return 0;

		char c = data[idx];
		if (c == '\n') {
			row++;
			column = 1;
		}
		else {
			column++;
		}

		idx++;
		return idx < len ? data[idx] : 0;
	}

	Location * SourceString::location() {
		return new LocationImpl(row, column);
	}
}
