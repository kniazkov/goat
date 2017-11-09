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

#include "Token.h"
#include "Source.h"
#include "Exception.h"

namespace goat {

	class Scanner {
	protected:
		Source *src;

		inline char get();
		inline char next();
		static inline bool isSpace(char c);
		static inline bool isLetter(char c);
		static inline bool isDigit(char c);
		static inline bool isOperator(char c);
		Token * getToken(Location **loc);

	public:
		Scanner(Source *_src);
		Token * getToken();

		class UnknownSymbol : public Exception {
		public:
			String loc;
			char symbol;

			UnknownSymbol(Location *_loc, char _symbol);
			RawString toRawString() override;
		};
	};

	char Scanner::get() {
		return src->get();
	}

	char Scanner::next() {
		return src->next();
	}

	bool Scanner::isSpace(char c) {
		switch (c)
		{
		case ' ':
		case '\t':
		case '\n':
		case '\r':
			return true;
		default:
			return false;
		}
	}

	bool Scanner::isLetter(char c) {
		return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
	}

	bool Scanner::isDigit(char c) {
		return c >= '0' && c <= '9';
	}

	bool Scanner::isOperator(char c) {
		switch (c)
		{
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '=':
		case '!':
		case '>':
		case '<':
			return true;
		default:
			return false;
		}
	}
}
