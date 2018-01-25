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

#include "In.h"
#include "StringBuilder.h"

namespace goat {

	In::In(bool _declVar, Identifier *_name) {
		loc = _name->loc;
		declVar = _declVar;
		name = _name;
	}

	In * In::toIn() {
		return this;
	}

	void In::trace() {
		name->mark();
	}

	String In::toString() {
		if (declVar) {
			return (StringBuilder() << "var " << name->name << " in").toString();
		}
		else {
			return (StringBuilder() << name->name << " in").toString();
		}
	}
}
