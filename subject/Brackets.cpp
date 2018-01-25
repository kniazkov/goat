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

#include "Brackets.h"
#include "StringBuilder.h"

namespace goat {

	Brackets::Brackets(Bracket *brk) {
		loc = brk->loc;
		symbol = brk->symbol;
		tokens = new TokenList();
	}

	Brackets * Brackets::toBrackets() {
		return this;
	}

	Brackets::~Brackets() {
		delete tokens;
	}

	void Brackets::trace() {
		tokens->mark();
	}

	String Brackets::toString() {
		StringBuilder b;
		b << symbol;
		tokens->forEach([&](Token *tok) {
			b << tok->toString();
		});
		switch (symbol) {
			case '(':
				b << ')';
				break;
			case '[':
				b << ']';
				break;
			case '{':
				b << '}';
				break;
		}
		return b.toString();
	}
}