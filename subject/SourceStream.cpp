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

#include "SourceStream.h"
#include "StringBuilder.h"

namespace goat {

	SourceStream::LocationImpl::LocationImpl(String _name, int _row, int _column) 
		: name(_name), row(_row), column(_column) {
	}

	String SourceStream::LocationImpl::toString() {
		return (StringBuilder() << name << ", " << row << '.' << column).toString();
	}

	SourceStream::SourceStream(InputStream<char> *_stream) {
		stream = _stream;
		stname = stream->name();
		row = 1;
		column = 1;
		data = stream->read();
	}

	char SourceStream::get() {
		return data.get(0);
	}

	char SourceStream::next() {
		if (!data.hasValue) {
			return 0;
		}
		if (data.value == '\n') {
			row++;
			column = 1;
		}
		else {
			column++;
		}
		data = stream->read();
		return data.get(0);
	}

	Location * SourceStream::location() {
		return new LocationImpl(stname, row, column);
	}
}
