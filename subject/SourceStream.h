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

#include "Source.h"
#include "InputStream.h"

namespace goat {

	class SourceStream : public Source {
	protected:
		class LocationImpl : public Location {
		protected:
			String name;
			int row,
				column;
		public:
			LocationImpl(String _name, int _row, int _column);
			String toString() override;
		};

		InputStream<char> *stream;
		String stname;
		int row,
			column;
		bool end;
		char C;

	public:
		SourceStream(InputStream<char> *_stream);
		char get() override;
		char next() override;
		Location *location() override;
	};

}