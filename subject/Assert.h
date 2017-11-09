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

#include "Exception.h"

namespace goat {

	class AssertionFailed : public Exception {
	public:
		const char *file;
		int line;
		const char *expression;

		AssertionFailed(const char *_file, int _line, const char *_expression);
		RawString toRawString() override;
	};

#define assert(expr) if (!(expr)) throw AssertionFailed(__FILE__, __LINE__, #expr);

}